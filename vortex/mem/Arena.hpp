#pragma once

#include "../numbers.hpp"
#include "Allocator.hpp"
#include "utils.hpp"

struct Arena {
    // Arena(Arena const &)            = delete;
    // Arena &operator=(Arena const &) = delete;
    Arena()  = default;
    ~Arena() = default;

    inline void *alloc(AllocatorState *a, usize s) { // NOLINT(misc-const-correctness)
        usize const new_pos = a->pos + s;
        if (new_pos > a->len) return null_ptr;

        void *ret = reinterpret_cast<char *>(a->ptr) + a->pos;
        a->pos    = new_pos;

        return ret;
    }

    inline void *
    resize(AllocatorState *a, void *p, usize s, usize new_s) { // NOLINT(misc-const-correctness)
        if ((reinterpret_cast<char *>(a->ptr) + a->pos) == (reinterpret_cast<char *>(p) + new_s)) {
            usize const new_pos = a->pos + (new_s - s);
            if (new_pos < a->len) {
                a->pos = new_pos;

                return p;
            }
        }

        return NULL;
    }

    void free(AllocatorState *, void *, usize) {} // NOLINT(misc-const-correctness)
};
