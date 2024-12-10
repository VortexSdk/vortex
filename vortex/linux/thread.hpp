#pragma once

#include "../math.hpp"
#include "../mem/utils.hpp"
#include "../numbers.hpp"
#include "asm-generic/mman.h"
#include "linux/futex.h"
#include "linux/sched.h"
#include "syscall/syscall.hpp"
#include "vortex/metap/structs.hpp"

// Process/thread waiting options
enum : i32 {
    P_ALL   = 0, // Wait for any child
    P_PID   = 1, // Wait for specific PID
    P_PGID  = 2, // Wait for members of process group
    P_PIDFD = 3  // Wait using pidfd
};

// Wait status options
enum : i32 {
    WEXITED    = 0x4,      // Wait for processes that have exited
    WSTOPPED   = 0x2,      // Wait for processes that have stopped
    WCONTINUED = 0x8,      // Wait for processes that have continued
    WNOWAIT    = 0x1000000 // Don't reap process after waiting
};

enum class ThreadState : u8 {
    New = 0,
    Running,
    Finished,
    Error
};

using ThreadFn = u8 (*)(void*);

__attribute__((naked, nonnull(1, 3, 4))) static void _raw_clone3(
    // %rdi
    clone_args* cl_args,
    // %rsi
    usize clone_args_size,
    // %rdx
    ThreadFn fn_ptr,
    // %rcx
    u32* join_futex,
    // %r8
    void* arg = null
) {
    asm volatile(
        // clone3 = 435
        "movq    $0x1B3, %rax\n"
        "syscall             \n"
        "test    %rax,   %rax\n"
        "jnz     1f          \n"

        "sub   $0x10, %rsp\n"
        "xorq  %rbp,  %rbp\n"
        "movq  %r8,   %rdi\n"
        "callq *%rdx      \n"
        "movq  %rax,  %r8 \n"

        "lea   0xC(%rsp), %rdi  \n"
        "movl  $0x1,      %eax  \n"
        "xchgl %eax,      (%rdi)\n"

        // __NR_futex_wake = 454
        "movq    $0x1C6, %rax\n"
        "movq    $0x1,   %rsi\n"
        "movq    $0x1,   %rdx\n"
        "movq    $0x02,  %r10\n"
        "syscall             \n"

        "movq    %r8,   %rdi\n"
        "movq    $0x3C, %rax\n"
        "syscall            \n"

        "1:\n"
        "    ret"
    );
}
__attribute__((naked, nonnull(1, 3))) static void _raw_clone3_nonjoinable(
    // %rdi
    clone_args* cl_args,
    // %rsi
    usize clone_args_size,
    // %rdx
    ThreadFn fn_ptr,
    // %rcx
    void* placeholder = null,
    // %r8
    void* arg         = null
) {
    asm volatile(
        // clone3 = 435
        "movq    $0x1B3, %rax\n"
        "syscall             \n"
        "test    %rax,   %rax\n"
        "jnz     1f          \n"

        "xorq    %rbp,  %rbp\n"
        "movq    %r8,   %rdi\n"
        "callq   *%rdx      \n"
        "movq    %rax,  %rdi\n"
        "movq    $0x3C, %rax\n"
        "syscall            \n"

        "1:\n"
        "    ret"
    );
}

struct Thread {
    PIN_STRUCT(
        Thread, stack_size, 0_usize, join_futex, reinterpret_cast<u32*>(0), stack_mem,
        reinterpret_cast<u8*>(0)
    )

    static SysRes<Thread> init(
        ThreadFn func, void* arg = null,
        usize stack_size = (1_usize << 12_usize) * 1024_usize * 1024_usize,
        u64 clone_flags  = CLONE_SIGHAND | CLONE_VM | CLONE_FILES | CLONE_IO
    ) {
        Thread self;
        clone_args args = zeroed<clone_args>();
        args.flags      = clone_flags;
        args.stack_size = self.stack_size = ceilPowerOfTwo<usize>(stack_size);
        self.stack_mem =
            TRY(mmap(
                    null, args.stack_size, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_GROWSDOWN | MAP_NORESERVE, 0xFFFFFFFFFFFFFFFF,
                    0
                ),
                Thread);
        args.stack      = reinterpret_cast<usize>(self.stack_mem);

        void* stack_top = reinterpret_cast<void*>(args.stack + args.stack_size);
        u32* sp         = (u32*)stack_top;
        u32* join_futex = self.join_futex = (--sp);
        *join_futex                       = 0;

        _raw_clone3(&args, sizeof(clone_args), func, join_futex, arg);

        return move(self);
    }

    SysRes<None> join(this Thread& self) {
        SysRes<usize> res = futex_wait(
            reinterpret_cast<void*>(self.join_futex), 0UL, 1UL, FUTEX2_SIZE_U32, null, 0
        );
        // -EAGAIN = Thread has already finished executing.
        if (res.is_ok() || res.kind == SysResKind::AGAIN) return None();
        else return res.err_or_none();
    }

    void deinit(this Thread& self) {
        munmap(reinterpret_cast<void*>(self.stack_mem), self.stack_size);
    }
};

struct NonJoinableThread {
    PIN_STRUCT(NonJoinableThread, stack_size, 0_usize, stack_mem, reinterpret_cast<u8*>(0))

    static SysRes<NonJoinableThread> init(
        ThreadFn func, void* arg = null, usize stack_size = 1 << 13,
        u64 clone_flags = CLONE_SIGHAND | CLONE_VM | CLONE_FILES | CLONE_IO
    ) {
        NonJoinableThread self;
        clone_args args = zeroed<clone_args>();
        args.flags      = clone_flags;
        args.stack_size = self.stack_size = ceilPowerOfTwo<usize>(stack_size);
        self.stack_mem =
            TRY(mmap(
                    null, args.stack_size, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_GROWSDOWN | MAP_NORESERVE, 0xFFFFFFFFFFFFFFFF,
                    0
                ),
                NonJoinableThread);
        args.stack = reinterpret_cast<usize>(self.stack_mem);

        _raw_clone3_nonjoinable(&args, sizeof(clone_args), func, null, arg);

        return move(self);
    }

    void deinit(this NonJoinableThread& self) {
        munmap(reinterpret_cast<void*>(self.stack_mem), self.stack_size);
    }
};

// Mutex implementation
struct Mutex {
    i32 futex_id{0};

    bool lock(this Mutex& self) {
        while (true) {
            i32 expected = 0;
            if (__atomic_compare_exchange_n(
                    &self.futex_id, &expected, 1, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST
                )) {
                break;
            }

            SysRes<usize> res =
                futex(reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAIT_PRIVATE, 1, null, null, 0);

            if (res.is_err() && res.kind != SysResKind::INTR) {
                return true;
            }
        }
        return false;
    }

    bool try_lock(this Mutex& self) {
        i32 expected = 0;
        return !__atomic_compare_exchange_n(
            &self.futex_id, &expected, 1, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST
        );
    }

    bool unlock(this Mutex& self) {
        if (__atomic_exchange_n(&self.futex_id, 0, __ATOMIC_SEQ_CST) != 1) {
            return true;
        }

        SysRes<usize> res =
            futex(reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAKE_PRIVATE, 1, null, null, 0);

        return res.is_err();
    }
};

// Condition variable implementation
struct CondVar {
    i32 futex_id{0};
    i32 waiters{0};

    bool wait(this CondVar& self, Mutex& mutex) {
        __atomic_add_fetch(&self.waiters, 1, __ATOMIC_SEQ_CST);

        if (mutex.unlock()) {
            __atomic_sub_fetch(&self.waiters, 1, __ATOMIC_SEQ_CST);
            return true;
        }

        while (true) {
            SysRes<usize> res = futex(
                reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAIT_PRIVATE,
                static_cast<u32>(self.futex_id), null, null, 0
            );

            if (res.is_err() && res.kind != SysResKind::INTR) {
                __atomic_sub_fetch(&self.waiters, 1, __ATOMIC_SEQ_CST);
                return true;
            }

            if (!res.is_err()) {
                break;
            }
        }

        __atomic_sub_fetch(&self.waiters, 1, __ATOMIC_SEQ_CST);

        return mutex.lock();
    }

    bool notify_one(this CondVar& self) {
        if (__atomic_load_n(&self.waiters, __ATOMIC_SEQ_CST) < 1) {
            return false;
        }

        __atomic_add_fetch(&self.futex_id, 1, __ATOMIC_SEQ_CST);

        SysRes<usize> res =
            futex(reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAKE_PRIVATE, 1, null, null, 0);

        return res.is_err();
    }

    bool notify_all(this CondVar& self) {
        i32 waiters_a = __atomic_load_n(&self.waiters, __ATOMIC_SEQ_CST);
        if (waiters_a < 1) {
            return false;
        }

        __atomic_add_fetch(&self.futex_id, 1, __ATOMIC_SEQ_CST);

        SysRes<usize> res = futex(
            reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAKE_PRIVATE, static_cast<u32>(waiters_a),
            null, null, 0
        );

        return res.is_err();
    }
};
