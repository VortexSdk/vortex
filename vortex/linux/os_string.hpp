#pragma once

#include "../mem/Slice.hpp"
#include "../strings.hpp"

struct ImOsString {
    VAL_STRUCT(ImOsString, s, NullTermSlice<const char>())

    template <usize Y>
    ImOsString(const char (&arr) [Y]) : s(NullTermSlice<const char>::init(Y, arr)) {}

    static ImOsString init(const NullTermSlice<const char> &_s) {
        return {_s};
    }
    static ImOsString init(const Slice<char> &_s) {
        return {NullTermSlice<char>::init(_s.len, _s.ptr)};
    }
    static ImOsString init(usize len_with_sentinel, const char *ptr) {
        return ImOsString::init(NullTermSlice<const char>::init(len_with_sentinel, ptr));
    }
    static ImOsString init(const char *ptr) {
        return ImOsString::init(NullTermSlice<const char>::init(strlen(ptr) + 1, ptr));
    }

    bool is_empty(this const ImOsString &self) {
        return self.s.is_empty();
    }
    usize len(this const ImOsString &self) {
        return self.s.len();
    }
    usize len_with_nullchar(this const ImOsString &self) {
        return self.s.len_with_nullchar();
    }
    const char *ptr(this ImOsString &self) {
        return self.s.ptr();
    }

    operator NullTermSlice<const char>() {
        return s;
    }
    const char *operator[](this const ImOsString &self, usize idx) {
        return self.s.get(idx);
    }

    NullTermSlice<const char> as_nullterm_slice(this ImOsString &self) {
        return self;
    }
    Slice<const char> as_slice(this ImOsString &self) {
        return self.as_nullterm_slice().as_slice();
    }
    Slice<const char> as_slice_with_nullchar(this ImOsString &self) {
        return self.as_nullterm_slice().as_slice_with_nullchar();
    }
};

struct OsString {
    VAL_STRUCT(OsString, s, NullTermSlice<char>())

    static OsString init(const NullTermSlice<char> &_s) {
        return {_s};
    }
    static OsString init(const Slice<char> &_s) {
        return {NullTermSlice<char>::init(_s.len, _s.ptr)};
    }
    static OsString init(usize len_with_sentinel, char *ptr) {
        return OsString::init(NullTermSlice<char>::init(len_with_sentinel, ptr));
    }
    static OsString init(char *ptr) {
        return OsString::init(NullTermSlice<char>::init(strlen(ptr) + 1, ptr));
    }

    operator NullTermSlice<const char>() {
        return NullTermSlice<const char>::init(s.len(), reinterpret_cast<const char *>(s.ptr()));
    }
    operator NullTermSlice<char>() {
        return s;
    }
    const char *operator[](this const OsString &self, usize idx) {
        return self.s.get(idx);
    }
    operator ImOsString() {
        return ImOsString::init(*this);
    }

    bool is_empty(this const OsString &self) {
        return self.s.is_empty();
    }
    usize len(this const OsString &self) {
        return self.s.len();
    }
    usize len_with_nullchar(this const OsString &self) {
        return self.s.len_with_nullchar();
    }
    char *ptr(this OsString &self) {
        return self.s.ptr();
    }

    NullTermSlice<char> as_nullterm_slice(this OsString &self) {
        return self;
    }
    Slice<char> as_slice(this OsString &self) {
        return self.as_nullterm_slice().as_slice();
    }
    Slice<char> as_slice_with_nullchar(this OsString &self) {
        return self.as_nullterm_slice().as_slice_with_nullchar();
    }
};
