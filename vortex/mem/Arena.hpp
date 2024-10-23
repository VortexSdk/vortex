#pragma once

#include "../diagnostics.hpp"
#include "../numbers.hpp"
#include "Allocator.hpp"
#include "utils.hpp"

DIAG_IGNORE_GCC_PUSH("-Wattributes")

// NOLINTBEGIN

struct Arena {
    Arena() {}
    Arena(Arena &&) noexcept {}
    Arena(const Arena &t)            = delete;
    Arena &operator=(const Arena &t) = delete;

    inline __attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow"))
    ) void *
    alloc(AllocatorState *a, usize s, usize alignment) {
        const usize aligned_pos = (a->pos + (alignment - 1)) & ~(alignment - 1);
        const usize new_pos     = aligned_pos + s;
        if (new_pos >= a->len) [[unlikely]] {
            return NULL;
        }

        a->pos = new_pos;
        return reinterpret_cast<char *>(a->ptr) + aligned_pos;
    }

    inline __attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow"))
    ) void *
    resize(AllocatorState *a, void *p, usize s, usize new_s, usize alignment) {
        usize pos =
            static_cast<usize>((reinterpret_cast<char *>(p) - reinterpret_cast<char *>(a->ptr)));
        if (pos + s == a->pos) {
            usize aligned_new_pos = (pos + (alignment - 1)) & ~(alignment - 1);
            usize new_pos         = aligned_new_pos + new_s;

            if (new_pos <= a->len) {
                a->pos = new_pos;
                return p;
            }
        }

        return NULL;
    }

    void free(AllocatorState *, void *, usize, usize) {}
};

// NOLINTEND

DIAG_IGNORE_GCC_POP
