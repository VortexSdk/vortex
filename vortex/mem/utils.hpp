#pragma once

#include "../diagnostics.hpp"
#include "../linux/syscall/syscall.hpp"
#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "musl.hpp"

#define PAGE_SIZE 4096
#define NULL      (reinterpret_cast<void *>(0))

template <typename T> static T *null() {
    return reinterpret_cast<T *>(NULL);
}

// A zero-sized struct that represents nothing.
struct None {};

template <typename T> static inline T zeroed() {
    T m;
    memset(reinterpret_cast<char *>(&m), 0, sizeof(T));

    return m;
}

#define __ZEROED_ARR_INNER(t, c, ctr)                                                              \
    ({                                                                                             \
        t UNIQUE_NAME(arr, ctr) [c];                                                               \
        ::vortex::memset(reinterpret_cast<char *>(&UNIQUE_NAME(arr, ctr)), 0, sizeof(t) * c);      \
        UNIQUE_NAME(arr, ctr);                                                                     \
    })
#define ZEROED_ARR(t, c) __ZEROED_ARR_INNER(t, c, __COUNTER__)

#include "Slice.hpp"
