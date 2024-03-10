#pragma once

#include "numbers.h"

/// Aligns the given number backward.
VORTEX_PREFIX usize math_align_backward(usize addr, usize alignment) {
    // TODO: Panic
    // assert(isValidAlignGeneric(T, alignment));

    // 000010000 // example alignment
    // 000001111 // subtract 1
    // 111110000 // binary not
    return addr & ~(alignment - 1);
}

/// Aligns the given number forward.
VORTEX_PREFIX usize math_align_forward(usize addr, usize alignment) {
    // TODO: Panic
    // assert(isValidAlignGeneric(T, alignment));

    return math_align_backward(addr + (alignment - 1), alignment);
}

/// Checks if the given number is a power of two.
VORTEX_PREFIX bool isPowerOfTwo(usize num) {
    return (num != 0) && ((num & (num - 1)) == 0);
}
