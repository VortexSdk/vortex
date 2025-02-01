#pragma once

#include "../metap/metap.hpp"
#include "../metap/structs.hpp"
#include "../numbers.hpp"
#include "utils.hpp"
#include "vortex/math.hpp"
#include "vortex/writer.hpp"

struct AllocatorState {
    PIN_STRUCT(AllocatorState, len, 0_usize, pos, 0_usize, ptr, reinterpret_cast<u8 *>(0))
};

template <typename T>
concept AllocatorStrategy = requires(T t) {
    { t.reset(null) } -> same_as<void>;
    { t.alloc(null, 0_usize, 0_usize) } -> same_as<void *>;
    { t.free(null, null, 0_usize, 0_usize) } -> same_as<bool>;
    { t.resize(null, null, 0_usize, 0_usize, 0_usize) } -> same_as<void *>;
};

template <AllocatorStrategy T> struct Allocator {
    PIN_STRUCT(Allocator, strategy, T(), state, (AllocatorState()))

    static Allocator<T> init(usize len, u8 *ptr, usize pos = 0) {
        return Allocator<T>(T{}, AllocatorState(len, pos, ptr));
    }

    template <typename E = u8>
    Slice<E> alloc_aligned(this Allocator &self, usize c, usize alignment) {
        if (!isPowerOfTwo(alignment)) [[unlikely]] {
            return Slice<E>();
        }

        return Slice<E>::init(
            c, reinterpret_cast<E *>(self.strategy.alloc(&self.state, sizeof(E) * c, alignment))
        );
    }

    template <typename E = u8> Slice<E> alloc(this Allocator &self, usize c) {
        return self.alloc_aligned<E>(c, alignof(E));
    }

    template <typename E> E *create(this Allocator &self) {
        return self.template alloc<E>(1).ptr;
    }

    template <typename E = u8>
    Slice<E> dup_aligned(this Allocator &self, Slice<const E> b, usize alignment) {
        if (!isPowerOfTwo(alignment)) [[unlikely]] {
            return Slice<E>();
        }

        Slice<E> s = self.template alloc_aligned<E>(b.len, alignment);
        if (s.is_empty()) return Slice<E>();

        memcpy(
            reinterpret_cast<void *>(s.ptr), reinterpret_cast<const void *>(b.ptr),
            sizeof(E) * b.len
        );

        return s;
    }
    template <typename E = u8> Slice<E> dup(this Allocator &self, Slice<const E> b) {
        return self.dup_aligned(b, alignof(E));
    }

    template <typename E = u8>
    Slice<E> resize_aligned(this Allocator &self, Slice<E> b, usize new_s, usize alignment) {
        if (!isPowerOfTwo(alignment)) [[unlikely]] {
            return Slice<E>();
        }

        return Slice<E>::init(
            new_s, reinterpret_cast<E *>(self.strategy.resize(
                       &self.state, reinterpret_cast<void *>(b.ptr), sizeof(E) * b.len,
                       sizeof(E) * new_s, alignment
                   ))
        );
    }
    template <typename E> Slice<E> resize(this Allocator &self, Slice<E> b, usize new_s) {
        return self.template resize_aligned<E>(b, new_s, alignof(E));
    }

    template <typename E = u8>
    Slice<E>
    resize_or_alloc_aligned(this Allocator &self, Slice<E> b, usize new_s, usize alignment) {
        if (!isPowerOfTwo(alignment)) [[unlikely]] {
            return Slice<E>();
        }

        Slice<E> r =
            self.template resize_aligned<E>(Slice<E>::init(b.len, b.ptr), new_s, alignment);
        if (!r.is_empty()) return r;

        Slice<E> a = self.template alloc_aligned<E>(new_s, alignment);
        if (!a.is_empty()) {
            memcpy(
                reinterpret_cast<void *>(a.ptr), reinterpret_cast<const void *>(b.ptr),
                sizeof(E) * b.len
            );
            self.template free_aligned<E>(Slice<E>::init(b.len, b.ptr), alignment);
        }

        return a;
    }
    template <typename E = u8>
    Slice<E> resize_or_alloc(this Allocator &self, Slice<E> b, usize new_s) {
        return self.resize_or_alloc_aligned(b, new_s, alignof(E));
    }

    template <typename E = u8>
    bool free_aligned(this Allocator &self, Slice<E> b, usize alignment) {
        if (!isPowerOfTwo(alignment)) [[unlikely]] {
            return true;
        }

        return self.strategy.free(
            &self.state, reinterpret_cast<void *>(b.ptr), sizeof(E) * b.len, alignment
        );
    }
    template <typename E = u8> bool free(this Allocator &self, Slice<E> b) {
        return self.free_aligned(b, alignof(E));
    }

    void reset(this Allocator &self) {
        return self.strategy.reset(&self.state);
    }

    template <typename E> void destroy(this Allocator &self, E *b) {
        return self.template free<E>(Slice<E>::init(1, b));
    }
};

#define TRY_ALLOC(expr, t) TRY(expr, t, SysResKind::NOMEM)
