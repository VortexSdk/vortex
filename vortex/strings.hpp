#pragma once

#include "numbers.hpp"

static usize strlen(const char *const s) {
    usize i = 0;
    for (;; i++)
        if (s [i] == '\0') return i;
}
