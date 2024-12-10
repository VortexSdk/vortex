#pragma once

#include "mem/utils.hpp"
#include "numbers.hpp"

template <typename T, usize c> struct Array {
    T data [c];

    Array(T _data [c]) : data(_data) {}
    Array() {
        memset(reinterpret_cast<void *>(data), 0, sizeof(T) * c);
    }

    Array(const Array &t)            = delete;
    Array &operator=(const Array &t) = delete;

    Array(Array &&s) noexcept {
        const usize bytes = sizeof(T) * c;
        auto s_data       = reinterpret_cast<void *>(s.data);

        memcpy(reinterpret_cast<void *>(data), s_data, bytes);
        memset(s_data, 0, bytes);
    }
    Array &operator=(Array &&other) noexcept {
        if (this != &other) [[likely]] {
            const usize bytes = sizeof(T) * c;
            auto o_data       = reinterpret_cast<void *>(other.data);

            memcpy(reinterpret_cast<void *>(data), o_data, bytes);
            memset(o_data, 0, bytes);
        }

        return *this;
    }

    operator Slice<T>(this const Array & self) {
        return Slice<T>::init(c, self.data);
    }
    operator FixedSlice<T>(this const Array & self) {
        return FixedSlice<T>::init(c, self.data);
    }

    operator T *() {
        return data;
    }
    operator const T *() const {
        return data;
    }
    operator usize() {
        return reinterpret_cast<usize>(data);
    }
};
