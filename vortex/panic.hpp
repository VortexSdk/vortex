#pragma once

// This functions should not depend on anything as it may be used anywhere (i.e. syscall.hpp).
static __attribute__((noreturn)) void panic(const char* msg) {
    unsigned long msg_len = 0;
    for (;; msg_len++)
        if (msg [msg_len] == '\0') break;

    const auto msg_addr = reinterpret_cast<unsigned long>(msg);
#if defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
    // Write = 64
    register unsigned long x8 __asm__("x8") = 64;
    register unsigned long x0 __asm__("x0") = 0;
    register unsigned long x1 __asm__("x1") = msg_addr;
    register unsigned long x2 __asm__("x2") = msg_len;
    __asm__ __volatile__("svc #0" : "=r"(x0) : "r"(x8), "r"(x0), "r"(x1), "r"(x2) : "memory");

    // Exit_group = 94
    x8 = 94;
    x0 = 1;
    x1 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    x2 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    __asm__ __volatile__("svc #0" : "=r"(x0) : "r"(x8), "r"(x0) : "memory");
#else
    // Write = 1
    register unsigned long r0 __asm__("rax") = 1;
    register unsigned long r1 __asm__("rdi") = 0;
    register unsigned long r2 __asm__("rsi") = msg_addr;
    register unsigned long r3 __asm__("rdx") = msg_len;
    __asm__ __volatile__("syscall"
                         : "=r"(r0)
                         : "r"(r0), "r"(r1), "r"(r2), "r"(r3)
                         : "rcx", "r11", "memory");

    // Exit_group = 231
    r0 = 231;
    r1 = 1;
    r2 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    r3 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    __asm__ __volatile__("syscall" : "=r"(r0) : "r"(r0), "r"(r1) : "rcx", "r11", "memory");
#endif

    __builtin_unreachable();
}
