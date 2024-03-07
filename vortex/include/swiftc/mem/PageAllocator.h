#pragma once

#include "../numbers.h"
#include "../os/linux/linux.h"
#include "../os/page_size.h"
#include "utils.h"

typedef struct PageAllocator {
    void *mem;
    usize len;
} PageAllocator;

/// Allocates n pages.
FNDECL_PREFIX PageAllocator page_init(usize n) {
    usize len = n * PAGE_SIZE;

#ifdef __unix__
    usize res =
        SYSCALL(SYS_mmap, 6, 0, len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);

    if (linux_get_syserrno(res) != SE_SUCCESS) {
        return (PageAllocator){.mem = nullptr, .len = 0};
    }

    return (PageAllocator){.mem = (void *)res, .len = len};
#else
    #error "PageAllocator is not implemented for the target OS!"
#endif
}

/// Deallocates all the pages.
FNDECL_PREFIX void page_deinit(PageAllocator *pa) {
    linux_get_syserrno(SYSCALL(SYS_munmap, 2, (usize)((*pa).mem), (usize)((*pa).len)));
}
