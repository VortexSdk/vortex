#pragma once

#include "../numbers.hpp"

#define PAGE_SIZE 4096
#define NULL      (reinterpret_cast<void *>(0))
static void *null_ptr = reinterpret_cast<void *>(0);

static void *memcpy(void *__restrict dest, void *__restrict src, usize n) {
    for (usize i = 0; i < n; i++) {
        (reinterpret_cast<char *>(dest)) [i] = (reinterpret_cast<char *>(src)) [i];
    }

    return reinterpret_cast<void *>((reinterpret_cast<char *>(dest) + n));
}

static usize strlen(const char *const s) {
    usize i = 0;
    for (;; i++) {
        if (s [i] == '\0') return i;
    }
}

template <typename T> struct Slice {
    T *ptr;
    usize len;

    Slice()  = delete;
    ~Slice() = default;
    Slice(T *_ptr, usize _len) : ptr(_ptr), len(_len) {}

    bool is_empty(this Slice &self) {
        return self.ptr == NULL;
    }

    T *get(this Slice &self, usize i) {
        if (i >= self.len) return NULL;

        return self.ptr [i];
    }
};
