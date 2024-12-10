#pragma once

#include "../numbers.hpp"

#ifndef NULL
#define NULL (reinterpret_cast<void *>(0))
#endif

struct false_type {
    static const bool value = false;
};
struct true_type {
    static const bool value = true;
};
template <typename...> struct void_t {
    using type = void;
};

template <typename T> struct add_rvalue_reference {
    using type = T &;
};
template <typename T> struct add_rvalue_reference<T &> {
    using type = T &;  // If T is already a reference, keep it as is
};
template <typename T> struct add_rvalue_reference<T &&> {
    using type = T &&; // If T is an rvalue reference, keep it as is
};

template <typename T> struct remove_reference {
    using type = T;
};
template <typename T> struct remove_reference<T &> {
    using type = T;
};
template <typename T> struct remove_reference<T &&> {
    using type = T;
};

template <typename T> typename remove_reference<T>::type &&move(T &&t) {
    return static_cast<typename remove_reference<T>::type &&>(t);
}

template <typename T> struct remove_const {
    typedef T type;
};
template <typename T> struct remove_const<const T> {
    typedef T type;
};
template <typename T> struct remove_volatile {
    typedef T type;
};
template <typename T> struct remove_volatile<volatile T> {
    typedef T type;
};
template <typename T> struct remove_cv {
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};
template <typename T> using remove_cv_t = typename remove_cv<T>::type;

template <typename T> struct is_pointer_helper {
    static const bool value = false;
};
template <typename T> struct is_pointer_helper<T *> {
    static const bool value = true;
};
template <typename T> struct is_pointer {
    static const bool value = is_pointer_helper<typename remove_cv<T>::type>::value;
};

template <typename T> struct is_lvalue_reference : false_type {};
template <typename T> struct is_lvalue_reference<T &> : true_type {};
template <typename T> inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <bool B, typename T = void> struct enable_if {};
template <typename T> struct enable_if<true, T> {
    typedef T type;
};

template <typename T, T v> struct integral_constant {
    static constexpr T value = v;
    using value_type         = T;
    using type               = integral_constant; // using injected-class-name

    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    } // since c++14
};

template <
    typename T, typename AT_1 = void, typename AT_2 = void, typename AT_3 = void,
    typename AT_4 = void>
struct is_constructible_impl {
    static const bool value = (sizeof(test<T, AT_1, AT_2, AT_3, AT_4>(NULL)) == sizeof(bool));

  private:
    template <typename C_T, typename C_AT_1, typename C_AT_2, typename C_AT_3, typename C_AT_4>
    static bool
    test(typename enable_if<
         sizeof(C_T) ==
         sizeof(
             C_T(static_cast<C_AT_1>(*static_cast<typename remove_reference<C_AT_1>::type *>(NULL)),
                 static_cast<C_AT_2>(*static_cast<typename remove_reference<C_AT_2>::type *>(NULL)),
                 static_cast<C_AT_3>(*static_cast<typename remove_reference<C_AT_3>::type *>(NULL)),
                 static_cast<C_AT_4>(*static_cast<typename remove_reference<C_AT_4>::type *>(NULL)))
         )>::type *);

    template <typename, typename, typename, typename, typename> static int test(...);
};
template <typename T, typename AT_1, typename AT_2, typename AT_3>
struct is_constructible_impl<T, AT_1, AT_2, AT_3, void> {
    static const bool value = (sizeof(test<T, AT_1, AT_2, AT_3>(NULL)) == sizeof(bool));

  private:
    template <typename C_T, typename C_AT_1, typename C_AT_2, typename C_AT_3>
    static bool
    test(typename enable_if<
         sizeof(C_T) ==
         sizeof(
             C_T(static_cast<C_AT_1>(*static_cast<typename remove_reference<C_AT_1>::type *>(NULL)),
                 static_cast<C_AT_2>(*static_cast<typename remove_reference<C_AT_2>::type *>(NULL)),
                 static_cast<C_AT_3>(*static_cast<typename remove_reference<C_AT_3>::type *>(NULL)))
         )>::type *);

    template <typename, typename, typename, typename> static int test(...);
};
template <typename T, typename AT_1, typename AT_2>
struct is_constructible_impl<T, AT_1, AT_2, void, void> {
    static const bool value = (sizeof(test<T, AT_1, AT_2>(NULL)) == sizeof(bool));

  private:
    template <typename C_T, typename C_AT_1, typename C_AT_2>
    static bool
    test(typename enable_if<
         sizeof(C_T) ==
         sizeof(
             C_T(static_cast<C_AT_1>(*static_cast<typename remove_reference<C_AT_1>::type *>(NULL)),
                 static_cast<C_AT_2>(*static_cast<typename remove_reference<C_AT_2>::type *>(NULL)))
         )>::type *);

    template <typename, typename, typename> static int test(...);
};
template <typename T, typename AT_1> struct is_constructible_impl<T, AT_1, void, void, void> {
    static const bool value = (sizeof(test<T, AT_1>(NULL)) == sizeof(bool));

  private:
    template <typename C_T, typename C_AT_1>
    static bool
    test(typename enable_if<
         sizeof(C_T) ==
         sizeof(C_T(static_cast<C_AT_1>(*static_cast<typename remove_reference<C_AT_1>::type *>(NULL
         ))))>::type *);

    template <typename, typename> static int test(...);
};
template <typename T> struct is_constructible_impl<T, void, void, void, void> {
    static const bool value = (sizeof(test<T>(NULL)) == sizeof(bool));

  private:
    template <typename C_T> static C_T testFun(C_T);

    template <typename C_T>
    static bool test(typename enable_if<sizeof(C_T) == sizeof(testFun(C_T()))>::type *);

    template <typename> static int test(...);
};
template <
    typename T, typename AT_1 = void, typename AT_2 = void, typename AT_3 = void,
    typename AT_4 = void>
struct is_constructible_impl_ptr {
    static const bool value = false;
};
template <typename T, typename AT_1>
struct is_constructible_impl_ptr<
    T, AT_1, typename enable_if<is_pointer<typename remove_reference<T>::type>::value, void>::type,
    void, void> {
    static const bool value = (sizeof(test<T>(static_cast<AT_1>(NULL))) == sizeof(bool));

  private:
    template <typename C_T> static bool test(C_T);

    template <typename> static int test(...);
};
template <typename T> struct is_constructible_impl_ptr<T, void, void, void, void> {
    static const bool value = true;
};
template <
    typename T, typename AT_1 = void, typename AT_2 = void, typename AT_3 = void,
    typename AT_4 = void>
struct is_constructible {
    static const bool value =
        (is_pointer<typename remove_reference<T>::type>::value
             ? is_constructible_impl_ptr<T, AT_1, AT_2, AT_3, AT_4>::value
             : is_constructible_impl<T, AT_1, AT_2, AT_3, AT_4>::value);
};

template <typename T, typename... Args> struct is_trivially_constructible {
    static constexpr bool value = __is_trivially_constructible(T, Args...);
};
template <typename T, typename... Args> struct is_nothrow_constructible {
    static constexpr bool value = __is_nothrow_constructible(T, Args...);
};
template <typename T, typename U> struct is_nothrow_assignable {
    static constexpr bool value = __is_nothrow_assignable(T, U);
};

template <typename T>
struct is_move_constructible : is_constructible<T, typename add_rvalue_reference<T>::type> {};
template <typename T>
struct is_trivially_move_constructible
    : is_trivially_constructible<T, typename add_rvalue_reference<T>::type> {};

template <typename T>
struct is_nothrow_move_constructible
    : is_nothrow_constructible<T, typename add_rvalue_reference<T>::type> {};

template <typename T> inline T &&forward(typename remove_reference<T>::type &t) noexcept {
    return static_cast<T &&>(t);
}
template <typename T> inline T &&forward(typename remove_reference<T>::type &&t) noexcept {
    static_assert(!is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
    return static_cast<T &&>(t);
}

template <typename T, typename U = T>
constexpr T exchange(T &obj, U &&new_value) noexcept(
    is_nothrow_move_constructible<T>::value && is_nothrow_assignable<T &, U>::value
) {
    T old_value = move(obj);
    obj         = forward<U>(new_value);
    return old_value;
}

template <typename T> typename add_rvalue_reference<T>::type declval() noexcept;

template <typename T, typename U> struct is_same : false_type {};
template <typename T> struct is_same<T, T> : true_type {};
template <typename T, typename U> inline constexpr bool is_same_v = is_same<T, U>::value;

template <typename T, typename U>
concept SameHelper = is_same_v<T, U>;
template <typename T, typename U>
concept same_as = SameHelper<T, U> && SameHelper<U, T>;

template <typename T> struct exact_type {
    operator T() const;
};

template <typename T> struct is_integral {
    static const bool value = false;
};
template <> struct is_integral<bool> {
    static const bool value = true;
};
template <> struct is_integral<char> {
    static const bool value = true;
};
template <> struct is_integral<signed char> {
    static const bool value = true;
};
template <> struct is_integral<unsigned char> {
    static const bool value = true;
};
template <> struct is_integral<short> {
    static const bool value = true;
};
template <> struct is_integral<unsigned short> {
    static const bool value = true;
};
template <> struct is_integral<int> {
    static const bool value = true;
};
template <> struct is_integral<unsigned int> {
    static const bool value = true;
};
template <> struct is_integral<long> {
    static const bool value = true;
};
template <> struct is_integral<unsigned long> {
    static const bool value = true;
};
template <> struct is_integral<long long> {
    static const bool value = true;
};
template <> struct is_integral<unsigned long long> {
    static const bool value = true;
};
template <typename T> inline constexpr bool is_integral_v = is_integral<T>::value;
template <typename T, bool IsIntegral = is_integral<T>::value> struct make_unsigned_impl {
    using type = void; // If T is not an integral type, no transformation occurs
};

template <typename T> struct is_unsigned {
    static const bool value = false;
};
template <> struct is_unsigned<unsigned char> {
    static const bool value = true;
};
template <> struct is_unsigned<unsigned short> {
    static const bool value = true;
};
template <> struct is_unsigned<unsigned int> {
    static const bool value = true;
};
template <> struct is_unsigned<unsigned long> {
    static const bool value = true;
};
template <> struct is_unsigned<unsigned long long> {
    static const bool value = true;
};

template <typename T> struct make_unsigned_impl<T, true> {
    using type = T;
};
template <> struct make_unsigned_impl<signed char, true> {
    using type = unsigned char;
};
template <> struct make_unsigned_impl<short, true> {
    using type = unsigned short;
};
template <> struct make_unsigned_impl<int, true> {
    using type = unsigned int;
};
template <> struct make_unsigned_impl<long, true> {
    using type = unsigned long;
};
template <> struct make_unsigned_impl<long long, true> {
    using type = unsigned long long;
};
template <> struct make_unsigned_impl<unsigned char, true> {
    using type = unsigned char;
};
template <> struct make_unsigned_impl<unsigned short, true> {
    using type = unsigned short;
};
template <> struct make_unsigned_impl<unsigned int, true> {
    using type = unsigned int;
};
template <> struct make_unsigned_impl<unsigned long, true> {
    using type = unsigned long;
};
template <> struct make_unsigned_impl<unsigned long long, true> {
    using type = unsigned long long;
};
template <typename T> using make_unsigned = typename make_unsigned_impl<T>::type;

template <typename T> struct remove_pointer {
    using type = T;
};
template <typename T> struct remove_pointer<T *> {
    using type = T;
};
template <typename T> using remove_pointer_t = typename remove_pointer<T>::type;

template <typename T> struct is_union {
    static const bool value = __is_union(T);
};
template <typename T> inline constexpr bool is_union_v = is_union<T>::value;
template <typename T> struct is_enum {
    static const bool value = __is_enum(T);
};
template <typename T> inline constexpr bool is_enum_v = is_enum<T>::value;

namespace IsClassDetail {
template <typename T> integral_constant<bool, !is_union<T>::value> test(int T::*);
template <typename> false_type test(...);
} // namespace IsClassDetail
template <typename T> struct is_class : decltype(IsClassDetail::test<T>(nullptr)) {};
template <typename T> inline constexpr bool is_class_v = is_class<T>::value;

template <typename T> struct is_array : false_type {};
template <typename T> struct is_array<T []> : true_type {};
template <typename T, usize N> struct is_array<T [N]> : true_type {};
template <typename T> inline constexpr bool is_array_v = is_array<T>::value;

template <typename T> struct is_bounded_array : false_type {};
template <typename T, usize N> struct is_bounded_array<T [N]> : true_type {};
template <typename T> inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

template <typename T> struct is_unbounded_array : false_type {};
template <typename T> struct is_unbounded_array<T []> : true_type {};
template <typename T> inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

template <typename T> struct is_floating_point : false_type {};
template <> struct is_floating_point<float> : true_type {};
template <> struct is_floating_point<double> : true_type {};

template <typename T> struct is_signed : false_type {};
template <> struct is_signed<signed char> : true_type {};
template <> struct is_signed<short> : true_type {};
template <> struct is_signed<int> : true_type {};
template <> struct is_signed<long> : true_type {};
template <> struct is_signed<long long> : true_type {};
template <> struct is_signed<f32> : true_type {};
template <> struct is_signed<f64> : true_type {};

template <typename T> inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
template <typename T> inline constexpr bool is_signed_v         = is_signed<T>::value;

template <typename T>
struct is_arithmetic
    : integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};

template <typename T> inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
