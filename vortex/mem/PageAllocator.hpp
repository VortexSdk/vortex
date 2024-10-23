#pragma once

#include "../linux/syscall/syscall.hpp"
#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "utils.hpp"
#include <asm-generic/mman-common.h>
#include <asm/unistd_64.h>
#include <linux/mman.h>

struct PageAllocator {
    usize len{0};
    u8 *ptr{null<u8>()};

    PageAllocator(usize _l, u8 *_p) : len(_l), ptr(_p) {}
    PageAllocator(const PageAllocator &t)            = delete;
    PageAllocator &operator=(const PageAllocator &t) = delete;
    PageAllocator(PageAllocator &&p) noexcept
        : len(exchange(p.len, USIZE_0)), ptr(exchange(p.ptr, null<u8>())) {}

    static SysRes<PageAllocator> init(usize c) {
        usize l = c * PAGE_SIZE;
        auto r  = mmap(NULL, l, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
        return SysRes<PageAllocator>::from_kind(
            move(PageAllocator(l, reinterpret_cast<u8 *>(r.unsafe_unwrap()))), r.kind
        );
    }

    void deinit(this PageAllocator &self) {
        munmap(reinterpret_cast<void *>(self.ptr), self.len);
    }
};
