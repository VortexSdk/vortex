#pragma once

#include "Array.hpp"
#include "linux/syscall/SysRes.hpp"
#include "math.hpp"
#include "mem/Allocator.hpp"
#include "metap/metap.hpp"
#include "numbers.hpp"

template <typename T> inline static T v [16 / sizeof(T)] = {0};

template <typename T = u8> union StringData {
    static_assert(
        is_same_v<T, u8> || is_same_v<T, u16> || is_same_v<T, u32>,
        "BasicString can only be represented as UTF-8, UTF-16, or UTF-32."
    );

    T *ptr;
    // u8 -> 16, u16 -> 8, u32 -> 4
    Array<T, 16 / sizeof(T)> buf;

    StringData() : ptr(null) {}
    StringData(T *_ptr) : ptr(_ptr) {}
    template <usize N> StringData(T (&_buf) [N]) : buf(_buf) {}
};

template <typename T = u8> struct BasicString {
    static constexpr usize PREFIX_COUNT   = 4 / sizeof(T);
    static constexpr usize STR_DATA_COUNT = 16 / sizeof(T);

    u32 len;
    u32 cap;
    StringData<T> data;
    Array<T, PREFIX_COUNT> prefix;

    BasicString() : len(0), cap(0), data(StringData<T>()), prefix(Array<T, PREFIX_COUNT>()) {}
    BasicString(u32 _len, u32 _cap, StringData<T> _data, T _prefix [PREFIX_COUNT])
        : len(_len), cap(_cap), data(_data), prefix(_prefix) {}

    BasicString(const BasicString &other)            = delete;
    BasicString &operator=(const BasicString &other) = delete;

    BasicString(BasicString &&other) noexcept
        : len(exchange(other.len, 0_u32)), cap(exchange(other.cap, 0_u32)),
          prefix(move(other.prefix)) {
        if (cap > STR_DATA_COUNT) data.ptr = other.data.ptr;
        else data.buf = move(other.data.buf);
        data.ptr = null;
    }
    BasicString &operator=(BasicString &&other) noexcept {
        if (this != &other) [[likely]] {
            len    = exchange(other.len, 0_u32);
            cap    = exchange(other.cap, 0_u32);
            prefix = move(other.prefix);

            if (cap > STR_DATA_COUNT) data.ptr = other.data.ptr;
            else data.buf = move(other.data.buf);
            data.ptr = null;
        }
        return *this;
    }

    template <AllocatorStrategy U>
    static SysRes<BasicString<T>> init(Allocator<U> *a, u32 capacity = STR_DATA_COUNT) {
        BasicString s;
        s.cap = capacity;
        if (capacity > STR_DATA_COUNT) {
            const Slice<T> alloc_res = a->template alloc<T>(capacity);
            if (alloc_res.is_empty()) return SysResKind::NOMEM;
            s.data.ptr = alloc_res.ptr;
        }

        return move(s);
    }

    template <AllocatorStrategy U> void deinit(this BasicString<T> &self, Allocator<U> *a) {
        if (self.cap > STR_DATA_COUNT) a->free(Slice<T>::init(self.cap, self.data.ptr));
    }

    template <AllocatorStrategy U>
    static SysRes<BasicString<T>> from(Allocator<U> *a, const Slice<const T> *const slice) {
        BasicString<T> s = TRY(BasicString<T>::init(a, slice->len));
        if (slice->len <= STR_DATA_COUNT) {
            memcpy(
                reinterpret_cast<void *>(s.prefix), reinterpret_cast<const void *>(slice->ptr),
                slice->len * sizeof(T)
            );
        } else {
            memcpy(
                reinterpret_cast<void *>(s.data.ptr), reinterpret_cast<const void *>(slice->ptr),
                slice->len * sizeof(T)
            );
        }
        s.len = slice->len;

        return move(s);
    }

    template <AllocatorStrategy U>
    SysRes<BasicString<T>> clone(this const BasicString<T> &self, Allocator<U> *a) {
        BasicString<T> s = TRY(BasicString<T>::init(a, self.cap));
        if (self.cap <= STR_DATA_COUNT) {
            memcpy(
                reinterpret_cast<void *>(s.prefix), reinterpret_cast<const void *>(self.prefix),
                self.len * sizeof(T)
            );
        } else {
            memcpy(
                reinterpret_cast<void *>(s.data.ptr), reinterpret_cast<const void *>(self.data.ptr),
                self.len * sizeof(T)
            );
        }
        s.len = self.len;

        return move(s);
    }

    Slice<T> as_mut_slice(this BasicString<T> &self) {
        if (self.len <= STR_DATA_COUNT) return Slice<T>::init(self.len, self.prefix);

        return Slice<T>::init(self.len, self.data.ptr);
    }
    Slice<const T> as_slice(this const BasicString<T> &self) {
        if (self.len <= STR_DATA_COUNT) return Slice<const T>::init(self.len, self.prefix);

        return Slice<const T>::init(self.len, self.data.ptr);
    }

    template <AllocatorStrategy U>
    bool grow(this BasicString<T> &self, Allocator<U> *a, usize new_cap) {
        if (new_cap <= self.cap) [[unlikely]]
            return false;
        if (new_cap <= STR_DATA_COUNT) {
            self.cap = new_cap;
            return false;
        }

        T *new_ptr;
        if (self.cap <= STR_DATA_COUNT) {
            Slice<T> alloc_res = a->template alloc<T>(new_cap);
            if (alloc_res.is_empty()) return true;
            new_ptr = alloc_res.ptr;

            memcpy(
                reinterpret_cast<void *>(new_ptr), reinterpret_cast<const void *>(self.prefix),
                self.len * sizeof(T)
            );
        } else {
            Slice<T> resize_res =
                a->template resize_or_alloc<T>(Slice<T>::init(self.cap, self.data.ptr), new_cap);
            if (resize_res.is_empty()) return true;
            new_ptr = resize_res.ptr;
        }

        self.data.ptr = new_ptr;
        self.cap      = new_cap;
        return false;
    }

    template <AllocatorStrategy U> bool push(this BasicString<T> &self, Allocator<U> *a, T ch) {
        if (self.len >= self.cap)
            if (self.grow(a, self.cap * 2)) return true;

        if (self.len <= STR_DATA_COUNT) self.prefix [self.len] = ch;
        else self.data.ptr [self.len] = ch;

        self.len++;
        return false;
    }

    template <AllocatorStrategy U>
    bool append(this BasicString<T> &self, Allocator<U> *a, const Slice<const T> *const slice) {
        const usize required_cap = self.len + slice->len;
        if (required_cap > self.cap) {
            const usize new_cap = max(required_cap, static_cast<usize>(self.cap * 2));
            if (self.grow(a, new_cap)) return true;
        }

        if (self.len <= STR_DATA_COUNT) {
            memcpy(
                reinterpret_cast<void *>(self.prefix + self.len),
                reinterpret_cast<const void *>(slice->ptr), slice->len * sizeof(T)
            );
        } else {
            memcpy(
                reinterpret_cast<void *>(self.data.ptr + self.len),
                reinterpret_cast<const void *>(slice->ptr), slice->len * sizeof(T)
            );
        }

        self.len += slice->len;
        return false;
    }

    template <AllocatorStrategy U>
    SysRes<BasicString<T>>
    substr(this const BasicString<T> &self, Allocator<U> *a, usize start, usize sublen) {
        if (start >= self.len || start + sublen > self.len) [[unlikely]]
            return SysResKind::INVAL;

        Slice<const T> slice = self.as_slice();
        return BasicString<T>::from(a, &Slice<const T>::init(sublen, &slice.ptr [start]));
    }

    T *nth(this const BasicString<T> &self, usize n) {
        if (n >= self.len) [[unlikely]]
            return null;

        if (n < STR_DATA_COUNT) return &self.prefix [n];
        return &self.data.ptr [n];
    }

    Slice<T> mut_subslice(this BasicString<T> &self, usize start, usize sublen) {
        if (start >= self.len || start + sublen > self.len) [[unlikely]]
            return Slice<T>();

        if (self.cap <= STR_DATA_COUNT) return Slice<T>::init(sublen, &self.prefix [start]);
        return Slice<T>::init(sublen, &self.data.ptr [start]);
    }
    Slice<const T> subslice(this const BasicString<T> &self, usize start, usize sublen) {
        if (start >= self.len || start + sublen > self.len) [[unlikely]]
            return Slice<const T>();

        if (self.cap <= STR_DATA_COUNT) return Slice<const T>::init(sublen, &self.prefix [start]);
        return Slice<const T>::init(sublen, &self.data.ptr [start]);
    }
};

using String = BasicString<u8>;

static_assert(sizeof(BasicString<u8>) == 32, "u8 variants of BasicString should be 32 bytes.");
static_assert(sizeof(BasicString<u16>) == 32, "u16 variants of BasicString should be 32 bytes.");
static_assert(sizeof(BasicString<u32>) == 32, "u32 variants of BasicString should be 32 bytes.");
