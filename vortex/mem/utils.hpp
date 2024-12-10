#pragma once

#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "musl.hpp"

#define PAGE_SIZE 4096
struct nullptr_t {
    template <typename T> constexpr operator T *() const noexcept {
        return 0;
    }
    template <typename T> constexpr operator T() const = delete;

    template <typename T> constexpr bool operator==(T *ptr) const noexcept {
        return !ptr;
    }
    template <typename T> constexpr bool operator!=(T *ptr) const noexcept {
        return ptr != 0;
    }
};
inline constexpr nullptr_t null;

// A zero-sized struct that represents nothing.
struct None {};

template <typename T> static inline T zeroed() {
    T m;
    memset(reinterpret_cast<char *>(&m), 0, sizeof(T));

    return m;
}

#define __ZEROED_ARR_INNER(t, c, arr)                                                              \
    ({                                                                                             \
        t arr [c];                                                                                 \
        ::vortex::memset(reinterpret_cast<char *>(&arr), 0, sizeof(t) * c);                        \
        arr;                                                                                       \
    })
#define ZEROED_ARR(t, c) __ZEROED_ARR_INNER(t, c, UNIQUE_NAME(arr))

#include "Slice.hpp"
