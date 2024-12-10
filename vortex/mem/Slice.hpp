#pragma once

#include "../metap/structs.hpp"
#include "utils.hpp"

template <typename T> struct FixedSlice;

template <typename T> struct Slice {
    PIN_STRUCT(Slice, len, 0_usize, ptr, reinterpret_cast<T *>(0))

    template <usize Y> Slice(T (&arr) [Y]) : len(Y), ptr(arr) {}

    operator Slice<const T>(this const Slice & self) {
        return self;
    }

    operator FixedSlice<T>(this const Slice & self) {
        return FixedSlice<T>::init(self.len, self.ptr);
    }

    static Slice<T> init(usize len, T *ptr) {
        return Slice<T>(len, ptr);
    }

    bool is_empty(this const Slice &self) {
        return self.ptr == null;
    }

    T *get_unchecked(this const Slice &self, usize i) {
        return &self.ptr [i];
    }

    T *get(this const Slice &self, usize i) {
        if (i >= self.len) [[unlikely]]
            return null;

        return self.get_unchecked(i);
    }

    T *operator[](this const Slice &self, usize idx) {
        return self.get(idx);
    }

    bool operator==(this const Slice &self, Slice<const T> *const other) {
        if (self.len != other->len) return false;
        for (usize i = 0; i < self.len; i++) {
            if (self.ptr [i] != other->ptr [i]) return false;
        }

        return true;
    }
};

template <typename T> struct FixedSlice {
  private:
    usize len{0};

    Slice<T> as_slice(this const FixedSlice &self) {
        return Slice<T>(self.len, self.ptr);
    }

  public:
    T *ptr;

    FixedSlice(const FixedSlice &t)            = delete;
    FixedSlice &operator=(const FixedSlice &t) = delete;
    FixedSlice(FixedSlice &&s) noexcept
        : len(exchange(s.len, 0_usize)), ptr(exchange(s.ptr, null)) {}
    FixedSlice &operator=(FixedSlice &&other) noexcept {
        if (this != &other) {
            ptr = exchange(other.ptr, null);
            len = exchange(other.len, 0_usize);
        }
        return *this;
    }

    operator FixedSlice<const T>(this const FixedSlice & self) {
        return self;
    }

    static FixedSlice<T> init(usize len, T *ptr) {
        return FixedSlice<T>(len, ptr);
    }

    bool is_empty(this const FixedSlice &self) {
        return self.as_slice().is_empty();
    }

    T *get_unchecked(this const FixedSlice &self, usize i) {
        return self.as_slice().get_unchecked(i);
    }

    T *get(this const FixedSlice &self, usize i) {
        return self.as_slice().get(i);
    }

    T *operator[](this const FixedSlice &self, usize idx) {
        return self.get(idx);
    }

    bool operator==(this const FixedSlice &self, FixedSlice<const T> *const other) {
        return self.as_slice() == other->as_slice();
    }
};

template <typename T> struct SliceWithPos {
    PIN_STRUCT(SliceWithPos, pos, 0_usize, slice, (Slice<T>()))

    operator Slice<T>(this const SliceWithPos & self) {
        return self.slice;
    }

    operator SliceWithPos<const T>(this const SliceWithPos & self) {
        return self;
    }

    static SliceWithPos<T> init(Slice<T> slice, usize pos = 0) {
        return FixedSlice<T>(pos, slice);
    }

    static SliceWithPos<T> init(usize len, T *ptr, usize pos = 0) {
        return FixedSlice<T>(pos, Slice<T>(len, ptr));
    }

    bool is_empty(this const SliceWithPos &self) {
        return self.slice.is_empty();
    }

    T *get_unchecked(this const SliceWithPos &self, usize i) {
        return &self.slice.ptr [i];
    }

    T *get(this const SliceWithPos &self, usize i) {
        if (i > self.pos) return null;

        return self.get_unchecked(i);
    }

    Slice<T> get(this const SliceWithPos &self, usize from, usize to) {
        if (from < 1 || to > self.pos || to < from) [[unlikely]]
            return Slice<T>();

        return Slice<T>::init((to - from), &self.slice.ptr [from]);
    }

    T *put(this SliceWithPos &self, const T v) {
        if (self.pos >= (self.slice.len - 1)) [[unlikely]]
            return null;

        auto ptr = &self.slice.ptr [self.pos];
        *ptr     = move(v);
        self.pos++;

        return ptr;
    }

    bool pop(this SliceWithPos &self, usize c) {
        if (c > self.pos) [[unlikely]]
            return true;

        self.pos -= c;
        return false;
    }

    bool pop(this SliceWithPos &self) {
        return self.pop(1);
    }

    Slice<T> put(this SliceWithPos &self, Slice<const T> *const v) {
        const usize new_pos = self.pos + v->len;
        if (new_pos >= (self.slice.len - 1)) [[unlikely]]
            return Slice<T>();

        for (usize i = 0; self.pos < new_pos; i++)
            *self.slice.get_unchecked(self.pos + i) = *v->get_unchecked(i);

        auto ptr = self.slice.get_unchecked(self.pos);
        self.pos = new_pos;

        return Slice<T>::init(v->len, ptr);
    }

    T *operator[](this const SliceWithPos &self, usize idx) {
        return self.get(idx);
    }

    bool operator==(this const SliceWithPos &self, SliceWithPos<const T> *const other) {
        return self.slice == other->slice;
    }
};
