#pragma once

#include "diagnostics.hpp"
#include "metap.hpp"

#define _FIELD_INIT(field, value)                       field(value)
#define _FIELD_COPY(field, unused)                      field(other.field)
#define _FIELD_MOVE(field, default)                     field(exchange(other.field, default))
#define _FIELD_PAIR3(m, f1, v1)                         m(f1, v1)
#define _FIELD_PAIR5(m, f1, v1, f2, v2)                 m(f1, v1), m(f2, v2)
#define _FIELD_PAIR7(m, f1, v1, f2, v2, f3, v3)         m(f1, v1), m(f2, v2), m(f3, v3)
#define _FIELD_PAIR9(m, f1, v1, f2, v2, f3, v3, f4, v4) m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4)
#define _FIELD_PAIR11(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5)                                   \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5)
#define _FIELD_PAIR13(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6)                           \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6)
#define _FIELD_PAIR15(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7)                   \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7)
#define _FIELD_PAIR17(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8)           \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7), m(f8, v8)
#define _FIELD_PAIR19(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9)   \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7), m(f8, v8),        \
        m(f9, v9)
#define _FIELD_PAIR21(                                                                             \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10            \
)                                                                                                  \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7), m(f8, v8),        \
        m(f9, v9), m(f10, v10)
#define _FIELD_PAIR23(                                                                             \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11  \
)                                                                                                  \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7), m(f8, v8),        \
        m(f9, v9), m(f10, v10), m(f11, v11)
#define _FIELD_PAIR25(                                                                             \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11, \
    f12, v12                                                                                       \
)                                                                                                  \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7), m(f8, v8),        \
        m(f9, v9), m(f10, v10), m(f11, v11), m(f12, v12)
#define _FIELD_PAIR27(                                                                             \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11, \
    f12, v12, f13, v13                                                                             \
)                                                                                                  \
    m(f1, v1), m(f2, v2), m(f3, v3), m(f4, v4), m(f5, v5), m(f6, v6), m(f7, v7), m(f8, v8),        \
        m(f9, v9), m(f10, v10), m(f11, v11), m(f12, v12), m(f13, v13)

#define _FIELD_PAIR(...)                   _VFUNC(_FIELD_PAIR, __VA_ARGS__)

#define _ASSIGN_FIELD_COPY(field, _)       other.field
#define _ASSIGN_FIELD_MOVE(field, default) exchange(other.field, default)
#define _ASSIGN_FIELD_PAIR3(m, f1, v1)     f1 = m(f1, v1);
#define _ASSIGN_FIELD_PAIR5(m, f1, v1, f2, v2)                                                     \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);
#define _ASSIGN_FIELD_PAIR7(m, f1, v1, f2, v2, f3, v3)                                             \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);
#define _ASSIGN_FIELD_PAIR9(m, f1, v1, f2, v2, f3, v3, f4, v4)                                     \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);                                                                                \
    f4 = m(f4, v4);
#define _ASSIGN_FIELD_PAIR11(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5)                            \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);                                                                                \
    f4 = m(f4, v4);                                                                                \
    f5 = m(f5, v5);
#define _ASSIGN_FIELD_PAIR13(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6)                    \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);                                                                                \
    f4 = m(f4, v4);                                                                                \
    f5 = m(f5, v5);                                                                                \
    f6 = m(f6, v6);
#define _ASSIGN_FIELD_PAIR15(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7)            \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);                                                                                \
    f4 = m(f4, v4);                                                                                \
    f5 = m(f5, v5);                                                                                \
    f6 = m(f6, v6);                                                                                \
    f7 = m(f7, v7);
#define _ASSIGN_FIELD_PAIR17(m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8)    \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);                                                                                \
    f4 = m(f4, v4);                                                                                \
    f5 = m(f5, v5);                                                                                \
    f6 = m(f6, v6);                                                                                \
    f7 = m(f7, v7);                                                                                \
    f8 = m(f8, v8);
#define _ASSIGN_FIELD_PAIR19(                                                                      \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9                      \
)                                                                                                  \
    f1 = m(f1, v1);                                                                                \
    f2 = m(f2, v2);                                                                                \
    f3 = m(f3, v3);                                                                                \
    f4 = m(f4, v4);                                                                                \
    f5 = m(f5, v5);                                                                                \
    f6 = m(f6, v6);                                                                                \
    f7 = m(f7, v7);                                                                                \
    f8 = m(f8, v8);                                                                                \
    f9 = m(f9, v9);
#define _ASSIGN_FIELD_PAIR21(                                                                      \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10            \
)                                                                                                  \
    f1  = m(f1, v1);                                                                               \
    f2  = m(f2, v2);                                                                               \
    f3  = m(f3, v3);                                                                               \
    f4  = m(f4, v4);                                                                               \
    f5  = m(f5, v5);                                                                               \
    f6  = m(f6, v6);                                                                               \
    f7  = m(f7, v7);                                                                               \
    f8  = m(f8, v8);                                                                               \
    f9  = m(f9, v9);                                                                               \
    f10 = m(f10, v10);
#define _ASSIGN_FIELD_PAIR23(                                                                      \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11  \
)                                                                                                  \
    f1  = m(f1, v1);                                                                               \
    f2  = m(f2, v2);                                                                               \
    f3  = m(f3, v3);                                                                               \
    f4  = m(f4, v4);                                                                               \
    f5  = m(f5, v5);                                                                               \
    f6  = m(f6, v6);                                                                               \
    f7  = m(f7, v7);                                                                               \
    f8  = m(f8, v8);                                                                               \
    f9  = m(f9, v9);                                                                               \
    f10 = m(f10, v10);                                                                             \
    f11 = m(f11, v11);
#define _ASSIGN_FIELD_PAIR25(                                                                      \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11, \
    f12, v12                                                                                       \
)                                                                                                  \
    f1  = m(f1, v1);                                                                               \
    f2  = m(f2, v2);                                                                               \
    f3  = m(f3, v3);                                                                               \
    f4  = m(f4, v4);                                                                               \
    f5  = m(f5, v5);                                                                               \
    f6  = m(f6, v6);                                                                               \
    f7  = m(f7, v7);                                                                               \
    f8  = m(f8, v8);                                                                               \
    f9  = m(f9, v9);                                                                               \
    f10 = m(f10, v10);                                                                             \
    f11 = m(f11, v11);                                                                             \
    f12 = m(f12, v12);
#define _ASSIGN_FIELD_PAIR27(                                                                      \
    m, f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11, \
    f12, v12, f13, v13                                                                             \
)                                                                                                  \
    f1  = m(f1, v1);                                                                               \
    f2  = m(f2, v2);                                                                               \
    f3  = m(f3, v3);                                                                               \
    f4  = m(f4, v4);                                                                               \
    f5  = m(f5, v5);                                                                               \
    f6  = m(f6, v6);                                                                               \
    f7  = m(f7, v7);                                                                               \
    f8  = m(f8, v8);                                                                               \
    f9  = m(f9, v9);                                                                               \
    f10 = m(f10, v10);                                                                             \
    f11 = m(f11, v11);                                                                             \
    f12 = m(f12, v12);                                                                             \
    f13 = m(f13, v13);
#define _ASSIGN_FIELD_PAIR(...) _VFUNC(_ASSIGN_FIELD_PAIR, __VA_ARGS__)

#define _FIELD_DEF2(f1, v1)     decltype(v1) f1;
#define _FIELD_DEF4(f1, v1, f2, v2)                                                                \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;
#define _FIELD_DEF6(f1, v1, f2, v2, f3, v3)                                                        \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;
#define _FIELD_DEF8(f1, v1, f2, v2, f3, v3, f4, v4)                                                \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;
#define _FIELD_DEF10(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5)                                       \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;
#define _FIELD_DEF12(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6)                               \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;
#define _FIELD_DEF14(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7)                       \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;
#define _FIELD_DEF16(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8)               \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;                                                                               \
    decltype(v8) f8;
#define _FIELD_DEF18(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9)       \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;                                                                               \
    decltype(v8) f8;                                                                               \
    decltype(v9) f9;
#define _FIELD_DEF20(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10               \
)                                                                                                  \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;                                                                               \
    decltype(v8) f8;                                                                               \
    decltype(v9) f9;                                                                               \
    decltype(v10) f10;
#define _FIELD_DEF22(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11     \
)                                                                                                  \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;                                                                               \
    decltype(v8) f8;                                                                               \
    decltype(v9) f9;                                                                               \
    decltype(v10) f10;                                                                             \
    decltype(v11) f11;
#define _FIELD_DEF24(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11,    \
    f12, v12                                                                                       \
)                                                                                                  \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;                                                                               \
    decltype(v8) f8;                                                                               \
    decltype(v9) f9;                                                                               \
    decltype(v10) f10;                                                                             \
    decltype(v11) f11;                                                                             \
    decltype(v12) f12;
#define _FIELD_DEF26(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11,    \
    f12, v12, f13, v13                                                                             \
)                                                                                                  \
    decltype(v1) f1;                                                                               \
    decltype(v2) f2;                                                                               \
    decltype(v3) f3;                                                                               \
    decltype(v4) f4;                                                                               \
    decltype(v5) f5;                                                                               \
    decltype(v6) f6;                                                                               \
    decltype(v7) f7;                                                                               \
    decltype(v8) f8;                                                                               \
    decltype(v9) f9;                                                                               \
    decltype(v10) f10;                                                                             \
    decltype(v11) f11;                                                                             \
    decltype(v12) f12;                                                                             \
    decltype(v13) f13;
#define _FIELD_DEF(...)             _VFUNC(_FIELD_DEF, __VA_ARGS__)

#define _PARAM_DEF2(f1, v1)         decltype(v1) _##f1
#define _PARAM_DEF4(f1, v1, f2, v2) decltype(v1) _##f1, decltype(v2) _##f2
#define _PARAM_DEF6(f1, v1, f2, v2, f3, v3)                                                        \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3
#define _PARAM_DEF8(f1, v1, f2, v2, f3, v3, f4, v4)                                                \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4
#define _PARAM_DEF10(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5)                                       \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5
#define _PARAM_DEF12(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6)                               \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6
#define _PARAM_DEF14(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7)                       \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7
#define _PARAM_DEF16(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8)               \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7, decltype(v8) _##f8
#define _PARAM_DEF18(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9)       \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7, decltype(v8) _##f8,            \
        decltype(v9) _##f9
#define _PARAM_DEF20(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10               \
)                                                                                                  \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7, decltype(v8) _##f8,            \
        decltype(v9) _##f9, decltype(v10) _##f10
#define _PARAM_DEF22(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11     \
)                                                                                                  \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7, decltype(v8) _##f8,            \
        decltype(v9) _##f9, decltype(v10) _##f10, decltype(v11) _##f11
#define _PARAM_DEF24(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11,    \
    f12, v12                                                                                       \
)                                                                                                  \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7, decltype(v8) _##f8,            \
        decltype(v9) _##f9, decltype(v10) _##f10, decltype(v11) _##f11, decltype(v12) _##f12
#define _PARAM_DEF26(                                                                              \
    f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11,    \
    f12, v12, f13, v13                                                                             \
)                                                                                                  \
    decltype(v1) _##f1, decltype(v2) _##f2, decltype(v3) _##f3, decltype(v4) _##f4,                \
        decltype(v5) _##f5, decltype(v6) _##f6, decltype(v7) _##f7, decltype(v8) _##f8,            \
        decltype(v9) _##f9, decltype(v10) _##f10, decltype(v11) _##f11, decltype(v12) _##f12,      \
        decltype(v13) _##f13
#define _PARAM_DEF(...)            _VFUNC(_PARAM_DEF, __VA_ARGS__)

#define _PARAM_INIT(field, unused) field(move(_##field))
#define _PARAM_PAIR(...)           _VFUNC(_FIELD_PAIR, _PARAM_INIT, __VA_ARGS__)

#define PIN_STRUCT(Type, ...)                                                                      \
    DIAG_IGNORE_CLANG_PUSH("-Wredundant-parens")                                                   \
                                                                                                   \
    _FIELD_DEF(__VA_ARGS__)                                                                        \
                                                                                                   \
    Type() : _FIELD_PAIR(_FIELD_INIT, __VA_ARGS__) {}                                              \
    Type(_PARAM_DEF(__VA_ARGS__)) : _PARAM_PAIR(__VA_ARGS__) {}                                    \
                                                                                                   \
    Type(const Type& other)            = delete;                                                   \
    Type& operator=(const Type& other) = delete;                                                   \
                                                                                                   \
    Type(Type&& other) noexcept : _FIELD_PAIR(_FIELD_MOVE, __VA_ARGS__) {}                         \
    Type& operator=(Type&& other) noexcept {                                                       \
        if (this != &other) [[likely]] {                                                           \
            _ASSIGN_FIELD_PAIR(_ASSIGN_FIELD_MOVE, __VA_ARGS__)                                    \
        }                                                                                          \
                                                                                                   \
        return *this;                                                                              \
    }                                                                                              \
                                                                                                   \
    DIAG_IGNORE_CLANG_POP

#define VAL_STRUCT(Type, ...)                                                                      \
    DIAG_IGNORE_CLANG_PUSH("-Wredundant-parens")                                                   \
                                                                                                   \
    _FIELD_DEF(__VA_ARGS__)                                                                        \
                                                                                                   \
    Type() : _FIELD_PAIR(_FIELD_INIT, __VA_ARGS__) {}                                              \
    Type(_PARAM_DEF(__VA_ARGS__)) : _PARAM_PAIR(__VA_ARGS__) {}                                    \
                                                                                                   \
    Type(const Type& other) : _FIELD_PAIR(_FIELD_COPY, __VA_ARGS__) {}                             \
    Type& operator=(const Type& other) {                                                           \
        if (this != &other) [[likely]] {                                                           \
            _ASSIGN_FIELD_PAIR(_ASSIGN_FIELD_COPY, __VA_ARGS__)                                    \
        }                                                                                          \
                                                                                                   \
        return *this;                                                                              \
    }                                                                                              \
                                                                                                   \
    Type(Type&& other) noexcept : _FIELD_PAIR(_FIELD_MOVE, __VA_ARGS__) {}                         \
    Type& operator=(Type&& other) noexcept {                                                       \
        if (this != &other) [[likely]] {                                                           \
            _ASSIGN_FIELD_PAIR(_ASSIGN_FIELD_MOVE, __VA_ARGS__)                                    \
        }                                                                                          \
                                                                                                   \
        return *this;                                                                              \
    }                                                                                              \
                                                                                                   \
    DIAG_IGNORE_CLANG_POP
