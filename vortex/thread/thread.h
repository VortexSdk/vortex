#pragma once

#include <linux/futex.h>
#include <linux/mman.h>
#include <linux/sched.h>
#include <sys/sys.h>
#include <vortex/mem/utils.h>
#include <vortex/numbers.h>

typedef struct __attribute((aligned(16))) thread_stack {
    void (*entry)(struct thread_stack *stack_data);
    usize join_futex;
} thread_stack;

void thread_entry(thread_stack *stack_data) {
    SYSCALL(SYS_write, 3, 0, (usize) "Hello World!\n", 14);
    SYSCALL(SYS_exit, 1, 0);
}

/// Allocates a thread for ring.
VORTEX_PREFIX void *thread_create(usize stack_pages) {
    usize clone_res;
    struct clone_args clonesys_args = (struct clone_args){
        .stack_size = (u64)stack_pages * PAGE_SIZE,
        .flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_PARENT | CLONE_THREAD |
                 CLONE_IO,
    };

    usize mmap_res = SYSCALL(SYS_mmap, 6, 0, clonesys_args.stack_size, PROT_READ | PROT_WRITE,
                             MAP_ANONYMOUS | MAP_PRIVATE | MAP_GROWSDOWN, 0, 0);
    if (linux_get_syserrno(mmap_res) != SE_SUCCESS) return nullptr;
    clonesys_args.stack = (u64)(mmap_res);

    clone_res =
        SYSCALL(SYS_clone3, 2, (usize)&clonesys_args, (usize)sizeof(struct clone_args) - 3 * 8);
    if (linux_get_syserrno(clone_res) != SE_SUCCESS) return nullptr;

    return (void *)clone_res;
}
