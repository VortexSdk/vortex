#pragma once

#ifndef NO_START

#include <vortex/vortex.hpp>

DIAG_IGNORE_GCC_PUSH("-Wundef", "-Wunused-variable")
DIAG_IGNORE_CLANG_PUSH(
    "-Wundef", "-Wunused-variable", "-Wundefined-internal", "-Wextern-initializer",
    "-Wmissing-prototypes", "-Wreserved-identifier", "-Wmissing-variable-declarations",
    "-Wunknown-attributes"
)

#if defined(__clang__)
extern "C" void *__dso_handle = reinterpret_cast<void *>(0);
#endif

#ifdef USE_AUX
static u8
main(vortex::Slice<char *>, vortex::Slice<char *>, void *); // NOLINT(clang-diagnostic-undefined-internal)
#elif NO_ENV
static
) u8 main(vortex::Slice<char *>); // NOLINT(clang-diagnostic-undefined-internal)
#elif NO_ARGS
static u8 main(); // NOLINT(clang-diagnostic-undefined-internal)
#else
static u8
    main(vortex::Slice<char *>, vortex::Slice<char *>); // NOLINT(clang-diagnostic-undefined-internal)
#endif

extern "C" __attribute__((noreturn, noinline, noipa)) void start_c(void *s) {
    usize vars_len        = 0;
    const usize args_len  = *reinterpret_cast<usize *>(s);

    auto args_addr        = reinterpret_cast<usize>(s) + 8;
    const usize vars_addr = args_addr + ((args_len + 1) * 8);

    auto *args            = reinterpret_cast<char **>(args_addr);
    auto *vars            = reinterpret_cast<char **>(vars_addr);

    while (vars [vars_len] != nullptr) vars_len++;

    // NOLINTBEGIN(clang-analyzer-deadcode.DeadStores)
    auto *auxv = reinterpret_cast<void *>(vars_addr + 8 + (vars_len * 8));
    // NOLINTEND(clang-analyzer-deadcode.DeadStores)

#ifdef USE_AUX
    u8 const ret =
        main(vortex::Slice<char *>(args_len, args), vortex::Slice<char *>(vars_len, vars)) auxv);
#elif NO_ENV
    u8 const ret = main(vortex::Slice<char *>(args_len, args));
#elif NO_ARGS
    u8 const ret = main();
#else
    u8 const ret =
        main(vortex::Slice<char *>(args_len, args), vortex::Slice<char *>(vars_len, vars));
#endif

    vortex::syscall(__NR_exit, static_cast<usize>(ret));

    __builtin_unreachable();
}

DIAG_IGNORE_POP

IF_ARM_OR(
    __asm__(".text \n"
            ".global _start\n"
            ".type _start,%function\n_start:\n"
            "   mov x29, #0\n"
            "   mov x30, #0\n"
            "   mov x0, sp\n"
            ".weak _DYNAMIC\n"
            ".hidden _DYNAMIC\n"
            "   adrp x1, _DYNAMIC\n"
            "   add x1, x1, #:lo12:_DYNAMIC\n"
            "   and sp, x0, #-16\n"
            "   b start_c\n"),
    __asm__(".text \n"
            ".global _start \n"
            "_start: \n"
            "   xor %rbp,%rbp \n"
            "   mov %rsp,%rdi \n"
            ".weak _DYNAMIC \n"
            ".hidden _DYNAMIC \n"
            "   lea _DYNAMIC(%rip),%rsi \n"
            "   andq $-16,%rsp \n"
            "   call start_c \n")
);

#endif
