#pragma once

#include "../diagnostics.hpp"
#include "../linux/syscall/syscall.hpp"
#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "musl.hpp"

#define PAGE_SIZE 4096
#define U32_0     static_cast<u32>(0)
#define USIZE_0   static_cast<usize>(0)
#define NULL      (reinterpret_cast<void *>(0))

template <typename T> static T *null() {
    return reinterpret_cast<T *>(NULL);
}

// A zero-sized struct that represents nothing.
struct None {};

template <typename T> static T zeroed() {
    T m;
    memset(reinterpret_cast<char *>(&m), 0, sizeof(T));

    return m;
}

#include "Slice.hpp"
