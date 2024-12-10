#pragma once

#include "../metap/metap.hpp"
#include "../metap/structs.hpp"
#include "../numbers.hpp"
#include "utils.hpp"

struct AllocatorState {
    PIN_STRUCT(AllocatorState, len, 0_usize, pos, 0_usize, ptr, reinterpret_cast<u8 *>(0))
};

template <typename T>
concept AllocatorStrategy = requires(T t) {
    { t.free(null, null, 0_usize, 0_usize) } -> same_as<void>;
    { t.alloc(null, 0_usize, 0_usize) } -> same_as<void *>;
    { t.resize(null, null, 0_usize, 0_usize, 0_usize) } -> same_as<void *>;
};

template <AllocatorStrategy T> struct Allocator {
    PIN_STRUCT(Allocator, strategy, T(), state, (AllocatorState()))

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

#define TRY_ALLOC(expr, t) TRY(expr, t, SysResKind::NOMEM)
