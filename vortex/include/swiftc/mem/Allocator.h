#pragma once

#include "../math.h"
#include "../numbers.h"
#include "Slice.h"
#include "utils.h"

typedef void *(*AllocatorReallocFn)(void *ctx, void *buf, usize len, usize new_len);

/// This is an allocator interface which relies on storing data with 8-byte
/// alignment. That means types that aren't aligned are stored with padding.
typedef struct Allocator {
    void *ptr;
    /// buf: ptr and len: non-zero and new_len: zero => free.
    /// buf: null and len: non-zero and new_len: zero => alloc.
    /// buf: ptr and len: non-zero and new_len: non-zero => realloc.
    AllocatorReallocFn realloc;
} Allocator;

/// Aligns forward all types to make everything align on 8 bytes.
FNDECL_PREFIX usize mem_align_len(usize len) {
    return math_align_forward(len, 8);
}

/// Initializes an Allocator interface.
FNDECL_PREFIX Allocator allocator_init(void *ptr, AllocatorReallocFn realloc) {
    Allocator allocator;
    allocator.ptr     = ptr;
    allocator.realloc = realloc;
    return allocator;
}

/// Allocates memory. Types that aren't aligned on 8 bytes will be stored with
/// padding.
FNDECL_PREFIX void *mem_alloc(Allocator self, usize len) {
    return self.realloc(self.ptr, nullptr, mem_align_len(len), 0);
}
/// Allocates a slice. Types that aren't aligned on 8 bytes will be stored with
/// padding.
FNDECL_PREFIX Slice mem_alloc_slice(Allocator self, usize len) {
    return (Slice){.ptr = mem_alloc(self, len), .len = len};
}

/// Resizes in place.
FNDECL_PREFIX void *mem_resize(Allocator self, void *buf, usize len, usize new_len) {
    return self.realloc(self.ptr, buf, mem_align_len(len), mem_align_len(new_len));
}
/// Resizes the slice in place.
FNDECL_PREFIX Slice mem_resize_slice(Allocator self, Slice slice, usize new_len) {
    return (Slice){.ptr = mem_resize(self, slice.ptr, slice.len, new_len), .len = new_len};
}

/// Frees the memory.
FNDECL_PREFIX void mem_free(Allocator self, void *buf, usize len) {
    self.realloc(self.ptr, buf, mem_align_len(len), 0);
}
/// Frees the slice.
FNDECL_PREFIX void mem_free_slice(Allocator self, Slice slice) {
    mem_free(self, slice.ptr, slice.len);
}

/// Resizes in place or allocates a new one if failed.
FNDECL_PREFIX void *mem_resize_or_alloc(Allocator self, void *buf, usize len, usize new_len) {
    void *resized = mem_resize(self, buf, len, new_len);
    if (resized != nullptr) return resized;

    void *allocated = mem_alloc(self, new_len);
    if (allocated == nullptr) return nullptr;

    mem_copy(allocated, buf, new_len);
    mem_free(self, buf, len);
    return allocated;
}
/// Resizes the slice in place or allocates a new one if failed.
FNDECL_PREFIX Slice mem_resize_or_alloc_slice(Allocator self, Slice slice, usize new_len) {
    return (Slice){.ptr = mem_resize_or_alloc(self, slice.ptr, slice.len, new_len), .len = new_len};
}

/// Duplicates the memory.
FNDECL_PREFIX void *mem_dupe(Allocator self, void *buf, usize len) {
    void *allocated = mem_alloc(self, mem_align_len(len));
    mem_copy(allocated, buf, len);
    return allocated;
}
/// Duplicates the slcie.
FNDECL_PREFIX Slice mem_dupe_slice(Allocator self, Slice slice) {
    return (Slice){.ptr = mem_dupe(self, slice.ptr, slice.len), .len = slice.len};
}
