/// Musl-like implementations.

#pragma once

#include "../diagnostics.hpp"
#include "../numbers.hpp"

// NOLINTBEGIN
DIAG_IGNORE_CLANG_PUSH(
    "-Wold-style-cast", "-Wcast-align", "-Wimplicit-int-conversion", "-Wsign-conversion",
    "-Wcast-qual", "-Wswitch-default"
)
DIAG_IGNORE_GCC_PUSH("-Wattributes")

// The UB sanitizer flags the `k = -(usize)s & 3;` line as a false positive. To prevent this, we
// need to disable these two specific sanitization checks.
__attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow"))
) static void *
memset(void *dest, int c, usize n) {
    usize k;
    auto *s = (unsigned char *)dest;

    /* Fill head and tail with minimal branching. Each
     * conditional ensures that all the subsequently used
     * offsets are well-defined and in the dest region. */

    if (!n) return dest;
    s [0]     = c;
    s [n - 1] = c;
    if (n <= 2) return dest;
    s [1]     = c;
    s [2]     = c;
    s [n - 2] = c;
    s [n - 3] = c;
    if (n <= 6) return dest;
    s [3]     = c;
    s [n - 4] = c;
    if (n <= 8) return dest;

    /* Advance pointer to align it at a 4-byte boundary,
     * and truncate n to a multiple of 4. The previous code
     * already took care of any head/tail that get cut off
     * by the alignment. */

    k = -(usize)s & 3;
    s += k;
    n -= k;
    n &= -4;

    u32_alias c32             = ((u32_alias)-1) / 255 * (unsigned char)c;

    /* In preparation to copy 32 bytes at a time, aligned on
     * an 8-byte bounary, fill head/tail up to 28 bytes each.
     * As in the initial byte-based head/tail fill, each
     * conditional below ensures that the subsequent offsets
     * are valid (e.g. !(n<=24) implies n>=28). */

    *(u32_alias *)(s + 0)     = c32;
    *(u32_alias *)(s + n - 4) = c32;
    if (n <= 8) return dest;
    *(u32_alias *)(s + 4)      = c32;
    *(u32_alias *)(s + 8)      = c32;
    *(u32_alias *)(s + n - 12) = c32;
    *(u32_alias *)(s + n - 8)  = c32;
    if (n <= 24) return dest;
    *(u32_alias *)(s + 12)     = c32;
    *(u32_alias *)(s + 16)     = c32;
    *(u32_alias *)(s + 20)     = c32;
    *(u32_alias *)(s + 24)     = c32;
    *(u32_alias *)(s + n - 28) = c32;
    *(u32_alias *)(s + n - 24) = c32;
    *(u32_alias *)(s + n - 20) = c32;
    *(u32_alias *)(s + n - 16) = c32;

    /* Align to a multiple of 8 so we can fill 64 bits at a time,
     * and avoid writing the same bytes twice as much as is
     * practical without introducing additional branching. */

    k                          = 24 + ((usize)s & 4);
    s += k;
    n -= k;

    /* If this loop is reached, 28 tail bytes have already been
     * filled, so any remainder when n drops below 32 can be
     * safely ignored. */

    u64 c64 = c32 | ((u64)c32 << 32);
    for (; n >= 32; n -= 32, s += 32) {
        *(u64 *)(s + 0)  = c64;
        *(u64 *)(s + 8)  = c64;
        *(u64 *)(s + 16) = c64;
        *(u64 *)(s + 24) = c64;
    }

    return dest;
}
static void *memcpy(void *__restrict dest, const void *__restrict src, usize n) {
    auto *d       = (unsigned char *)dest;
    const auto *s = (const unsigned char *)src;

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define LS >>
#define RS <<
#else
#define LS <<
#define RS >>
#endif

    u32 w, x;

    for (; (usize)s % 4 && n; n--) *d++ = *s++;

    if ((usize)d % 4 == 0) {
        for (; n >= 16; s += 16, d += 16, n -= 16) {
            *(u32_alias *)(d + 0)  = *(u32_alias *)(s + 0);
            *(u32_alias *)(d + 4)  = *(u32_alias *)(s + 4);
            *(u32_alias *)(d + 8)  = *(u32_alias *)(s + 8);
            *(u32_alias *)(d + 12) = *(u32_alias *)(s + 12);
        }
        if (n & 8) {
            *(u32_alias *)(d + 0) = *(u32_alias *)(s + 0);
            *(u32_alias *)(d + 4) = *(u32_alias *)(s + 4);
            d += 8;
            s += 8;
        }
        if (n & 4) {
            *(u32_alias *)(d + 0) = *(u32_alias *)(s + 0);
            d += 4;
            s += 4;
        }
        if (n & 2) {
            *d++ = *s++;
            *d++ = *s++;
        }
        if (n & 1) {
            *d = *s;
        }
        return dest;
    }

    if (n >= 32) switch ((usize)d % 4) {
        case 1 :
            w    = *(u32_alias *)s;
            *d++ = *s++;
            *d++ = *s++;
            *d++ = *s++;
            n -= 3;
            for (; n >= 17; s += 16, d += 16, n -= 16) {
                x                      = *(u32_alias *)(s + 1);
                *(u32_alias *)(d + 0)  = (w LS 24) | (x RS 8);
                w                      = *(u32_alias *)(s + 5);
                *(u32_alias *)(d + 4)  = (x LS 24) | (w RS 8);
                x                      = *(u32_alias *)(s + 9);
                *(u32_alias *)(d + 8)  = (w LS 24) | (x RS 8);
                w                      = *(u32_alias *)(s + 13);
                *(u32_alias *)(d + 12) = (x LS 24) | (w RS 8);
            }
            break;
        case 2 :
            w    = *(u32_alias *)s;
            *d++ = *s++;
            *d++ = *s++;
            n -= 2;
            for (; n >= 18; s += 16, d += 16, n -= 16) {
                x                      = *(u32_alias *)(s + 2);
                *(u32_alias *)(d + 0)  = (w LS 16) | (x RS 16);
                w                      = *(u32_alias *)(s + 6);
                *(u32_alias *)(d + 4)  = (x LS 16) | (w RS 16);
                x                      = *(u32_alias *)(s + 10);
                *(u32_alias *)(d + 8)  = (w LS 16) | (x RS 16);
                w                      = *(u32_alias *)(s + 14);
                *(u32_alias *)(d + 12) = (x LS 16) | (w RS 16);
            }
            break;
        case 3 :
            w    = *(u32_alias *)s;
            *d++ = *s++;
            n -= 1;
            for (; n >= 19; s += 16, d += 16, n -= 16) {
                x                      = *(u32_alias *)(s + 3);
                *(u32_alias *)(d + 0)  = (w LS 8) | (x RS 24);
                w                      = *(u32_alias *)(s + 7);
                *(u32_alias *)(d + 4)  = (x LS 8) | (w RS 24);
                x                      = *(u32_alias *)(s + 11);
                *(u32_alias *)(d + 8)  = (w LS 8) | (x RS 24);
                w                      = *(u32_alias *)(s + 15);
                *(u32_alias *)(d + 12) = (x LS 8) | (w RS 24);
            }
            break;
        }
    if (n & 16) {
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
    }
    if (n & 8) {
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
    }
    if (n & 4) {
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
    }
    if (n & 2) {
        *d++ = *s++;
        *d++ = *s++;
    }
    if (n & 1) {
        *d = *s;
    }

    return dest;
}
static int memcmp(const void *vl, const void *vr, usize n) {
    const auto *l = (const unsigned char *)(vl);
    const auto *r = (const unsigned char *)(vr);
    for (; n && *l == *r; n--, l++, r++);

    return n ? *l - *r : 0;
}
static int strcmp(const char *l, const char *r) {
    for (; *l == *r && *l; l++, r++);
    return *(unsigned char *)l - *(unsigned char *)r;
}
static int strncmp(const char *_l, const char *_r, usize n) {
    const unsigned char *l = (const unsigned char *)_l, *r = (const unsigned char *)_r;
    if (!n--) return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--);
    return *l - *r;
}

// NOLINTEND
DIAG_IGNORE_POP
