#pragma once

#include "mem/utils.hpp"
#include "metap/diagnostics.hpp"
#include "numbers.hpp"

DIAG_IGNORE_PUSH("-Wsign-conversion")

static usize strlen(const char *const s) {
    usize i = 0;
    for (;; i++)
        if (s [i] == '\0') return i;
}

DIAG_IGNORE_POP
