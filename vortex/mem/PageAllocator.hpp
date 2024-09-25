#pragma once

#include "../linux/syscall.hpp"
#include "../numbers.hpp"
#include "utils.hpp"
#include <asm-generic/mman-common.h>
#include <asm/unistd_64.h>
#include <linux/mman.h>

struct PageAllocator {
    PageAllocator()  = delete;
    ~PageAllocator() = default;

    void *ptr;
    usize len;

    PageAllocator(usize c) : ptr(nullptr), len(c * PAGE_SIZE) {}

    SysRes init(this PageAllocator &self) {
        SysRes r = syscall(
            __NR_mmap, 0, self.len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0
        );

        if (r.is_ok()) self.ptr = reinterpret_cast<void *>(r.res);

        return r;
    }

    void deinit(this PageAllocator &self) {
        syscall(__NR_munmap, self.ptr, self.len);
    }
};
