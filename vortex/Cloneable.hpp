#pragma once

#include "mem/Allocator.hpp"
#include "mem/Arena.hpp"
#include "metap/metap.hpp"

template <typename T>
concept Cloneable = requires(T t) {
    { t.clone(null<Allocator<Arena>>()) } -> same_as<T>;
};
