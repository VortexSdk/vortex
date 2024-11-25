#pragma once

#ifdef __MINGW32__
#define WRDSIZE 32
#else
#if defined __x86_64__ && !defined __ILP32__
#define WRDSIZE 64
#else
#define WRDSIZE 32
#endif
#endif

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __PDP_ENDIAN    3412

#ifndef __BYTE_ORDER
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

static_assert(sizeof(signed char) == 1, "`signed char` should be 1 byte.");
using i8 = signed char;
static_assert(sizeof(unsigned char) == 1, "`unsigned char` should be 1 byte.");
using u8 = unsigned char;
static_assert(sizeof(signed short int) == 2, "`signed short int` should be 2 bytes.");
using i16 = short;
static_assert(sizeof(unsigned short int) == 2, "`unsigned short int` should be 2 bytes.");
using u16 = unsigned short;
static_assert(sizeof(signed int) == 4, "`signed int` should be 4 bytes.");
using i32 = int;
static_assert(sizeof(unsigned int) == 4, "`unsigned int` should be 4 bytes.");
using u32 = unsigned int;

#if WRDSIZE == 64
static_assert(sizeof(signed long int) == 8, "`signed long int` should be 8 bytes.");
using i64 = long;
static_assert(sizeof(unsigned long int) == 8, "`unsigned long int` should be 8 bytes.");
using u64 = unsigned long;
#else
static_assert(sizeof(signed long long int) == 8, "`signed long long int` should be 8 bytes.");
typedef signed long long int i64;
static_assert(sizeof(unsigned long long int) == 8, "`unsigned long long int` should be 8 bytes.");
typedef unsigned long long int u64;
#endif

using isize       = i64;
using usize       = u64;

using u8_alias    = __attribute__((__may_alias__)) u8;
using i8_alias    = __attribute__((__may_alias__)) i8;
using u16_alias   = __attribute__((__may_alias__)) u16;
using i16_alias   = __attribute__((__may_alias__)) i16;
using u32_alias   = __attribute__((__may_alias__)) u32;
using i32_alias   = __attribute__((__may_alias__)) i32;
using u64_alias   = __attribute__((__may_alias__)) u64;
using i64_alias   = __attribute__((__may_alias__)) i64;
using usize_alias = __attribute__((__may_alias__)) usize;
using isize_alias = __attribute__((__may_alias__)) isize;

constexpr u8 operator""_u8(unsigned long long n) {
    return static_cast<u8>(n);
}
constexpr i8 operator""_i8(unsigned long long n) {
    return static_cast<i8>(n);
}
constexpr u16 operator""_u16(unsigned long long n) {
    return static_cast<u16>(n);
}
constexpr i16 operator""_i16(unsigned long long n) {
    return static_cast<i16>(n);
}
constexpr u32 operator""_u32(unsigned long long n) {
    return static_cast<u32>(n);
}
constexpr i32 operator""_i32(unsigned long long n) {
    return static_cast<i32>(n);
}
constexpr u64 operator""_u64(unsigned long long n) {
    return static_cast<u64>(n);
}
constexpr i64 operator""_i64(unsigned long long n) {
    return static_cast<i64>(n);
}
constexpr usize operator""_usize(unsigned long long n) {
    return static_cast<usize>(n);
}
constexpr isize operator""_isize(unsigned long long n) {
    return static_cast<isize>(n);
}

#define I8_MIN    (-128_i8)
#define I8_MAX    (127_i8)
#define U8_MAX    (255_u8)
#define I16_MIN   (-32768_i16)
#define I16_MAX   (32767_i16)
#define U16_MAX   (65535_u16)
#define I32_MIN   (-2147483648_i32)
#define I32_MAX   (2147483647_i32)
#define U32_MAX   (4294967295_u32)
#define I64_MIN   (-9223372036854775808_i64)
#define I64_MAX   (9223372036854775807_i64)
#define U64_MAX   (18446744073709551615_u64)
#define ISIZE_MIN I64_MIN
#define ISIZE_MAX I64_MAX
#define USIZE_MAX U64_MAX
