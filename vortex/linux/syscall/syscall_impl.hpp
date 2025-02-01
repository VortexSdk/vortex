#pragma once

#include "../../metap/diagnostics.hpp"
#include "../../numbers.hpp"
#include "SysRes.hpp"

// #define IS_ARM defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
#if defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
#define IF_ARM(code)                        code;
#define IF_NOT_ARM(code)                    /* Empty */ ;
#define IF_ARM_OR(aarch64_code, other_code) aarch64_code;
#else
#define IF_NOT_ARM(code)                    code;
#define IF_ARM(code)                        /* Empty */ ;
#define IF_ARM_OR(aarch64_code, other_code) other_code;
#endif

DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast")
template <typename... T> static SysRes<usize> syscall(usize n, T... args) {
    constexpr usize n_args = sizeof...(T);
    static_assert(n_args <= 6, "Too much syscall arguments! You can only pass up to 6.");

    const usize args_array [] = {(usize)(args)...};
    IF_ARM(register usize x8 __asm__("x8") = n)
    IF_ARM(register usize x0 __asm__("x0"))
    IF_ARM(register usize x1 __asm__("x1"))
    IF_ARM(register usize x2 __asm__("x2"))
    IF_ARM(register usize x3 __asm__("x3"))
    IF_ARM_OR(register usize x4 __asm__("x4"), register usize r10 __asm__("r10"))
    IF_ARM_OR(register usize x5 __asm__("x5"), register usize r8 __asm__("r8"))
    IF_NOT_ARM(register usize r9 __asm__("r9"))
    IF_ARM({
        if constexpr (n_args > 0) x0 = args_array [0];
        if constexpr (n_args > 1) x1 = args_array [1];
        if constexpr (n_args > 2) x2 = args_array [2];
    })
    if constexpr (n_args > 3) IF_ARM_OR(x3 = args_array [3], r10 = args_array [3])
    if constexpr (n_args > 4) IF_ARM_OR(x4 = args_array [4], r8 = args_array [4])
    if constexpr (n_args > 5) IF_ARM_OR(x5 = args_array [5], r9 = args_array [5])

    IF_NOT_ARM(usize ret)
    if constexpr (n_args == 0) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0" : "=r"(x0) : "r"(x8) : "memory", "cc"),
            __asm__ __volatile__("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory")
        )
    } else if constexpr (n_args == 1) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0" : "=r"(x0) : "r"(x8), "r"(x0) : "memory", "cc"),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 2) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0" : "=r"(x0) : "r"(x8), "r"(x0), "r"(x1) : "memory", "cc"),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]) : "rcx",
                "r11", "memory"
            )
        )
    } else if constexpr (n_args == 3) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(x0) : "r"(x8), "r"(x0), "r"(x1), "r"(x2) : "memory", "cc"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 4) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(x0) : "r"(x8), "r"(x0), "r"(x1), "r"(x2), "r"(x3) : "memory", "cc"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]), "r"(r10) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 5) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(x0) : "r"(x8), "r"(x0), "r"(x1), "r"(x2), "r"(x3),
                "r"(x4) : "memory", "cc"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]), "r"(r10), "r"(r8) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 6) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(x0) : "r"(x8), "r"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4),
                "r"(x5) : "memory", "cc"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory"
            )
        )
    }

    IF_ARM_OR(return SysRes<usize>::from_sys(x0), return SysRes<usize>::from_sys(ret))
}
DIAG_IGNORE_CLANG_POP

#include "wrapper.hpp"
