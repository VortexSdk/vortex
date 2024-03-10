#pragma once

#include "../numbers.h"
#include "Allocator.h"
#include "utils.h"

typedef struct ArenaAllocator {
    void *mem;
    usize len;
    usize pos;
} ArenaAllocator;

/// Allocates an Arena from the mem.
VORTEX_PREFIX ArenaAllocator arena_init(void *mem, usize len) {
    return (ArenaAllocator){.mem = mem, .len = len, .pos = 0};
}

/// Realloc's implementation that's used in the Allocator interface.
VORTEX_PREFIX void *arena_realloc(void *ctx, void *buf, usize len, usize new_len) {
    ArenaAllocator *self = (ArenaAllocator *)ctx;
    if (buf == nullptr) {
        // Alloc
        void *ret;
        usize new_pos = self->pos + len;
        if (new_pos > self->len) {
            return nullptr;
        }

        ret       = (char *)self->mem + self->pos;
        self->pos = new_pos;
        return ret;
    } else if (new_len != 0) {
        // Resize
        if (((char *)self->mem + self->pos) == ((char *)buf + len)) {
            usize new_pos = self->pos + (new_len - len);
            if (new_pos < self->len) {
                self->pos = new_pos;
                return buf;
            } // Falls to `return nullptr;` if failes.
        }
    }

    // Free
    return nullptr;
}

/// Returns an Allocator interface.
VORTEX_PREFIX Allocator arena_allocator(ArenaAllocator *arena) {
    return (Allocator){.ptr = (void *)arena, .realloc = arena_realloc};
}

/// Resets the Arena.
VORTEX_PREFIX void arena_rest(ArenaAllocator *arena) {
    (*arena).pos = 0;
}
