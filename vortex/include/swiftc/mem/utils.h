#pragma once

#include "../numbers.h"

#define nullptr (void *)0

/// Copies from `src` to `dest`.
FNDECL_PREFIX void *mem_copy(void *__restrict dest, void *__restrict src, usize n) {
    if (src != nullptr && dest != nullptr) {}
    for (usize i = 0; i < n; i++) {
        ((char *)dest) [i] = ((char *)src) [i];
    }
    return (void *)((char *)dest + n);
}

/// Returns the length of a null-terminated string.
FNDECL_PREFIX usize strlen(const char *ptr) {
    usize i = 0;
    while (ptr [i] != 0) i += 1;
    return i;
}
