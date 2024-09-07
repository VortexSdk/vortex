#pragma once

#include "../numbers.hpp"

#define IS_ARM defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
#if defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
    #define IF_ARM_OR(aarch64_code, other_code)     aarch64_code;
    #define IF_ARM_OR_EXP(aarch64_code, other_code) (aarch64_code)
#else
    #define IF_ARM_OR(aarch64_code, other_code)     other_code;
    #define IF_ARM_OR_EXP(aarch64_code, other_code) (other_code)
#endif

template <class... T> static auto syscall(usize type, T... args) -> usize {
    constexpr usize n = sizeof...(T);
    static_assert(n <= 6, "Too much syscall arguments! You can only pass up to 6.");

    IF_ARM_OR(register usize _r0 __asm__("x0") = type, register usize _r0 __asm__("rax") = type)
    IF_ARM_OR(register usize _r1 __asm__("x1") = type, register usize _r1 __asm__("rdi") = type)
    IF_ARM_OR(register usize _r2 __asm__("x2") = type, register usize _r2 __asm__("rsi") = type)
    IF_ARM_OR(register usize _r3 __asm__("x3") = type, register usize _r3 __asm__("rdx") = type)
    IF_ARM_OR(register usize _r4 __asm__("x4") = type, register usize _r4 __asm__("r10") = type)
    IF_ARM_OR(register usize _r5 __asm__("x5") = type, register usize _r5 __asm__("r8") = type)
    IF_ARM_OR(register usize _r6 __asm__("x8") = type, register usize _r6 __asm__("r9") = type)

    auto set_registers = [&](auto... syscall_args) {
        usize args_array [] = {static_cast<usize>(syscall_args)...};

        if constexpr (n > 0) _r1 = args_array [0];
        if constexpr (n > 1) _r2 = args_array [1];
        if constexpr (n > 2) _r3 = args_array [2];
        if constexpr (n > 3) _r4 = args_array [3];
        if constexpr (n > 4) _r5 = args_array [4];
        if constexpr (n > 5) _r6 = args_array [5];
    };
    set_registers(args...);

    if constexpr (n == 0) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0)
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0)
                                 : "rcx", "r11", "memory")
        )
    } else if constexpr (n == 1) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1)
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1)
                                 : "rcx", "r11", "memory")
        )
    } else if constexpr (n == 2) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2)
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2)
                                 : "rcx", "r11", "memory")
        )
    } else if constexpr (n == 3) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3)
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3)
                                 : "rcx", "r11", "memory")
        )
    } else if constexpr (n == 4) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(_r4)
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(_r4)
                                 : "rcx", "r11", "memory")
        )
    } else if constexpr (n == 5) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(_r4), "r"(_r5),
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(_r4), "r"(_r5)
                                 : "rcx", "r11", "memory")
        )
    } else if constexpr (n == 6) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(_r4), "r"(_r5),
                                   "r"(_r6)
                                 : "memory"),
            __asm__ __volatile__("syscall"
                                 : "=r"(_r0)
                                 : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(_r4), "r"(_r5),
                                   "r"(_r6)
                                 : "rcx", "r11", "memory")
        )
    }

    return _r0;
}
