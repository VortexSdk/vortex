#pragma once

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

#define __NARG__(...)  __NARG_I_(__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(                                                                                   \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,     \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
    _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, \
    _59, _60, _61, _62, _63, N, ...                                                                \
)                                                                                                  \
    N
#define __RSEQ_N()                                                                                 \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,    \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,    \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define _VFUNC_(name, n)      name##n
#define _VFUNC(name, n)       _VFUNC_(name, n)
#define VFUNC(func, ...)      _VFUNC(func, __NARG__(__VA_ARGS__))(__VA_ARGS__)

#define DIAG_IGNORE(...)      VFUNC(DIAG_IGNORE, __VA_ARGS__)
#define DIAG_IGNORE_PUSH(...) VFUNC(DIAG_IGNORE_PUSH, __VA_ARGS__)

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
