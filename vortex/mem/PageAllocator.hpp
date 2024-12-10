#pragma once

#include "../linux/syscall/syscall.hpp"
#include "../metap/metap.hpp"
#include "../metap/structs.hpp"
#include "../numbers.hpp"
#include "utils.hpp"
#include <asm-generic/mman-common.h>
#include <asm/unistd_64.h>
#include <linux/mman.h>

struct PageAllocator {
    PIN_STRUCT(PageAllocator, len, 0_usize, ptr, reinterpret_cast<u8 *>(0))

    static SysRes<PageAllocator> init(usize c) {
        usize l = c * PAGE_SIZE;
        auto r  = mmap(
            NULL, l, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
            18446744073709551615_usize, 0
        );
        return SysRes<PageAllocator>::from_kind(
            move(PageAllocator(l, reinterpret_cast<u8 *>(r.unsafe_unwrap()))), r.kind
        );
    }

    void deinit(this PageAllocator &self) {
        munmap(reinterpret_cast<void *>(self.ptr), self.len);
    }
};
