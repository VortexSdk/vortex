#pragma once

#include "macros.hpp"

#define __PRAGMA(x) _Pragma(#x)

#if defined(__clang__)
#define DIAG_IGNORE_POP    _Pragma("clang diagnostic pop")
#define __DIAG_IGNORE(x)   __PRAGMA(clang diagnostic ignored x)
#define __DIAG_IGNORE_PUSH _Pragma("clang diagnostic push")
#elif defined(__GNUC__)
#define DIAG_IGNORE_POP    _Pragma("GCC diagnostic pop")
#define __DIAG_IGNORE(x)   __PRAGMA(GCC diagnostic ignored x)
#define __DIAG_IGNORE_PUSH _Pragma("GCC diagnostic push")
#else
#define DIAG_IGNORE_POP    // Unsupported compiler
#define __DIAG_IGNORE(...) // Unsupported compiler
#define __DIAG_IGNORE_PUSH // Unsupported compiler
#endif

// clang-format off
#define DIAG_IGNORE2(c, a1)                     \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    c                                                                                              \
    DIAG_IGNORE_POP
#define DIAG_IGNORE3(c, a1, a2)                 \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    c                                                                                              \
    DIAG_IGNORE_POP
#define DIAG_IGNORE4(c, a1, a2, a3)             \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    c                                                                                              \
    DIAG_IGNORE_POP
#define DIAG_IGNORE5(c, a1, a2, a3, a4)         \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    c                                                                                              \
    DIAG_IGNORE_POP
#define DIAG_IGNORE6(c, a1, a2, a3, a4, a5)     \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)                                                                              \
    c                                                                                              \
    DIAG_IGNORE_POP
// clang-format on

#define DIAG_IGNORE_PUSH1(a1)                                                                      \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)
#define DIAG_IGNORE_PUSH2(a1, a2)                                                                  \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)
#define DIAG_IGNORE_PUSH3(a1, a2, a3)                                                              \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)
#define DIAG_IGNORE_PUSH4(a1, a2, a3, a4)                                                          \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)
#define DIAG_IGNORE_PUSH5(a1, a2, a3, a4, a5)                                                      \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)
#define DIAG_IGNORE_PUSH6(a1, a2, a3, a4, a5, a6)                                                  \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)                                                                              \
    __DIAG_IGNORE(a6)
#define DIAG_IGNORE_PUSH7(a1, a2, a3, a4, a5, a6, a7)                                              \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)                                                                              \
    __DIAG_IGNORE(a6)                                                                              \
    __DIAG_IGNORE(a7)
#define DIAG_IGNORE_PUSH8(a1, a2, a3, a4, a5, a6, a7, a8)                                          \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)                                                                              \
    __DIAG_IGNORE(a6)                                                                              \
    __DIAG_IGNORE(a7)                                                                              \
    __DIAG_IGNORE(a8)
#define DIAG_IGNORE_PUSH9(a1, a2, a3, a4, a5, a6, a7, a8, a9)                                      \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)                                                                              \
    __DIAG_IGNORE(a6)                                                                              \
    __DIAG_IGNORE(a7)                                                                              \
    __DIAG_IGNORE(a8)                                                                              \
    __DIAG_IGNORE(a9)
#define DIAG_IGNORE_PUSH10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)                                \
    __DIAG_IGNORE_PUSH                                                                             \
    __DIAG_IGNORE(a1)                                                                              \
    __DIAG_IGNORE(a2)                                                                              \
    __DIAG_IGNORE(a3)                                                                              \
    __DIAG_IGNORE(a4)                                                                              \
    __DIAG_IGNORE(a5)                                                                              \
    __DIAG_IGNORE(a6)                                                                              \
    __DIAG_IGNORE(a7)                                                                              \
    __DIAG_IGNORE(a8)                                                                              \
    __DIAG_IGNORE(a9)                                                                              \
    __DIAG_IGNORE(a10)

#define DIAG_IGNORE(...)      _VFUNC(DIAG_IGNORE, __VA_ARGS__)
#define DIAG_IGNORE_PUSH(...) _VFUNC(DIAG_IGNORE_PUSH, __VA_ARGS__)

#if defined(__clang__)
#define DIAG_IGNORE_GCC(c, ...)     c
#define DIAG_IGNORE_GCC_POP         // Empty
#define DIAG_IGNORE_GCC_PUSH(...)   // Empty
#define DIAG_IGNORE_CLANG(...)      DIAG_IGNORE(__VA_ARGS__)
#define DIAG_IGNORE_CLANG_POP       DIAG_IGNORE_POP
#define DIAG_IGNORE_CLANG_PUSH(...) DIAG_IGNORE_PUSH(__VA_ARGS__)
#elif defined(__GNUC__)
#define DIAG_IGNORE_CLANG(c, ...)   c
#define DIAG_IGNORE_CLANG_POP       // Empty
#define DIAG_IGNORE_CLANG_PUSH(...) // Empty
#define DIAG_IGNORE_GCC(...)        DIAG_IGNORE(__VA_ARGS__)
#define DIAG_IGNORE_GCC_POP         DIAG_IGNORE_POP
#define DIAG_IGNORE_GCC_PUSH(...)   DIAG_IGNORE_PUSH(__VA_ARGS__)
#endif
