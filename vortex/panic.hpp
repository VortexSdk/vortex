#pragma once

// This functions should not depend on anything as it may be used anywhere (i.e. syscall.hpp).
static __attribute__((noreturn)) void panic(const char* msg) {
    unsigned long msg_len = 0;
    for (;; msg_len++)
        if (msg [msg_len] == '\0') break;

    const auto msg_addr = reinterpret_cast<unsigned long>(msg);
#if defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
    // Write = 64
    register unsigned long _r8 __asm__("x8") = 64;
    register unsigned long _r0 __asm__("x0") = 0;
    register unsigned long _r1 __asm__("x1") = msg_addr;
    register unsigned long _r2 __asm__("x2") = msg_len;
    __asm__ __volatile__("svc #0" : "=r"(_r0) : "r"(_r8), "r"(_r0), "r"(_r1), "r"(rx2) : "memory");

    // Exit_group = 94
    _r8 = 94;
    _r0 = 1;
    _r1 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    _r2 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    __asm__ __volatile__("svc #0" : "=r"(_r0) : "r"(_r8), "r"(_r0) : "memory");
#else
    // Write = 1
    register unsigned long _r0 __asm__("rax") = 1;
    register unsigned long _r1 __asm__("rdi") = 0;
    register unsigned long _r2 __asm__("rsi") = msg_addr;
    register unsigned long _r3 __asm__("rdx") = msg_len;
    __asm__ __volatile__("syscall"
                         : "=r"(_r0)
                         : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3)
                         : "rcx", "r11", "memory");

    // Exit_group = 231
    _r0 = 231;
    _r1 = 1;
    _r2 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    _r3 = 0; // NOLINT(clang-analyzer-deadcode.DeadStores)
    __asm__ __volatile__("syscall" : "=r"(_r0) : "r"(_r0), "r"(_r1) : "rcx", "r11", "memory");
#endif

    __builtin_unreachable();
}
