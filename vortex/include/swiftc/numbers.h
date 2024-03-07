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

_Static_assert(sizeof(signed char) == 1, "`signed char` should be 1 byte.");
typedef signed char i8;
_Static_assert(sizeof(unsigned char) == 1, "`unsigned char` should be 1 byte.");
typedef unsigned char u8;
_Static_assert(sizeof(signed short int) == 2, "`signed short int` should be 2 bytes.");
typedef signed short int i16;
_Static_assert(sizeof(unsigned short int) == 2, "`unsigned short int` should be 2 bytes.");
typedef unsigned short int u16;
_Static_assert(sizeof(signed int) == 4, "`signed int` should be 4 bytes.");
typedef signed int i32;
_Static_assert(sizeof(unsigned int) == 4, "`unsigned int` should be 4 bytes.");
typedef unsigned int u32;

#if WRDSIZE == 64
_Static_assert(sizeof(signed long int) == 8, "`signed long int` should be 8 bytes.");
typedef signed long int i64;
_Static_assert(sizeof(unsigned long int) == 8, "`unsigned long int` should be 8 bytes.");
typedef unsigned long int u64;
#else
_Static_assert(sizeof(signed long long int) == 8, "`signed long long int` should be 8 bytes.");
typedef signed long long int i64;
_Static_assert(sizeof(unsigned long long int) == 8, "`unsigned long long int` should be 8 bytes.");
typedef unsigned long long int u64;
#endif

typedef i64 isize;
typedef u64 usize;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
#elif !defined(__cplusplus)
    #define bool  _Bool
    #define true  1
    #define false 0
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
    /* Define _Bool as a GNU extension. */
    #define _Bool bool
    #if defined(__cplusplus) && __cplusplus < 201103L
        /* For C++98, define bool, false, true as a GNU extension. */
        #define bool  bool
        #define false false
        #define true  true
    #endif
#endif
