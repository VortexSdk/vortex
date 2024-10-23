#pragma once

#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "utils.hpp"

struct AllocatorState {
    usize len{0};
    usize pos{0};
    u8 *ptr{null<u8>()};

    AllocatorState() = default;
    AllocatorState(usize _len, usize _pos, u8 *_ptr) : len(_len), pos(_pos), ptr(_ptr) {}
    AllocatorState(AllocatorState &&as) noexcept
        : len(exchange(as.len, USIZE_0)), pos(exchange(as.pos, USIZE_0)),
          ptr(exchange(as.ptr, null<u8>())) {}
};

template <typename T>
concept AllocatorStrategy = requires(T t) {
    { t.free(null<AllocatorState>(), NULL, USIZE_0, USIZE_0) } -> same_as<void>;
    { t.alloc(null<AllocatorState>(), USIZE_0, USIZE_0) } -> same_as<void *>;
    { t.resize(null<AllocatorState>(), NULL, USIZE_0, USIZE_0, USIZE_0) } -> same_as<void *>;
};

template <AllocatorStrategy T> struct Allocator {
    T strategy{};
    AllocatorState state{};

    Allocator(const Allocator &t)            = delete;
    Allocator &operator=(const Allocator &t) = delete;
    Allocator(Allocator &&a) noexcept : strategy(move(a.strategy)), state(move(a.state)) {}
    Allocator(T _strategy, AllocatorState _state)
        : strategy(move(_strategy)), state(move(_state)) {}

    static Allocator<T> init(usize len, u8 *ptr, usize pos = 0) {
        return Allocator<T>(T{}, AllocatorState(len, pos, ptr));
    }

    template <typename E> Slice<E> alloc(this Allocator &self, usize c) {
        return Slice<E>::init(
            c, reinterpret_cast<E *>(self.strategy.alloc(&self.state, sizeof(E) * c, alignof(E)))
        );
    }

    template <typename E> E *create(this Allocator &self) {
        return self.template alloc<E>(1).ptr;
    }

    template <typename E> Slice<E> dup(this Allocator &self, Slice<const E> *const b) {
        Slice<E> s = self.template alloc<E>(b->len);
        if (s.is_empty()) return Slice<E>();

        memcpy(
            reinterpret_cast<void *>(s.ptr), reinterpret_cast<const void *>(b->ptr),
            sizeof(E) * b->len
        );

        return s;
    }

    template <typename E> Slice<E> resize(this Allocator &self, Slice<E> b, usize new_s) {
        return Slice<E>::init(
            new_s, reinterpret_cast<E *>(self.strategy.resize(
                       &self.state, reinterpret_cast<void *>(b.ptr), sizeof(E) * b.len,
                       sizeof(E) * new_s, alignof(E)
                   ))

        );
    }

    template <typename E> Slice<E> resize_or_alloc(this Allocator &self, Slice<E> b, usize new_s) {
        Slice<E> r = self.template resize<E>(Slice<E>::init(b.len, b.ptr), new_s);
        if (!r.is_empty()) return r;

        Slice<E> a = self.template alloc<E>(new_s);
        if (!a.is_empty()) {
            memcpy(
                reinterpret_cast<void *>(a.ptr), reinterpret_cast<const void *>(b.ptr),
                sizeof(E) * b.len
            );
            self.template free<E>(Slice<E>::init(b.len, b.ptr));
        }

        return a;
    }

    template <typename E> void free(this Allocator &self, Slice<E> b) {
        self.strategy.free(
            &self.state, reinterpret_cast<void *>(b.ptr), sizeof(E) * b.len, alignof(E)
        );
    }

    template <typename E> void destroy(this Allocator &self, E *b) {
        return self.template free<E>(Slice<E>::init(1, b));
    }
};
