#pragma once

#include "../mem/utils.hpp"
#include "../numbers.hpp"
#include "../strings.hpp"
#include "asm-generic/mman.h"
#include "linux/futex.h"
#include "linux/sched.h"
#include "syscall/syscall.hpp"

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

/// Thread context structure
struct ThreadContext {
    ThreadFn func;
    void* arg;
    int futex_id; // For synchronization
};

/// Thread entry point wrapper - must be static to avoid thiscall ABI
static int thread_entry(void* arg) {
    auto* ctx      = reinterpret_cast<ThreadContext*>(arg);
    const auto ret = static_cast<int>(ctx->func(ctx->arg));

    // Signal completion using futex
    __atomic_store_n(&ctx->futex_id, 1, __ATOMIC_SEQ_CST);
    futex_wake(reinterpret_cast<u32*>(&ctx->futex_id), 0, 1, 0);

    exit(ret); // Use exit() not exit_group() for thread termination
    return ret;
}

__attribute__((naked, nonnull(1, 3, 4))) static void _raw_clone3(
    // %rdi
    clone_args* cl_args,
    // %rsi will be used for clone3 syscall. Reserved for optimization purposes.
    usize,
    // %rdx
    ThreadFn fn_ptr,
    // %rcx
    int* futex_id,
    // %r8
    void* arg
) {
#define __MOV_CLONEARGS_SIZE_HELPER(s1, s2, s3) s1 #s2 s3
    asm volatile(
        // __NR_clone3 = 435
        "movq   $435, %rax\n"
        // Dynamically uses the sizeof clone_args
        __MOV_CLONEARGS_SIZE_HELPER("movq $", sizeof(clone_args), ", %rsi\n")
        // movq $(sizoeof(clone_args)), %rax

        "syscall          \n"
        "test   %rax, %rax\n"
        "jnz    1f        \n"

        "xorq   %rbp, %rbp\n"
        "movq   %r8,  %rdi\n"
        "callq  *%rdx     \n"
        "push   %rax      \n"
        "movq   %rax, %r9 \n"

        "movl   $1, %eax\n"
        "xchgl  (%rcx), %eax\n"

        // __NR_futex_wake = 454
        "movq   $454, %rax\n"
        "movq   %rcx, %rdi\n"
        "movq   $0,   %rsi\n"
        "movq   $1,   %rdx\n"
        "movq   $0,   %r10\n"
        "syscall          \n"

        "movq   %r9, %rdi\n"
        "movq   $60, %rax\n"
        "syscall         \n"

        "1:\n"
        "    ret"
    );
}

struct Thread {
    pid_t tid{-1};
    ThreadContext* ctx{null<ThreadContext>()};
    u8* stack_base{null<u8>()}; // For cleanup
    usize stack_size{0};

    Thread() : tid(-1), ctx(null<ThreadContext>()), stack_base(null<u8>()), stack_size(0) {}
    Thread(const Thread& t)            = delete;
    Thread& operator=(const Thread& t) = delete;
    Thread(Thread&& t) noexcept
        : tid(exchange(t.tid, -1)), ctx(exchange(t.ctx, null<ThreadContext>())),
          stack_base(exchange(t.stack_base, null<u8>())),
          stack_size(exchange(t.stack_size, 0_usize)) {}

    static SysRes<Thread> init(ThreadFn func, void* arg = NULL, usize stack_size = 1 << 16) {
        Thread t;
        t.stack_size = stack_size;

        // Allocate stack with proper alignment
        t.stack_base =
            TRY(mmap(
                    null<void>(), stack_size, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK | MAP_GROWSDOWN,
                    18446744073709551615_usize, 0
                ),
                Thread);

        void* stack_top           = t.stack_base + stack_size;
        unsigned long* sp         = (unsigned long*)stack_top;
        *(--sp)                   = 0;
        *(--sp)                   = (unsigned long)thread_func;

        // Setup thread context at bottom of stack
        t.ctx                     = reinterpret_cast<ThreadContext*>(t.stack_base);
        t.ctx->func               = func;
        t.ctx->arg                = arg;
        t.ctx->futex_id           = 0;

        // Setup clone_args structure
        struct clone_args cl_args = zeroed<clone_args>();
        cl_args.flags             = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND;
        cl_args.stack             = reinterpret_cast<u64>(t.stack_base);
        cl_args.stack_size        = stack_size;

        // Create thread using clone3
        auto res                  = clone3(&cl_args, sizeof(cl_args));
        if (res.is_err()) {
            munmap(t.stack_base, t.stack_size);
            return res.return_err<Thread>();
        }
        auto id = res.unwrap();

        // In child thread
        if (id == 0) {
            thread_entry(t.ctx);
            __builtin_unreachable();
        }

        // In parent thread
        t.tid = static_cast<pid_t>(id);
        return SysRes<Thread>::from_successful(move(t));
    }

    SysRes<None> join(this Thread& self) {
        if (self.tid == -1) {
            return SysRes<None>::from_err(SysResKind::INVAL);
        }

        // Wait for thread completion using futex
        futex_wait(reinterpret_cast<void*>(&self.ctx->futex_id), 0, 0, 0, null<timespec>(), -1);
        return SysRes<None>::from_successful(None{});
    }

    void deinit(this Thread& self) {
        if (self.stack_base != null<u8>()) {
            munmap(self.stack_base, self.stack_size);
            self.stack_base = null<u8>();
        }
    }
};

// Helper to create and run a thread
template <typename T>
static SysRes<Thread> spawn_thread(T&& func, void* arg = null<void>(), usize stack_size = 1 << 16) {
    return Thread::init(reinterpret_cast<ThreadFn>(func), arg, stack_size);
}
// Mutex implementation
struct Mutex {
    i32 futex_id{0};

    Mutex()                        = default;
    Mutex(const Mutex&)            = delete;
    Mutex& operator=(const Mutex&) = delete;

    bool lock(this Mutex& self) {
        while (true) {
            i32 expected = 0;
            if (__atomic_compare_exchange_n(
                    &self.futex_id, &expected, 1, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST
                )) {
                break;
            }

            auto res = futex(
                reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAIT_PRIVATE, 1,
                null<__kernel_timespec>(), null<u32>(), 0
            );

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

        auto res = futex(
            reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAKE_PRIVATE, 1,
            null<__kernel_timespec>(), null<u32>(), 0
        );

        return res.is_err();
    }
};

// Condition variable implementation
struct CondVar {
    i32 futex_id{0};
    i32 waiters{0};

    CondVar()                          = default;
    CondVar(const CondVar&)            = delete;
    CondVar& operator=(const CondVar&) = delete;

    bool wait(this CondVar& self, Mutex& mutex) {
        __atomic_add_fetch(&self.waiters, 1, __ATOMIC_SEQ_CST);

        if (mutex.unlock()) {
            __atomic_sub_fetch(&self.waiters, 1, __ATOMIC_SEQ_CST);
            return true;
        }

        while (true) {
            auto res = futex(
                reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAIT_PRIVATE,
                static_cast<u32>(self.futex_id), null<__kernel_timespec>(), null<u32>(), 0
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

        auto res = futex(
            reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAKE_PRIVATE, 1,
            null<__kernel_timespec>(), null<u32>(), 0
        );

        return res.is_err();
    }

    bool notify_all(this CondVar& self) {
        i32 waiters_a = __atomic_load_n(&self.waiters, __ATOMIC_SEQ_CST);
        if (waiters_a < 1) {
            return false;
        }

        __atomic_add_fetch(&self.futex_id, 1, __ATOMIC_SEQ_CST);

        auto res = futex(
            reinterpret_cast<u32*>(&self.futex_id), FUTEX_WAKE_PRIVATE, static_cast<u32>(waiters_a),
            null<__kernel_timespec>(), null<u32>(), 0
        );

        return res.is_err();
    }
};
