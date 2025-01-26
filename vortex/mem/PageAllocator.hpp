#pragma once

#include "../linux/syscall/wrapper.hpp"
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
        u8 *r =
            TRY(mmap(NULL, l, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1_usize, 0));

        return PageAllocator(l, r);
    }

    Slice<u8> as_slice(this PageAllocator &self) {
        return Slice<u8>::init(self.len, self.ptr);
    }

    SysRes<None> deinit(this PageAllocator &self) {
        TRY(munmap(reinterpret_cast<void *>(self.ptr), self.len));

        return None();
    }
};
