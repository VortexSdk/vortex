#pragma once

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

using isize = i64;
using usize = u64;
