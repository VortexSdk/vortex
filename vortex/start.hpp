#pragma once

#include "asm/unistd_64.h"
#include "numbers.hpp"
#include "vortex.hpp"
#include "vortex/linux/syscall.hpp"

static auto main() -> u8; // NOLINT(clang-diagnostic-undefined-internal)

static usize stack = 0;
__attribute((noreturn)) void inline start() {
    u8 const ret = main();

    vortex::syscall(__NR_exit, static_cast<usize>(ret));

    __builtin_unreachable();
}

__attribute((noreturn)) extern "C" void _start() { // NOLINT(misc-definitions-in-headers)
    IF_ARM_OR(
        __asm__ __volatile__("mov x29, xzr\n"
                             "mov x0, sp\n"
                             "and sp, sp, #-16\n"
                             "blr x1\n"
                             : [stack] "=r"(stack)
                             : [start] "r"(reinterpret_cast<void *>(start))
                             : "x0", "x1", "x29"),
        __asm__ __volatile__("xorl %%ebp, %%ebp\n"
                             "movq %%rsp, %[stack]\n"
                             "andq $-16, %%rsp\n"
                             "call *%[start]\n"
                             : [stack] "=m"(stack)
                             : [start] "r"(reinterpret_cast<void *>(start))
                             : "rbp", "rsp")
    )

    __builtin_unreachable();
}
