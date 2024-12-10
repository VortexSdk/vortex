#pragma once

#include "mem/mem.hpp"
#include "metap/metap.hpp"
#include "numbers.hpp"
#include "vortex/metap/structs.hpp"

template <typename T> struct Vec {
    PIN_STRUCT(Vec, len, 0_u32, cap, 0_u32, ptr, reinterpret_cast<T *>(0))

    template <AllocatorStrategy U> static SysRes<Vec<T>> init(Allocator<U> *a, u32 capacity = 8) {
        Vec<T> v;
        v.cap                = capacity;
        const auto alloc_res = a->template alloc<T>(capacity);
        if (alloc_res.is_empty()) return SysRes<Vec<T>>::from_err(SysResKind::NOMEM);
        v.ptr = alloc_res.ptr;

        return move(v);
    }

    template <AllocatorStrategy U>
    static SysRes<Vec<T>> from(Allocator<U> *a, const Slice<const T> *const slice) {
        auto init_res = Vec<T>::init(a, slice->len);
        if (init_res.is_error()) return init_res;

        auto &v = init_res.unwrap();

        memcpy(
            reinterpret_cast<void *>(v.data.ptr), reinterpret_cast<const void *>(slice->ptr),
            slice->len * sizeof(T)
        );
        v.len = slice->len;

        return move(v);
    }

    template <AllocatorStrategy U> void deinit(this Vec<T> &self, Allocator<U> *a) {
        a->free(Slice<T>::init(self.cap, self.ptr));
    }

    Slice<T> as_mut_slice(this Vec<T> &self) {
        return Slice<T>::init(self.len, self.ptr);
    }
    Slice<const T> as_slice(this const Vec<T> &self) {
        return Slice<const T>::init(self.len, self.ptr);
    }

    template <AllocatorStrategy U> bool grow(this Vec<T> &self, Allocator<U> *a, usize new_cap) {
        if (new_cap <= self.cap) [[unlikely]]
            return false;

        auto resize_res =
            a->template resize_or_alloc<T>(Slice<T>::init(self.cap, self.ptr), new_cap);
        if (resize_res.is_empty()) return true;

        self.ptr = resize_res.ptr;
        self.cap = static_cast<u32>(new_cap);
        return false;
    }

    template <AllocatorStrategy U> bool push(this Vec<T> &self, Allocator<U> *a, T val) {
        if (self.len >= self.cap)
            if (self.grow(a, self.cap * 2)) return true;

        self.ptr [self.len] = move(val);
        self.len++;
        return false;
    }

    template <AllocatorStrategy U> bool resize(this Vec<T> &self, Allocator<U> *a, usize new_cap) {
        return self.grow(a, new_cap);
    }

    template <AllocatorStrategy U>
    bool append(this Vec<T> &self, Allocator<U> *a, const Slice<const T> *const slice) {
        const usize required_cap = self.len + slice->len;
        if (required_cap > self.cap) {
            const usize new_cap = max(required_cap, self.cap * 2);
            if (self.grow(a, new_cap)) return true;
        }

        memcpy(
            reinterpret_cast<void *>(self.ptr + self.len),
            reinterpret_cast<const void *>(slice->ptr), slice->len * sizeof(T)
        );
        self.len += slice->len;
        return false;
    }

    T *nth(this const Vec<T> &self, usize n) {
        if (n >= self.len) [[unlikely]]
            return null;

        return &self.ptr [n];
    }

    Slice<T> mut_subslice(this Vec<T> &self, usize start, usize sublen) {
        if (start >= self.len || start + sublen > self.len) [[unlikely]]
            return Slice<T>();

        return Slice<T>::init(sublen, &self.ptr [start]);
    }
    Slice<const T> subslice(this const Vec<T> &self, usize start, usize sublen) {
        if (start >= self.len || start + sublen > self.len) [[unlikely]]
            return Slice<const T>();

        return Slice<const T>::init(sublen, &self.ptr [start]);
    }

    template <AllocatorStrategy U> SysRes<Vec<T>> clone(this const Vec<T> &self, Allocator<U> *a) {
        auto init_res = Vec<T>::init(a, self.cap);
        if (init_res.is_error()) return init_res;

        auto &v = init_res.unwrap();
        memcpy(
            reinterpret_cast<void *>(v.data.ptr), reinterpret_cast<const void *>(self.ptr),
            self.len * sizeof(T)
        );
        v.len = self.len;

        return move(v);
    }
};

static_assert(sizeof(Vec<u8>) == 16, "Vec<u8> should be 16 bytes.");
