#pragma once

#include "mem/utils.hpp"
#include "metap/metap.hpp"

template <typename T>
concept Cloneable = requires(T t) {
    { t.clone(null) } -> same_as<T>;
};
