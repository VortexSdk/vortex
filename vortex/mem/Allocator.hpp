#pragma once

#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "utils.hpp"

struct AllocatorState {
    void *ptr;
    usize pos{0};
    usize len;

    AllocatorState(const AllocatorState &) = delete;

    AllocatorState(void *_ptr, usize _len) : ptr(_ptr), len(_len) {}
};

template <typename T>
concept AllocatorStrategy = requires(T t) {
    { t.free(reinterpret_cast<AllocatorState *>(null_ptr), null_ptr, 0) } -> same_as<void>;
    {
        t.alloc(reinterpret_cast<AllocatorState *>(null_ptr), static_cast<usize>(0))
    } -> same_as<void *>;
    {
        t.resize(
            reinterpret_cast<AllocatorState *>(null_ptr), null_ptr, static_cast<usize>(0),
            static_cast<usize>(0)
        )
    } -> same_as<void *>;
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
template <AllocatorStrategy T> struct Allocator {
    T strategy;
    AllocatorState state;

    Allocator()  = delete;
    ~Allocator() = default;

    Allocator(void *_ptr, usize _len) : strategy(), state(_ptr, _len) {}

    template <typename E> E *create(this Allocator &self) {
        reinterpret_cast<E *>(self.strategy.alloc(&self.state, sizeof(E)));
    }

    template <typename E> Slice<E> alloc(this Allocator &self, usize c) {
        return Slice<E>{reinterpret_cast<E *>(self.strategy.alloc(&self.state, sizeof(E) * c)), c};
    }

    template <typename E> Slice<E> dup(this Allocator &self, Slice<E> b) {
        Slice<E> const s = {
            reinterpret_cast<E *>(self.strategy.alloc(&self.state, sizeof(E) * b.len)), b.len
        };
        memcpy(s.ptr, b.ptr, b.len);

        return s;
    }

    template <typename E> Slice<E> resize(this Allocator &self, Slice<E> b, usize new_s) {
        return Slice<E>{
            reinterpret_cast<E *>(self.strategy.resize(
                &self.state, reinterpret_cast<void *>(b.ptr), sizeof(E) * b.len, sizeof(E) * new_s
            )),
            new_s
        };
    }

    template <typename E> void destroy(this Allocator &self, E *b) {
        self.strategy.free(reinterpret_cast<void *>(b), sizeof(E));
    }

    template <typename E> void free(this Allocator &self, Slice<E> b) {
        self.strategy.free(reinterpret_cast<void *>(b.ptr), sizeof(E) * b.len);
    }
};
#pragma clang diagnostic pop
