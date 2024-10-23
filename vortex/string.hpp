#pragma once

#include "linux/syscall/syscall.hpp"
#include "mem/Allocator.hpp"
#include "mem/Arena.hpp"
#include "mem/utils.hpp"
#include "metap/metap.hpp"
#include "numbers.hpp"

template <typename T = u8> union StringData {
    static_assert(
        is_same_v<T, u8> || is_same_v<T, u16> || is_same_v<T, u32>,
        "BasicString can only be represented as UTF-8, UTF-16, or UTF-32."
    );

    T *ptr;
    // u8 -> 12, u16 -> 6, u32 -> 3
    T buf [12 / sizeof(T)];
};

template <typename T = u8> struct BasicString {
    static constexpr usize PREFIX_COUNT        = 4 / sizeof(T);
    static constexpr usize STR_DATA_COUNT      = 12 / sizeof(T);
    static constexpr usize MAX_SMALL_STR_COUNT = PREFIX_COUNT + STR_DATA_COUNT;

    u32 len{0};
    u32 cap{0};
    T prefix [PREFIX_COUNT]{0};
    StringData<T> data{{.buf = {0}}};

    BasicString(const BasicString &t)            = delete;
    BasicString &operator=(const BasicString &t) = delete;
    BasicString() : len(0), cap(0), data({.buf = {0}}) {
        memset(prefix, 0, sizeof(prefix));
    }
    BasicString(BasicString &&s) noexcept
        : len(exchange(s.len, U32_0)), cap(exchange(s.cap, U32_0)),
          data(exchange(s.data, StringData<T>{.buf = {}})) {
        memcpy(prefix, s.prefix, sizeof(prefix));
        memset(s.prefix, 0, sizeof(s.prefix));
    }

    template <AllocatorStrategy U>
    static SysRes<BasicString<T>> init(Allocator<U> *a, u32 capacity = MAX_SMALL_STR_COUNT) {
        BasicString s;
        s.cap = capacity;
        if (capacity > MAX_SMALL_STR_COUNT) {
            const auto alloc_res = a->template alloc<T>(capacity - PREFIX_COUNT);
            if (alloc_res.is_empty()) return SysRes<BasicString<T>>::from_err(SysResKind::NOMEM);
            s.data = StringData{.ptr = alloc_res.ptr};
        }

        return SysRes<BasicString<T>>::from_successful(move(s));
    }

    template <AllocatorStrategy U> void deinit(this BasicString<T> &self, Allocator<U> *a) {
        if (self.cap > MAX_SMALL_STR_COUNT) {
            a->free(Slice<T>::init(self.cap - PREFIX_COUNT, self.data.ptr));
        }
    }
};

static_assert(sizeof(BasicString<u8>) == 32, "u8 variants of BasicString should be 32 bytes.");
static_assert(sizeof(BasicString<u16>) == 32, "u16 variants of BasicString should be 32 bytes.");
static_assert(sizeof(BasicString<u32>) == 32, "u32 variants of BasicString should be 32 bytes.");
