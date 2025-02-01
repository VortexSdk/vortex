#pragma once

#include "Array.hpp"
#include "linux/syscall/syscall_impl.hpp"
#include "mem/Slice.hpp"
#include "mem/utils.hpp"
#include "metap/metap.hpp"
#include "strings.hpp"

#ifndef DEFAULT_WRITER_BUF
#define DEFAULT_WRITER_BUF 1024
#endif

struct Writer {
    PIN_STRUCT(Writer, buf, (SliceWithPos<char>()))

    template <typename T>
        requires is_same_v<T, char> || is_same_v<T, u8>
    static Writer init(Slice<T> s) {
        return Writer(SliceWithPos<char>::init(s.len, reinterpret_cast<char*>(s.ptr), 0));
    }

    bool write_is_possible(this Writer& self, usize len) {
        if ((self.buf.pos + len) > self.buf.slice.len) [[unlikely]] {
            return false;
        }

        return true;
    }

    SysRes<None> write_slice(this Writer& self, const Slice<const char>& data) {
        if (!self.write_is_possible(data.len)) return SysResKind::OVERFLOW;

        memcpy(self.buf.slice.ptr + self.buf.pos, data.ptr, data.len);
        self.buf.pos += data.len;

        return None();
    }

    SysRes<None> write_null_terminated(this Writer& self, const char* data) {
        usize len = strlen(data);
        if (!self.write_is_possible(len)) return SysResKind::OVERFLOW;

        memcpy(
            reinterpret_cast<void*>(&self.buf.slice.ptr [self.buf.pos]),
            reinterpret_cast<const void*>(data), len
        );
        self.buf.pos += len;

        return None();
    }

    template <typename T> SysRes<None> write_int(this Writer& self, T num) {
        char temp [32];
        usize idx     = sizeof(temp) - 1;
        temp [idx]    = '\0';

        bool negative = false;
        if constexpr (is_signed_v<T>) {
            if (num < 0) {
                negative = true;
                num      = -num;
            }
        }

        do {
            temp [--idx] = static_cast<char>('0' + (num % 10));
            num /= 10;
        } while (num > 0);

        if (negative) temp [--idx] = '-';

        Slice<const char> s =
            Slice<const char>::init(sizeof(temp) - idx, reinterpret_cast<const char*>(temp + idx));

        return self.write(&s);
    }

    template <typename T> SysRes<None> write_float(this Writer& self, T num, int precision = 10) {
        char temp [320];
        usize idx  = sizeof(temp) - 1;
        temp [idx] = '\0';

        T frac     = num - static_cast<T>(static_cast<long long>(num));
        for (int i = 0; i < precision; i++) {
            frac *= 10;
            temp [--idx] = static_cast<char>('0' + static_cast<int>(frac) % 10);
        }
        temp [--idx] = '.';

        auto ipart   = static_cast<long long>(abs(num));
        if (ipart == 0) temp [--idx] = '0';
        while (ipart > 0) {
            temp [--idx] = static_cast<char>('0' + static_cast<unsigned char>(ipart % 10));
            ipart /= 10;
        }

        if (num < 0) temp [--idx] = '-';

        Slice<const char> s =
            Slice<const char>::init(sizeof(temp) - idx, reinterpret_cast<const char*>(temp + idx));

        return self.write(&s);
    }

    template <typename T> SysRes<None> write(this Writer& self, const T& data) {
        if constexpr (is_same_v<T, Slice<u8>> || is_same_v<T, Slice<const u8>> ||
                      is_same_v<T, const Slice<u8>> || is_same_v<T, const Slice<const u8>> ||
                      is_same_v<T, Slice<char>> || is_same_v<T, Slice<const char>> ||
                      is_same_v<T, const Slice<char>> || is_same_v<T, const Slice<const char>>) {
            return self.write_slice(
                Slice<const char>::init(data.len, reinterpret_cast<const char*>(data.ptr))
            );
        } else if constexpr (is_same_v<T, const char*> || is_same_v<T, char*>) {
            if (data) return self.write_null_terminated(data);
        } else if constexpr (is_integral_v<T>) {
            return self.write_int(data);
        } else if constexpr (is_floating_point_v<T>) {
            return self.write_float(data);
        }
        return SysResKind::INVAL;
    }

    template <usize n>
    SysRes<None> write(this Writer& self, const char (&data) [n])
        requires(n > 1)
    {
        return self.write_slice(Slice<const char>::init(n - 1, reinterpret_cast<const char*>(data))
        );
    }
};

template <usize c = DEFAULT_WRITER_BUF, unsigned int fd = 0> struct FdWriter {
    PIN_STRUCT(FdWriter, writer, Writer(), buf, (Array<char, c>()))

    static FdWriter init() {
        FdWriter<c, fd> self;
        self.writer = Writer::init(self.buf.as_mut_slice());

        return self;
    }

    template <typename T> SysRes<None> write(this FdWriter& self, const T& data) {
        SysRes<None> res = self.writer.write(data);
        if (res.is_ok()) [[likely]]
            return res;

        TRY(self.flush());
        res = self.writer.write(data);
        if (res.is_ok()) [[likely]]
            return res;

        return SysResKind::OVERFLOW;
    }

    SysRes<None> flush(this FdWriter& self) {
        TRY(syscall(__NR_write, fd, self.buf.data, self.writer.buf.pos));
        self.writer.buf.pos = 0;

        return None();
    }
};

template <typename... Args> SysRes<None> bufwrite(const Slice<char>& s, Args&&... args) {
    Writer writer = Writer::init(s);
    TRY((..., (writer.write(forward<Args>(args)))));
    return None();
}

template <typename... Args> SysRes<None> print(Args&&... args) {
    FdWriter<DEFAULT_WRITER_BUF, 0> writer;
    TRY((..., (writer.write(forward<Args>(args)))));
    return writer.flush();
}
template <typename... Args> SysRes<None> eprint(Args&&... args) {
    FdWriter<DEFAULT_WRITER_BUF, 1> writer;
    TRY((..., (writer.write(forward<Args>(args)))));
    return writer.flush();
}
template <typename... Args> SysRes<None> println(Args&&... args) {
    FdWriter<DEFAULT_WRITER_BUF, 0> writer;
    TRY((..., (writer.write(forward<Args>(args)))));

    writer.write("\n");
    return writer.flush();
}
template <typename... Args> SysRes<None> eprintln(Args&&... args) {
    FdWriter<DEFAULT_WRITER_BUF, 1> writer;
    TRY((..., (writer.write(forward<Args>(args)))));

    writer.write("\n");
    return writer.flush();
}
