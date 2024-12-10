#pragma once

#include "linux/syscall/syscall.hpp"
#include "strings.hpp"
#include <asm/unistd_64.h>

static void assert(bool r, const char *const m) {
    if (!r) {
        write(1, m, strlen(m));
        write(1, "\n", 1);

        exit_group(1);
    }
}
