#pragma once

#include "Array.hpp"
#include "linux/syscall/syscall_impl.hpp"
#include "mem/utils.hpp"
#include "metap/metap.hpp"
#include "strings.hpp"

#ifndef DEFAULT_WRITER_BUF
#define DEFAULT_WRITER_BUF 1024
#endif

template <usize c = DEFAULT_WRITER_BUF, unsigned int fd = 0> struct BufWriter {
    usize pos{0};
    Array<char, c> buf;

    BufWriter() : buf(Array<char, c>()) {}

    SysRes<None> write_slice(this BufWriter& self, const Slice<const u8>* data) {
        if (data->len > c) [[unlikely]]
            return SysResKind::OVERFLOW;
        if ((self.pos + data->len) > c) TRY(self.flush());

        memcpy(self.buf.data + self.pos, data->ptr, data->len);
        self.pos += data->len;
        return None();
    }

    SysRes<None> write_null_terminated(this BufWriter& self, const char* data) {
        usize len = strlen(data);
        if (len > c) [[unlikely]]
            return SysResKind::OVERFLOW;
        if (self.pos + len > c) TRY(self.flush());

        memcpy(
            reinterpret_cast<void*>(&self.buf.data [self.pos]), reinterpret_cast<const void*>(data),
            len
        );
        self.pos += len;
        return None();
    }

    template <typename T> SysRes<None> write_int(this BufWriter& self, T num) {
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

        Slice<const u8> s =
            Slice<const u8>::init(sizeof(temp) - idx, reinterpret_cast<const u8*>(temp + idx));
        return self.write(&s);
    }

    template <typename T>
    SysRes<None> write_float(this BufWriter& self, T num, int precision = 10) {
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

        Slice<const u8> s =
            Slice<const u8>::init(sizeof(temp) - idx, reinterpret_cast<const u8*>(temp + idx));
        return self.write(&s);
    }

    template <typename T> SysRes<None> write(this BufWriter& self, T data) {
        if constexpr (is_same_v<T, Slice<u8>*> || is_same_v<T, Slice<const u8>*> ||
                      is_same_v<T, const Slice<u8>*> || is_same_v<T, const Slice<const u8>*> ||
                      is_same_v<T, Slice<char>*> || is_same_v<T, Slice<const char>*> ||
                      is_same_v<T, const Slice<char>*> || is_same_v<T, const Slice<const char>*>) {
            Slice<const u8> s = Slice<const u8>::init(data->len, data->ptr);
            return self.write_slice(&s);
        } else if constexpr (is_same_v<T, const char*> || is_same_v<T, char*>) {
            if (data) return self.write_null_terminated(move(data));
        } else if constexpr (is_integral_v<T>) {
            return self.write_int(data);
        } else if constexpr (is_floating_point_v<T>) {
            return self.write_float(data);
        }
        return SysResKind::INVAL;
    }

    template <usize n>
    SysRes<None> write(this BufWriter& self, const char (&data) [n])
        requires(n != 0)
    {
        Slice<const u8> s = Slice<const u8>::init(n, reinterpret_cast<const u8*>(data));
        return self.write_slice(&s);
    }

    SysRes<None> flush(this BufWriter& self) {
        TRY(syscall(__NR_write, fd, self.buf.data, self.pos));
        self.pos = 0;
        return None();
    }
};

template <typename... Args> SysRes<None> print(Args&&... args) {
    BufWriter<DEFAULT_WRITER_BUF, 0> writer;
    (..., (writer.write(forward<Args>(args))));
    return writer.flush();
}
template <typename... Args> SysRes<None> eprint(Args&&... args) {
    BufWriter<DEFAULT_WRITER_BUF, 1> writer;
    (..., (writer.write(forward<Args>(args))));
    return writer.flush();
}
template <typename... Args> SysRes<None> println(Args&&... args) {
    BufWriter<DEFAULT_WRITER_BUF, 0> writer;
    (..., (writer.write(forward<Args>(args))));

    writer.write("\n");
    return writer.flush();
}
template <typename... Args> SysRes<None> eprintln(Args&&... args) {
    BufWriter<DEFAULT_WRITER_BUF, 1> writer;
    (..., (writer.write(forward<Args>(args))));

    writer.write("\n");
    return writer.flush();
}
