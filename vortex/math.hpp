#pragma once

#include "metap/metap.hpp"
#include "numbers.hpp"

// Aligns the given number backward.
template <typename T> T alignBackward(T addr, T alignment) {
    // TODO: Panic if alignment is not valid
    // assert(isValidAlignGeneric(T, alignment));

    return addr & ~(alignment - 1);
}

// Aligns the given number forward.
template <typename T> T alignForward(T addr, T alignment) {
    // TODO: Panic if alignment is not valid
    // assert(isValidAlignGeneric(T, alignment));

    return alignBackward(addr + (alignment - 1), alignment);
}

// Checks if the given number is a power of two.
template <typename T> bool isPowerOfTwo(T num) {
    static_assert(is_integral<T>::value, "Type must be integral.");

    return (num != 0) && ((num & (num - 1)) == 0);
}

// Rounds to the next power of two.
template <typename T> constexpr T ceilPowerOfTwo(T n) {
    static_assert(is_integral<T>::value, "Type must be integral.");

    if (n <= 0) return 1;

    --n;

    for (usize i = 1; i < sizeof(T) * 8; i *= 2) n |= n >> i;

    return n + 1;
}
