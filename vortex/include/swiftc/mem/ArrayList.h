#pragma once

#include "../numbers.h"
#include "Allocator.h"
#include "utils.h"

#ifndef DEFAULT_ARRAYLIST_CAPACITY
    #define DEFAULT_ARRAYLIST_CAPACITY (1 * 1024)
#endif

typedef struct ArrayList {
    void *mem;
    usize cap;
    usize pos;
    Allocator allocator;
} ArrayList;

/// Allocates an Arena from the mem.
FNDECL_PREFIX ArrayList arraylist_init_with_capacity(Allocator allocator, usize cap) {
    return (ArrayList){.mem = mem_alloc(allocator, cap), .cap = cap, .pos = 0};
}

/// Allocates an Arena from the mem.
FNDECL_PREFIX ArrayList arraylist_init(Allocator allocator) {
    return arraylist_init_with_capacity(allocator, DEFAULT_ARRAYLIST_CAPACITY);
}

/// Pushes a new item to the ArrayList.
FNDECL_PREFIX u8 push(ArrayList *self, void *entity) {
    if (self->pos >= self->cap) {
        self->mem = mem_resize_or_alloc(self->allocator, self->mem, self->cap, self->cap * 2);
        if (self->mem == nullptr) return 1;
    }

    ((char *)self->mem) [self->pos] = *(char *)entity;
    self->pos += 1;

    return 0;
}

/// Fetches the item by index from the ArrayList.
FNDECL_PREFIX void *get(ArrayList *self, usize idx) {
    if (idx > self->pos) return nullptr;

    return (void *)((usize)(((char *)self->mem) [idx]));
}
