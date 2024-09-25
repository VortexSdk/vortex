#pragma once

/// Similar to std::false_type
struct false_type {
    static const bool value = false;
};
/// Similar to std::true_type
struct true_type {
    static const bool value = true;
};
/// Similar to std::void_t
template <typename...> struct void_t {
    using type = void;
};

/// Similar to std::add_rvalue_reference
template <typename T> struct add_rvalue_reference {
    using type = T &;
};
/// Similar to std::add_rvalue_reference
template <typename T> struct add_rvalue_reference<T &> {
    using type = T &; // If T is already a reference, keep it as is
};
/// Similar to std::add_rvalue_reference
template <typename T> struct add_rvalue_reference<T &&> {
    using type = T &&; // If T is an rvalue reference, keep it as is
};

/// Similar to std::remove_reference
template <typename T> struct remove_reference {
    using type = T;
};
/// Similar to std::remove_reference
template <typename T> struct remove_reference<T &> {
    using type = T;
};
/// Similar to std::remove_reference
template <typename T> struct remove_reference<T &&> {
    using type = T;
};

/// Similar to std::move
template <typename T> typename remove_reference<T>::type &&move(T &&t) {
    return static_cast<typename remove_reference<T>::type &&>(t);
}

template <typename T> typename add_rvalue_reference<T>::type declval() noexcept;

template <class T, class U> struct is_same : false_type {};
template <class T> struct is_same<T, T> : true_type {};
template <class T, class U> inline constexpr bool is_same_v = is_same<T, U>::value;

template <class T, class U>
concept SameHelper = is_same_v<T, U>;
template <class T, class U>
concept same_as = SameHelper<T, U> && SameHelper<U, T>;

template <typename T> struct exact_type {
    operator T() const;
};
