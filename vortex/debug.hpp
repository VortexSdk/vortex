#pragma once

#include "linux/syscall/wrapper.hpp"
#include "writer.hpp"

static void __assert(const char *file, int line, bool r, const char *m = null) {
    if (!r) {
        eprintln("Assertion failed in file `", file, "` at line ", line, ".");
        if (m) eprintln(m);

        exit_group(1);
    }
}

#define assert(...) __assert(__FILE__, __LINE__, __VA_ARGS__)
