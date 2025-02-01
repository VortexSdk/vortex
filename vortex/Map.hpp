#pragma once

#include "linux/syscall/SysRes.hpp"
#include "math.hpp"
#include "mem/Allocator.hpp"
#include "mem/utils.hpp"
#include "metap/structs.hpp"
#include "numbers.hpp"
#include "rapidhash.hpp"
#include "vortex/string.hpp"

template <typename T> using MapEqlFn = bool (*)(const T &, const T &);
template <typename T> bool default_map_eql_fn(const T &a, const T &b) {
    const auto *a_ptr = reinterpret_cast<const u8 *const>(&a);
    const auto *b_ptr = reinterpret_cast<const u8 *const>(&b);
    for (usize i = 0; i < sizeof(T); i++)
        if (a_ptr [i] != b_ptr [i]) return false;

    return true;
}

template <typename T> using MapHashFn = u64 (*)(const T &);
template <typename T> static u64 default_map_hash_fn(const T &s) {
    return rapidhash_withSeed(&s, sizeof(T), 0);
}

template <typename T> struct MapConfig {
    double max_load_factor{70.0};
    MapEqlFn<T> eql_fn{default_map_eql_fn};
    MapHashFn<T> hash_fn{default_map_hash_fn};
};

enum class MapEntryStatus : u8 {
    Empty = 0,
    Filled,
    Tombstone,
};

template <typename T, typename Y> struct MapEntry {
    T *key{null};
    Y *value{null};
    MapEntryStatus status{MapEntryStatus::Empty};
};

enum class MapInsertRes : u8 {
    Success = 0,
    TableIsFull,
    FailedToRehash,
    OutOfMemory,
};

struct MapSearchResult {
    usize index;
    bool found;
    // Only used in find_index_inner for starting from index 0 in the table if the end of
    // it is reached and no empty spot found.
    bool run_me_again;
};

template <typename T, typename Y, MapConfig<T> CONFIG = MapConfig<T>{}> struct NoStorageMap {
    using CURRENT_MAP = NoStorageMap<T, Y, CONFIG>;
    static_assert(
        CONFIG.max_load_factor >= 10 && CONFIG.max_load_factor < 100,
        "Invalid load factor for CONFIG.max_load_factor."
    );

    PIN_STRUCT(NoStorageMap, count, 0_usize, entries, (Slice<MapEntry<T, Y>>()))

    MapSearchResult
    find_index_inner(this const CURRENT_MAP &self, const T key, usize idx, usize end_idx) {
        for (; idx < end_idx; idx++) {
            const MapEntry<T, Y> *entry = self.entries [idx];

            if (entry->status == MapEntryStatus::Empty)
                return MapSearchResult{.index = idx, .found = false, .run_me_again = false};
            if (entry->status == MapEntryStatus::Filled && CONFIG.eql_fn(key, *entry->key)) {
                return MapSearchResult{.index = idx, .found = true, .run_me_again = false};
            }
        }

        return MapSearchResult{.index = 0, .found = false, .run_me_again = true};
    }
    MapSearchResult find_index(this const CURRENT_MAP &self, const T key) {
        const usize start_idx           = CONFIG.hash_fn(key) & (self.entries.len - 1);
        const MapSearchResult find_res1 = self.find_index_inner(key, start_idx, self.entries.len);
        if (!find_res1.run_me_again) [[likely]] {
            return find_res1;
        }

        const MapSearchResult find_res2 = self.find_index_inner(key, 0, start_idx);
        if (!find_res2.run_me_again) [[likely]] {
            return find_res2;
        }

        return {self.entries.len, false, false}; // Table is full
    }

    template <AllocatorStrategy U>
    static SysRes<CURRENT_MAP> init_cap_unchecked(Allocator<U> *a, usize capacity) {
        CURRENT_MAP m;
        m.entries = a->template alloc<MapEntry<T, Y>>(capacity);
        if (m.entries.is_empty()) return SysResKind::NOMEM;

        for (usize i = 0; i < capacity; i++) {
            *m.entries [i] =
                MapEntry<T, Y>{.key = null, .value = null, .status = MapEntryStatus::Empty};
        }

        return m;
    }
    template <AllocatorStrategy U>
    static SysRes<CURRENT_MAP> init(Allocator<U> *a, usize capacity = 128) {
        return CURRENT_MAP::init_cap_unchecked(a, ceilPowerOfTwo(capacity));
    }

    template <AllocatorStrategy U> void deinit(this CURRENT_MAP &self, Allocator<U> *a) {
        a->free(self.entries);
    }

    template <AllocatorStrategy U>
    bool rehash(this CURRENT_MAP &self, Allocator<U> *a, usize new_capacity) {
        SysRes<CURRENT_MAP> res = CURRENT_MAP::init(a, new_capacity);
        if (res.is_err()) return true;
        CURRENT_MAP new_map = res.unwrap();

        for (usize i = 0; i < self.entries.len; i++) {
            MapEntry<T, Y> *entry = self.entries [i];
            if (entry->status == MapEntryStatus::Filled) [[likely]] {
                new_map.insert(a, entry->key, entry->value);
            }
        }

        a->free(self.entries);
        self.count   = new_map.count;
        self.entries = new_map.entries;

        return false;
    }
    template <AllocatorStrategy U> bool rehash_if_needed(this CURRENT_MAP &self, Allocator<U> *a) {
        if ((static_cast<double>(self.count) / static_cast<double>(self.entries.len)) >
            static_cast<double>(CONFIG.max_load_factor / 100.0)) [[unlikely]] {
            const bool res = self.rehash(a, self.entries.len * 2);
            if (res) return false;
        } else return false;

        return true;
    }

    Y *get(this const CURRENT_MAP &self, const T key) {
        MapSearchResult result = self.find_index(key);
        if (result.found) [[likely]]
            return self.entries [result.index]->value;

        return null;
    }

    template <AllocatorStrategy U>
    MapInsertRes insert(this CURRENT_MAP &self, Allocator<U> *a, T *key, Y *value) {
        self.rehash_if_needed(a);

        MapSearchResult result = self.find_index(*key);
        if (result.index == self.entries.len) [[unlikely]]
            return MapInsertRes::TableIsFull;

        MapEntry<T, Y> *entry = self.entries [result.index];
        if (!result.found) {
            *entry = MapEntry<T, Y>{.key = key, .value = value, .status = MapEntryStatus::Filled};
            self.count++;
        } else {
            entry->value = value;
        }

        return MapInsertRes::Success;
    }

    bool remove(this CURRENT_MAP &self, const T key) {
        MapSearchResult result = self.find_index(key);
        if (result.found) [[likely]] {
            *self.entries [result.index] =
                MapEntry<T, Y>{.key = null, .value = null, .status = MapEntryStatus::Tombstone};
            return false;
        }

        return true;
    }
};

template <typename T, typename Y, MapConfig<T> CONFIG = MapConfig<T>{}> struct Map {
    using CURRENT_MAP = Map<T, Y, CONFIG>;
    using CURRENT_NCM = NoStorageMap<T, Y, CONFIG>;

    PIN_STRUCT(
        Map, ncm, (CURRENT_NCM()), keys, reinterpret_cast<T *>(0), values, reinterpret_cast<Y *>(0),
        key_value_len, 0_usize
    )

    template <AllocatorStrategy U>
    static SysRes<CURRENT_MAP> init(Allocator<U> *a, usize capacity = 128) {
        CURRENT_MAP m;
        capacity                    = ceilPowerOfTwo(capacity);
        m.key_value_len             = capacity;
        SysRes<CURRENT_NCM> ncm_res = CURRENT_NCM::init_cap_unchecked(a, capacity);
        if (ncm_res.is_err()) return ncm_res.template return_err<CURRENT_MAP>();
        m.ncm                   = ncm_res.unwrap();

        const Slice<T> keys_res = a->template alloc<T>(capacity);
        if (keys_res.is_empty()) return SysResKind::NOMEM;
        m.keys                    = keys_res.ptr;

        const Slice<Y> values_res = a->template alloc<Y>(capacity);
        if (values_res.is_empty()) return SysResKind::NOMEM;
        m.values = values_res.ptr;

        return m;
    }

    template <AllocatorStrategy U> void deinit(this CURRENT_MAP &self, Allocator<U> *a) {
        self.ncm.deinit(a);
        a->free(Slice<T>::init(self.ncm.entries.len, self.keys));
        a->free(Slice<Y>::init(self.ncm.entries.len, self.values));
    }

    Y *get(this const CURRENT_MAP &self, const T key) {
        return self.ncm.get(key);
    }

    template <AllocatorStrategy U>
    MapInsertRes insert(this CURRENT_MAP &self, Allocator<U> *a, const T &key, const Y &value) {
        if (self.ncm.rehash_if_needed(a)) [[unlikely]] {
            Slice<T> keys_alloc_res = a->resize_or_alloc(
                Slice<T>::init(self.key_value_len, self.keys), self.ncm.entries.len
            );
            if (keys_alloc_res.is_empty()) {
                self.ncm.remove(key);
                return MapInsertRes::OutOfMemory;
            }
            Slice<Y> values_alloc_res = a->resize_or_alloc(
                Slice<Y>::init(self.key_value_len, self.values), self.ncm.entries.len
            );
            if (values_alloc_res.is_empty()) {
                self.ncm.remove(key);
                a->free(keys_alloc_res);
                return MapInsertRes::OutOfMemory;
            }

            self.keys          = keys_alloc_res.ptr;
            self.values        = values_alloc_res.ptr;
            self.key_value_len = self.ncm.entries.len;
        }

        MapSearchResult result = self.ncm.find_index(key);
        if (result.index == self.ncm.entries.len) [[unlikely]]
            return MapInsertRes::TableIsFull;

        MapEntry<T, Y> *entry = self.ncm.entries [result.index];
        if (!result.found) {
            T *new_key   = &self.keys [self.ncm.count];
            Y *new_value = &self.values [self.ncm.count];
            *new_key     = key;
            *new_value   = value;
            *entry       = MapEntry<T, Y>{
                      .key = new_key, .value = new_value, .status = MapEntryStatus::Filled
            };
            self.ncm.count++;
        } else {
            *entry->value = value;
        }

        return MapInsertRes::Success;
    }

    bool remove(this CURRENT_MAP &self, const T key) {
        return self.ncm.remove(key);
    }
};

static bool string_map_eql_fn(const String &s1, const String &s2) {
    return s1.eql(s2);
}
static u64 string_map_hash_fn(const String &s) {
    Slice<const u8> slice = s.as_slice();
    return rapidhash_withSeed(reinterpret_cast<const void *>(slice.ptr), slice.len, 0);
}
using StringMap =
    Map<String, String,
        MapConfig<String>{
            .eql_fn  = string_map_eql_fn,
            .hash_fn = string_map_hash_fn,
        }>;
