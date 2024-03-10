#pragma once

#include "sys/sys.h"
#include "vortex/mem/utils.h"
#include "vortex/numbers.h"

#ifndef VORTEX_PREFIX
    #define VORTEX_PREFIX extern
#endif

struct __attribute((aligned(16))) new_thread_stack_head {
    void (*entry)(struct new_thread_stack_head *);
    int join_futex;
};

/// Allocates a thread for ring.
VORTEX_PREFIX void *allocate_ring_thread(usize stack_pages) {
    usize clone_res;
    clone_args clonesys_args = (clone_args){
        .stack_size = (u64)stack_pages * PAGE_SIZE,
        .flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_PARENT | CLONE_THREAD |
                 CLONE_IO,
    };

    usize mmap_res = SYSCALL(SYS_mmap, 6, 0, clonesys_args.stack_size, PROT_READ | PROT_WRITE,
                             MAP_ANONYMOUS | MAP_PRIVATE | MAP_GROWSDOWN, 0, 0);
    if (linux_get_syserrno(mmap_res) != SE_SUCCESS) return nullptr;

    clonesys_args.stack = (u64)(mmap_res);
    clone_res = SYSCALL(SYS_clone3, 2, (usize)&clonesys_args, (usize)sizeof(clone_args) - 3 * 8);
    if (linux_get_syserrno(clone_res) != SE_SUCCESS) return nullptr;

    return (void *)clone_res;
}
