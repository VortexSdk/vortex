#pragma once

#include "linux/syscall/syscall.hpp"
#include "mem/utils.hpp"
#include "numbers.hpp"
#include "strings.hpp"
#include <asm/unistd_64.h>

static void assert(bool r, const char *const m) {
    if (!r) {
        syscall(__NR_write, 0, m, strlen(m));
        syscall(__NR_write, 0, "\n", 1);

        syscall(__NR_exit, 1);
    }
}
