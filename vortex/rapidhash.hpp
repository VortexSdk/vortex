#pragma once

#include "diagnostics.hpp"
#include "mem/utils.hpp"
#include "numbers.hpp"

#if defined(_MSC_VER)
#include <intrin.h>
#if defined(_M_X64) && !defined(_M_ARM64EC)
#pragma intrinsic(_umul128)
#endif
#endif

DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast")

#ifdef __cplusplus
#define RAPIDHASH_NOEXCEPT  noexcept
#define RAPIDHASH_CONSTEXPR constexpr
#ifndef RAPIDHASH_INLINE
#define RAPIDHASH_INLINE inline
#endif
#else
#define RAPIDHASH_NOEXCEPT
#define RAPIDHASH_CONSTEXPR static const
#ifndef RAPIDHASH_INLINE
#define RAPIDHASH_INLINE static inline
#endif
#endif

/*
 *  Protection macro, alters behaviour of rapid_mum multiplication function.
 *
 *  RAPIDHASH_FAST: Normal behavior, max speed.
 *  RAPIDHASH_PROTECTED: Extra protection against entropy loss.
 */
#ifndef RAPIDHASH_PROTECTED
#define RAPIDHASH_FAST
#elif defined(RAPIDHASH_FAST)
#error "cannot define RAPIDHASH_PROTECTED and RAPIDHASH_FAST simultaneously."
#endif

/*
 *  Unrolling macros, changes code definition for main hash function.
 *
 *  RAPIDHASH_COMPACT: Legacy variant, each loop process 48 bytes.
 *  RAPIDHASH_UNROLLED: Unrolled variant, each loop process 96 bytes.
 *
 *  Most modern CPUs should benefit from having RAPIDHASH_UNROLLED.
 *
 *  These macros do not alter the output hash.
 */
#ifndef RAPIDHASH_COMPACT
#define RAPIDHASH_UNROLLED
#elif defined(RAPIDHASH_UNROLLED)
#error "cannot define RAPIDHASH_COMPACT and RAPIDHASH_UNROLLED simultaneously."
#endif

#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
#define _likely_(x)   __builtin_expect(x, 1)
#define _unlikely_(x) __builtin_expect(x, 0)
#else
#define _likely_(x)   (x)
#define _unlikely_(x) (x)
#endif

#ifndef RAPIDHASH_LITTLE_ENDIAN
#if defined(_WIN32) || defined(__LITTLE_ENDIAN__) ||                                               \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define RAPIDHASH_LITTLE_ENDIAN
#elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define RAPIDHASH_BIG_ENDIAN
#else
#warning "could not determine endianness! Falling back to little endian."
#define RAPIDHASH_LITTLE_ENDIAN
#endif
#endif

/*
 *  Default seed.
 */
#define RAPID_SEED (0xbdd89aa982704029ull)

/*
 *  Default secret parameters.
 */
RAPIDHASH_CONSTEXPR u64 rapid_secret [3] = {
    0x2d358dccaa6c78a5ull, 0x8bb84b93962eacc9ull, 0x4b33a62ed433d4a3ull
};

/*
 *  64*64 -> 128bit multiply function.
 *
 *  @param A  Address of 64-bit number.
 *  @param B  Address of 64-bit number.
 *
 *  Calculates 128-bit C = *A * *B.
 *
 *  When RAPIDHASH_FAST is defined:
 *  Overwrites A contents with C's low 64 bits.
 *  Overwrites B contents with C's high 64 bits.
 *
 *  When RAPIDHASH_PROTECTED is defined:
 *  Xors and overwrites A contents with C's low 64 bits.
 *  Xors and overwrites B contents with C's high 64 bits.
 */
RAPIDHASH_INLINE void rapid_mum(u64 *A, u64 *B) RAPIDHASH_NOEXCEPT {
#if defined(__SIZEOF_INT128__)
    __uint128_t r = *A;
    r *= *B;
#ifdef RAPIDHASH_PROTECTED
    *A ^= (u64)r;
    *B ^= (u64)(r >> 64);
#else
    *A = (u64)r;
    *B = (u64)(r >> 64);
#endif
#elif defined(_MSC_VER) && (defined(_WIN64) || defined(_M_HYBRID_CHPE_ARM64))
#if defined(_M_X64)
#ifdef RAPIDHASH_PROTECTED
    u64 a, b;
    a = _umul128(*A, *B, &b);
    *A ^= a;
    *B ^= b;
#else
    *A = _umul128(*A, *B, B);
#endif
#else
#ifdef RAPIDHASH_PROTECTED
    u64 a, b;
    b = __umulh(*A, *B);
    a = *A * *B;
    *A ^= a;
    *B ^= b;
#else
    u64 c = __umulh(*A, *B);
    *A    = *A * *B;
    *B    = c;
#endif
#endif
#else
    u64 ha = *A >> 32, hb = *B >> 32, la = (u32)*A, lb = (u32)*B, hi, lo;
    u64 rh = ha * hb, rm0 = ha * lb, rm1 = hb * la, rl = la * lb, t = rl + (rm0 << 32), c = t < rl;
    lo = t + (rm1 << 32);
    c += lo < t;
    hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;
#ifdef RAPIDHASH_PROTECTED
    *A ^= lo;
    *B ^= hi;
#else
    *A = lo;
    *B = hi;
#endif
#endif
}

/*
 *  Multiply and xor mix function.
 *
 *  @param A  64-bit number.
 *  @param B  64-bit number.
 *
 *  Calculates 128-bit C = A * B.
 *  Returns 64-bit xor between high and low 64 bits of C.
 */
RAPIDHASH_INLINE u64 rapid_mix(u64 A, u64 B) RAPIDHASH_NOEXCEPT {
    rapid_mum(&A, &B);
    return A ^ B;
}

/*
 *  Read functions.
 */
#ifdef RAPIDHASH_LITTLE_ENDIAN
RAPIDHASH_INLINE u64 rapid_read64(const u8 *p) RAPIDHASH_NOEXCEPT {
    u64 v;
    memcpy(&v, p, sizeof(u64));
    return v;
}

RAPIDHASH_INLINE u64 rapid_read32(const u8 *p) RAPIDHASH_NOEXCEPT {
    u32 v;
    memcpy(&v, p, sizeof(u32));
    return v;
}
#elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
RAPIDHASH_INLINE u64 rapid_read64(const u8 *p) RAPIDHASH_NOEXCEPT {
    u64 v;
    memcpy(&v, p, sizeof(u64));
    return __builtin_bswap64(v);
}

RAPIDHASH_INLINE u64 rapid_read32(const u8 *p) RAPIDHASH_NOEXCEPT {
    u32 v;
    memcpy(&v, p, sizeof(u32));
    return __builtin_bswap32(v);
}
#elif defined(_MSC_VER)
RAPIDHASH_INLINE u64 rapid_read64(const u8 *p) RAPIDHASH_NOEXCEPT {
    u64 v;
    memcpy(&v, p, sizeof(u64));
    return _byteswap_uint64(v);
}

RAPIDHASH_INLINE u64 rapid_read32(const u8 *p) RAPIDHASH_NOEXCEPT {
    u32 v;
    memcpy(&v, p, sizeof(u32));
    return _byteswap_ulong(v);
}
#else
RAPIDHASH_INLINE u64 rapid_read64(const u8 *p) RAPIDHASH_NOEXCEPT {
    u64 v;
    memcpy(&v, p, 8);
    return (
        ((v >> 56) & 0xff) | ((v >> 40) & 0xff00) | ((v >> 24) & 0xff0000) |
        ((v >> 8) & 0xff000000) | ((v << 8) & 0xff00000000) | ((v << 24) & 0xff0000000000) |
        ((v << 40) & 0xff000000000000) | ((v << 56) & 0xff00000000000000)
    );
}

RAPIDHASH_INLINE u64 rapid_read32(const u8 *p) RAPIDHASH_NOEXCEPT {
    u32 v;
    memcpy(&v, p, 4);
    return (
        ((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | ((v << 24) & 0xff000000)
    );
}
#endif

/*
 *  Reads and combines 3 bytes of input.
 *
 *  @param p  Buffer to read from.
 *  @param k  Length of @p, in bytes.
 *
 *  Always reads and combines 3 bytes from memory.
 *  Guarantees to read each buffer position at least once.
 *
 *  Returns a 64-bit value containing all three bytes read.
 */
RAPIDHASH_INLINE u64 rapid_readSmall(const u8 *p, usize k) RAPIDHASH_NOEXCEPT {
    return (((u64)p [0]) << 56) | (((u64)p [k >> 1]) << 32) | p [k - 1];
}

/*
 *  rapidhash main function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *  @param secret  Triplet of 64-bit secrets used to alter hash result predictably.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE u64 rapidhash_internal(const void *key, usize len, u64 seed, const u64 *secret)
    RAPIDHASH_NOEXCEPT {
    const u8 *p = (const u8 *)key;
    seed ^= rapid_mix(seed ^ secret [0], secret [1]) ^ len;
    u64 a, b;
    if (_likely_(len <= 16)) {
        if (_likely_(len >= 4)) {
            const u8 *plast = p + len - 4;
            a               = (rapid_read32(p) << 32) | rapid_read32(plast);
            const u64 delta = ((len & 24) >> (len >> 3));
            b               = ((rapid_read32(p + delta) << 32) | rapid_read32(plast - delta));
        } else if (_likely_(len > 0)) {
            a = rapid_readSmall(p, len);
            b = 0;
        } else a = b = 0;
    } else {
        usize i = len;
        if (_unlikely_(i > 48)) {
            u64 see1 = seed, see2 = seed;
#ifdef RAPIDHASH_UNROLLED
            while (_likely_(i >= 96)) {
                seed = rapid_mix(rapid_read64(p) ^ secret [0], rapid_read64(p + 8) ^ seed);
                see1 = rapid_mix(rapid_read64(p + 16) ^ secret [1], rapid_read64(p + 24) ^ see1);
                see2 = rapid_mix(rapid_read64(p + 32) ^ secret [2], rapid_read64(p + 40) ^ see2);
                seed = rapid_mix(rapid_read64(p + 48) ^ secret [0], rapid_read64(p + 56) ^ seed);
                see1 = rapid_mix(rapid_read64(p + 64) ^ secret [1], rapid_read64(p + 72) ^ see1);
                see2 = rapid_mix(rapid_read64(p + 80) ^ secret [2], rapid_read64(p + 88) ^ see2);
                p += 96;
                i -= 96;
            }
            if (_unlikely_(i >= 48)) {
                seed = rapid_mix(rapid_read64(p) ^ secret [0], rapid_read64(p + 8) ^ seed);
                see1 = rapid_mix(rapid_read64(p + 16) ^ secret [1], rapid_read64(p + 24) ^ see1);
                see2 = rapid_mix(rapid_read64(p + 32) ^ secret [2], rapid_read64(p + 40) ^ see2);
                p += 48;
                i -= 48;
            }
#else
            do {
                seed = rapid_mix(rapid_read64(p) ^ secret [0], rapid_read64(p + 8) ^ seed);
                see1 = rapid_mix(rapid_read64(p + 16) ^ secret [1], rapid_read64(p + 24) ^ see1);
                see2 = rapid_mix(rapid_read64(p + 32) ^ secret [2], rapid_read64(p + 40) ^ see2);
                p += 48;
                i -= 48;
            } while (_likely_(i >= 48));
#endif
            seed ^= see1 ^ see2;
        }
        if (i > 16) {
            seed = rapid_mix(rapid_read64(p) ^ secret [2], rapid_read64(p + 8) ^ seed ^ secret [1]);
            if (i > 32)
                seed = rapid_mix(rapid_read64(p + 16) ^ secret [2], rapid_read64(p + 24) ^ seed);
        }
        a = rapid_read64(p + i - 16);
        b = rapid_read64(p + i - 8);
    }
    a ^= secret [1];
    b ^= seed;
    rapid_mum(&a, &b);
    return rapid_mix(a ^ secret [0] ^ len, b ^ secret [1]);
}

/*
 *  rapidhash default seeded hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE u64 rapidhash_withSeed(const void *key, usize len, u64 seed) RAPIDHASH_NOEXCEPT {
    return rapidhash_internal(key, len, seed, rapid_secret);
}

/*
 *  rapidhash default hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE u64 rapidhash(const void *key, usize len) RAPIDHASH_NOEXCEPT {
    return rapidhash_withSeed(key, len, RAPID_SEED);
}

static inline u64 wyrand(u64 *seed) {
    *seed += 0x2d358dccaa6c78a5ull;
    return rapid_mix(*seed, *seed ^ 0x8bb84b93962eacc9ull);
}

// modified from https://github.com/going-digital/Prime64
static inline unsigned long long
mul_mod(unsigned long long a, unsigned long long b, unsigned long long m) {
    unsigned long long r = 0;
    while (b) {
        if (b & 1) {
            unsigned long long r2 = r + a;
            if (r2 < r) r2 -= m;
            r = r2 % m;
        }
        b >>= 1;
        if (b) {
            unsigned long long a2 = a + a;
            if (a2 < a) a2 -= m;
            a = a2 % m;
        }
    }
    return r;
}

static inline unsigned long long
pow_mod(unsigned long long a, unsigned long long b, unsigned long long m) {
    unsigned long long r = 1;
    while (b) {
        if (b & 1) r = mul_mod(r, a, m);
        b >>= 1;
        if (b) a = mul_mod(a, a, m);
    }
    return r;
}

static unsigned sprp(unsigned long long n, unsigned long long a) {
    unsigned long long d = n - 1;
    unsigned char s      = 0;
    while (!(d & 0xff)) {
        d >>= 8;
        s += 8;
    }
    if (!(d & 0xf)) {
        d >>= 4;
        s += 4;
    }
    if (!(d & 0x3)) {
        d >>= 2;
        s += 2;
    }
    if (!(d & 0x1)) {
        d >>= 1;
        s += 1;
    }
    unsigned long long b = pow_mod(a, d, n);
    if ((b == 1) || (b == (n - 1))) return 1;
    unsigned char r;
    for (r = 1; r < s; r++) {
        b = mul_mod(b, b, n);
        if (b <= 1) return 0;
        if (b == (n - 1)) return 1;
    }
    return 0;
}

static unsigned is_prime(unsigned long long n) {
    if (n < 2 || !(n & 1)) return 0;
    if (n < 4) return 1;
    if (!sprp(n, 2)) return 0;
    if (n < 2047) return 1;
    if (!sprp(n, 3)) return 0;
    if (!sprp(n, 5)) return 0;
    if (!sprp(n, 7)) return 0;
    if (!sprp(n, 11)) return 0;
    if (!sprp(n, 13)) return 0;
    if (!sprp(n, 17)) return 0;
    if (!sprp(n, 19)) return 0;
    if (!sprp(n, 23)) return 0;
    if (!sprp(n, 29)) return 0;
    if (!sprp(n, 31)) return 0;
    if (!sprp(n, 37)) return 0;
    return 1;
}

static inline void make_secret(u64 seed, u64 *secret) {
    u8 c [] = {15,  23,  27,  29,  30,  39,  43,  45,  46,  51,  53,  54,  57,  58,
               60,  71,  75,  77,  78,  83,  85,  86,  89,  90,  92,  99,  101, 102,
               105, 106, 108, 113, 114, 116, 120, 135, 139, 141, 142, 147, 149, 150,
               153, 154, 156, 163, 165, 166, 169, 170, 172, 177, 178, 180, 184, 195,
               197, 198, 201, 202, 204, 209, 210, 212, 216, 225, 226, 228, 232, 240};
    for (usize i = 0; i < 4; i++) {
        u8 ok;
        do {
            ok         = 1;
            secret [i] = 0;
            for (usize j = 0; j < 64; j += 8)
                secret [i] |= ((u64)c [wyrand(&seed) % sizeof(c)]) << j;
            if (secret [i] % 2 == 0) {
                ok = 0;
                continue;
            }
            for (usize j = 0; j < i; j++) {
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
                if (__builtin_popcountll(secret [j] ^ secret [i]) != 32) {
                    ok = 0;
                    break;
                }
#elif defined(_MSC_VER) && defined(_WIN64)
#if defined(_M_X64)
                if (_mm_popcnt_u64(secret [j] ^ secret [i]) != 32) {
                    ok = 0;
                    break;
                }
#else
                if (_CountOneBits64(secret [j] ^ secret [i]) != 32) {
                    ok = 0;
                    break;
                }
#endif
#else
                // manual popcount
                u64 x = secret [j] ^ secret [i];
                x -= (x >> 1) & 0x5555555555555555;
                x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
                x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
                x = (x * 0x0101010101010101) >> 56;
                if (x != 32) {
                    ok = 0;
                    break;
                }
#endif
            }
            if (ok && !is_prime(secret [i])) ok = 0;
        } while (!ok);
    }
}

DIAG_IGNORE_CLANG_POP
