/*
 * Copyright (c) 2009-2014 Kazuho Oku, Tokuhiro Matsuno, Daisuke Murase,
 *                         Shigeo Mitsunari
 *
 * The software is licensed under either the MIT License (below) or the Perl
 * license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include "debug.hpp"
#include "mem/utils.hpp"
#include "numbers.hpp"

/// contains name and value of a header (name == null if is a continuing line of a multiline header
struct PhrHeader {
    Slice<const char> name{};
    Slice<const char> value{};
};

/// should be zero-filled before start
struct PhrChunkedDecoder {
    usize bytes_left_in_chunk{0}; /* number of bytes left in current chunk */
    char consume_trailer{0};      /* if trailing headers should be consumed */
    char _hex_count{0};
    char _state{0};
    u64 _total_read{0};
    u64 _total_overhead{0};

    int phr_decode_chunked_is_in_data(this PhrChunkedDecoder &self);
    isize phr_decode_chunked(this PhrChunkedDecoder &self, Slice<char> *buf);
};

namespace __http {
#ifdef __SSE4_2__
#ifdef _MSC_VER
#include <nmmintrin.h>
#else
#include <x86intrin.h>
#endif
#endif

#if __GNUC__ >= 3
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

#ifdef _MSC_VER
#define ALIGNED(n) _declspec(align(n))
#else
#define ALIGNED(n) __attribute__((aligned(n)))
#endif

#define IS_PRINTABLE_ASCII(c) ((unsigned char)(c) - 040u < 0137u)

#define CHECK_EOF()                                                                                \
    if (buf == buf_end) {                                                                          \
        *ret = -2;                                                                                 \
        return null;                                                                               \
    }

#define EXPECT_CHAR_NO_CHECK(ch)                                                                   \
    if (*buf++ != ch) {                                                                            \
        *ret = -1;                                                                                 \
        return null;                                                                               \
    }

#define EXPECT_CHAR(ch)                                                                            \
    CHECK_EOF()                                                                                    \
    EXPECT_CHAR_NO_CHECK(ch)

#define ADVANCE_TOKEN(tok, toklen)                                                                 \
    do {                                                                                           \
        const char *tok_start                      = buf;                                          \
        static const char ALIGNED(16) ranges2 [16] = "\000\040\177\177";                           \
        int found2;                                                                                \
        buf = findchar_fast(buf, buf_end, ranges2, 4, &found2);                                    \
        if (!found2) {                                                                             \
            CHECK_EOF()                                                                            \
        }                                                                                          \
        while (1) {                                                                                \
            if (*buf == ' ') {                                                                     \
                break;                                                                             \
            }                                                                                      \
            if (unlikely(!IS_PRINTABLE_ASCII(*buf))) {                                             \
                if ((unsigned char)*buf < '\040' || *buf == '\177') {                              \
                    *ret = -1;                                                                     \
                    return null;                                                                   \
                }                                                                                  \
            }                                                                                      \
            ++buf;                                                                                 \
            CHECK_EOF()                                                                            \
        }                                                                                          \
        tok    = tok_start;                                                                        \
        toklen = (usize)(buf - tok_start);                                                         \
    } while (0)

static const char *token_char_map =
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\1\0\1\1\1\1\1\0\0\1\1\0\1\1\0\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"
    "\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\1\1"
    "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\0\1\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static const char *findchar_fast(
    const char *buf, const char *buf_end, const char *ranges, usize ranges_size, int *found
) {
    *found = 0;
#if __SSE4_2__
    if (likely(buf_end - buf >= 16)) {
        __m128i ranges16 = _mm_loadu_si128((const __m128i *)ranges);

        usize left       = (buf_end - buf) & ~15;
        do {
            __m128i b16 = _mm_loadu_si128((const __m128i *)buf);
            int r       = _mm_cmpestri(
                ranges16, ranges_size, b16, 16,
                _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS
            );
            if (unlikely(r != 16)) {
                buf += r;
                *found = 1;
                break;
            }
            buf += 16;
            left -= 16;
        } while (likely(left != 0));
    }
#else
    /* suppress unused parameter warning */
    (void)buf_end;
    (void)ranges;
    (void)ranges_size;
#endif
    return buf;
}

static const char *get_token_to_eol(
    const char *buf, const char *buf_end, const char **token, usize *token_len, int *ret
) {
    const char *token_start = buf;

#ifdef __SSE4_2__
    static const char ALIGNED(16) ranges1 [16] =
        "\0\010"    /* allow HT */
        "\012\037"  /* allow SP and up to but not including DEL */
        "\177\177"; /* allow chars w. MSB set */
    int found;
    buf = findchar_fast(buf, buf_end, ranges1, 6, &found);
    if (found) goto FOUND_CTL;
#else
    /* find non-printable char within the next 8 bytes, this is the hottest code; manually
     * inlined
     */
    while (likely(buf_end - buf >= 8)) {
#define DOIT()                                                                                     \
    do {                                                                                           \
        if (unlikely(!IS_PRINTABLE_ASCII(*buf))) goto NonPrintable;                                \
        ++buf;                                                                                     \
    } while (0)
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
#undef DOIT
        continue;
    NonPrintable:
        if ((likely((unsigned char)*buf < '\040') && likely(*buf != '\011')) ||
            unlikely(*buf == '\177')) {
            goto FOUND_CTL;
        }
        ++buf;
    }
#endif
    for (;; ++buf) {
        CHECK_EOF()
        if (unlikely(!IS_PRINTABLE_ASCII(*buf))) {
            if ((likely((unsigned char)*buf < '\040') && likely(*buf != '\011')) ||
                unlikely(*buf == '\177')) {
                goto FOUND_CTL;
            }
        }
    }
FOUND_CTL:
    if (likely(*buf == '\015')) {
        ++buf;
        EXPECT_CHAR('\012')
        *token_len = (usize)(buf - 2 - token_start);
    } else if (*buf == '\012') {
        *token_len = (usize)(buf - token_start);
        ++buf;
    } else {
        *ret = -1;
        return null;
    }
    *token = token_start;

    return buf;
}

static const char *is_complete(const char *buf, const char *buf_end, usize last_len, int *ret) {
    int ret_cnt = 0;
    buf         = last_len < 3 ? buf : buf + last_len - 3;

    while (true) {
        CHECK_EOF()
        if (*buf == '\015') {
            ++buf;
            CHECK_EOF()
            EXPECT_CHAR('\012')
            ++ret_cnt;
        } else if (*buf == '\012') {
            ++buf;
            ++ret_cnt;
        } else {
            ++buf;
            ret_cnt = 0;
        }
        if (ret_cnt == 2) {
            return buf;
        }
    }

    // Will never be executed!
    // *ret = -2;
    // return null;
}

#define PARSE_INT(valp_, mul_)                                                                     \
    if (*buf < '0' || '9' < *buf) {                                                                \
        buf++;                                                                                     \
        *ret = -1;                                                                                 \
        return null;                                                                               \
    }                                                                                              \
    *(valp_) = (mul_) * (*buf++ - '0');

#define PARSE_INT_3(valp_)                                                                         \
    do {                                                                                           \
        int res_ = 0;                                                                              \
        PARSE_INT(&res_, 100)                                                                      \
        *valp_ = res_;                                                                             \
        PARSE_INT(&res_, 10)                                                                       \
        *valp_ += res_;                                                                            \
        PARSE_INT(&res_, 1)                                                                        \
        *valp_ += res_;                                                                            \
    } while (0)

/* returned pointer is always within [buf, buf_end), or null */
static const char *parse_token(
    const char *buf, const char *buf_end, const char **token, usize *token_len, char next_char,
    int *ret
) {
    /* We use pcmpestri to detect non-token characters. This instruction can take no more than
     * eight character ranges (8*2*8=128 bits that is the size of a SSE register). Due to this
     * restriction, characters `|` and `~` are handled in the slow loop. */
    static const char ALIGNED(16) ranges [] = "\x00 "  /* control chars and up to SP */
                                              "\"\""   /* 0x22 */
                                              "()"     /* 0x28,0x29 */
                                              ",,"     /* 0x2c */
                                              "//"     /* 0x2f */
                                              ":@"     /* 0x3a-0x40 */
                                              "[]"     /* 0x5b-0x5d */
                                              "{\xff"; /* 0x7b-0xff */
    const char *buf_start = buf;
    int found;
    buf = findchar_fast(buf, buf_end, ranges, sizeof(ranges) - 1, &found);
    if (!found) {
        CHECK_EOF()
    }
    while (true) {
        if (*buf == next_char) {
            break;
        }
        if (!token_char_map [(unsigned char)*buf]) {
            *ret = -1;
            return null;
        }
        ++buf;
        CHECK_EOF()
    }
    *token     = buf_start;
    *token_len = (usize)(buf - buf_start);
    return buf;
}

/* returned pointer is always within [buf, buf_end), or null */
static const char *
parse_http_version(const char *buf, const char *buf_end, int *minor_version, int *ret) {
    /* we want at least [HTTP/1.<two chars>] to try to parse */
    if (buf_end - buf < 9) {
        *ret = -2;
        return null;
    }
    EXPECT_CHAR_NO_CHECK('H')
    EXPECT_CHAR_NO_CHECK('T')
    EXPECT_CHAR_NO_CHECK('T')
    EXPECT_CHAR_NO_CHECK('P')
    EXPECT_CHAR_NO_CHECK('/')
    EXPECT_CHAR_NO_CHECK('1')
    EXPECT_CHAR_NO_CHECK('.')
    PARSE_INT(minor_version, 1)
    return buf;
}

static const char *parse_headers(
    const char *buf, const char *buf_end, PhrHeader *headers, usize *num_headers, usize max_headers,
    int *ret
) {
    for (;; ++*num_headers) {
        CHECK_EOF()
        if (*buf == '\015') {
            ++buf;
            EXPECT_CHAR('\012')
            break;
        }
        if (*buf == '\012') {
            ++buf;
            break;
        }
        if (*num_headers == max_headers) {
            *ret = -1;
            return null;
        }
        if (!(*num_headers != 0 && (*buf == ' ' || *buf == '\t'))) {
            /* parsing name, but do not discard SP before colon, see
             * http://www.mozilla.org/security/announce/2006/mfsa2006-33.html */
            if ((buf = parse_token(
                     buf, buf_end, &headers [*num_headers].name.ptr,
                     &headers [*num_headers].name.len, ':', ret
                 )) == null) {
                return null;
            }
            if (headers [*num_headers].name.len == 0) {
                *ret = -1;
                return null;
            }
            ++buf;
            for (;; ++buf) {
                CHECK_EOF()
                if (!(*buf == ' ' || *buf == '\t')) {
                    break;
                }
            }
        } else {
            headers [*num_headers].name.ptr = null;
            headers [*num_headers].name.len = 0;
        }
        const char *value;
        usize value_len;
        if ((buf = get_token_to_eol(buf, buf_end, &value, &value_len, ret)) == null) {
            return null;
        }
        /* remove trailing SPs and HTABs */
        const char *value_end = value + value_len;
        for (; value_end != value; --value_end) {
            const char c = *(value_end - 1);
            if (!(c == ' ' || c == '\t')) {
                break;
            }
        }
        headers [*num_headers].value.ptr = value;
        headers [*num_headers].value.len = (usize)(value_end - value);
    }
    return buf;
}

static const char *parse_request(
    const char *buf, const char *buf_end, const char **method, usize *method_len, const char **path,
    usize *path_len, int *minor_version, PhrHeader *headers, usize *num_headers, usize max_headers,
    int *ret
) {
    /* skip first empty line (some clients add CRLF after POST content) */
    CHECK_EOF()
    if (*buf == '\015') {
        ++buf;
        EXPECT_CHAR('\012')
    } else if (*buf == '\012') {
        ++buf;
    }

    /* parse request line */
    if ((buf = parse_token(buf, buf_end, method, method_len, ' ', ret)) == null) {
        return null;
    }
    do {
        ++buf;
        CHECK_EOF()
    } while (*buf == ' ');
    ADVANCE_TOKEN(*path, *path_len);
    do {
        ++buf;
        CHECK_EOF()
    } while (*buf == ' ');
    if (*method_len == 0 || *path_len == 0) {
        *ret = -1;
        return null;
    }
    if ((buf = parse_http_version(buf, buf_end, minor_version, ret)) == null) {
        return null;
    }
    if (*buf == '\015') {
        ++buf;
        EXPECT_CHAR('\012')
    } else if (*buf == '\012') {
        ++buf;
    } else {
        *ret = -1;
        return null;
    }

    return parse_headers(buf, buf_end, headers, num_headers, max_headers, ret);
}

/* returns number of bytes consumed if successful, -2 if request is partial,
 * -1 if failed */
static int phr_parse_request(
    const char *buf_start, usize len, const char **method, usize *method_len, const char **path,
    usize *path_len, int *minor_version, PhrHeader *headers, usize *num_headers, usize last_len
) {
    const char *buf = buf_start, *buf_end = buf_start + len;
    usize max_headers = *num_headers;
    int r;

    *method        = null;
    *method_len    = 0;
    *path          = null;
    *path_len      = 0;
    *minor_version = -1;
    *num_headers   = 0;

    /* if last_len != 0, check if the request is complete (a fast countermeasure
       againt slowloris */
    if (last_len != 0 && is_complete(buf, buf_end, last_len, &r) == null) {
        return r;
    }

    if ((buf = parse_request(
             buf, buf_end, method, method_len, path, path_len, minor_version, headers, num_headers,
             max_headers, &r
         )) == null) {
        return r;
    }

    return (int)(buf - buf_start);
}

static const char *parse_response(
    const char *buf, const char *buf_end, int *minor_version, int *status, const char **msg,
    usize *msg_len, PhrHeader *headers, usize *num_headers, usize max_headers, int *ret
) {
    /* parse "HTTP/1.x" */
    if ((buf = parse_http_version(buf, buf_end, minor_version, ret)) == null) {
        return null;
    }
    /* skip space */
    if (*buf != ' ') {
        *ret = -1;
        return null;
    }
    do {
        ++buf;
        CHECK_EOF()
    } while (*buf == ' ');
    /* parse status code, we want at least [:digit:][:digit:][:digit:]<other char> to try to
     * parse
     */
    if (buf_end - buf < 4) {
        *ret = -2;
        return null;
    }
    PARSE_INT_3(status);

    /* get message including preceding space */
    if ((buf = get_token_to_eol(buf, buf_end, msg, msg_len, ret)) == null) {
        return null;
    }
    if (*msg_len == 0) {
        /* ok */
    } else if (**msg == ' ') {
        /* Remove preceding space. Successful return from `get_token_to_eol` guarantees that we
         * would hit something other than SP before running past the end of the given buffer. */
        do {
            ++*msg;
            --*msg_len;
        } while (**msg == ' ');
    } else {
        /* garbage found after status code */
        *ret = -1;
        return null;
    }

    return parse_headers(buf, buf_end, headers, num_headers, max_headers, ret);
}

/* ditto */
static int phr_parse_response(
    const char *buf_start, usize len, int *minor_version, int *status, const char **msg,
    usize *msg_len, PhrHeader *headers, usize *num_headers, usize last_len
) {
    const char *buf = buf_start, *buf_end = buf + len;
    usize max_headers = *num_headers;
    int r;

    *minor_version = -1;
    *status        = 0;
    *msg           = null;
    *msg_len       = 0;
    *num_headers   = 0;

    /* if last_len != 0, check if the response is complete (a fast countermeasure
       against slowloris */
    if (last_len != 0 && is_complete(buf, buf_end, last_len, &r) == null) {
        return r;
    }

    if ((buf = parse_response(
             buf, buf_end, minor_version, status, msg, msg_len, headers, num_headers, max_headers,
             &r
         )) == null) {
        return r;
    }

    return (int)(buf - buf_start);
}

/* ditto */
static int phr_parse_headers(
    const char *buf_start, usize len, PhrHeader *headers, usize *num_headers, usize last_len
) {
    const char *buf = buf_start, *buf_end = buf + len;
    usize max_headers = *num_headers;
    int r;

    *num_headers = 0;

    /* if last_len != 0, check if the response is complete (a fast countermeasure
       against slowloris */
    if (last_len != 0 && is_complete(buf, buf_end, last_len, &r) == null) {
        return r;
    }

    if ((buf = parse_headers(buf, buf_end, headers, num_headers, max_headers, &r)) == null) {
        return r;
    }

    return (int)(buf - buf_start);
}

enum : u8 {
    CHUNKED_IN_CHUNK_SIZE,
    CHUNKED_IN_CHUNK_EXT,
    CHUNKED_IN_CHUNK_DATA,
    CHUNKED_IN_CHUNK_CRLF,
    CHUNKED_IN_TRAILERS_LINE_HEAD,
    CHUNKED_IN_TRAILERS_LINE_MIDDLE
};

static int decode_hex(int ch) {
    if ('0' <= ch && ch <= '9') {
        return ch - '0';
    }
    if ('A' <= ch && ch <= 'F') {
        return ch - 'A' + 0xa;
    }
    if ('a' <= ch && ch <= 'f') {
        return ch - 'a' + 0xa;
    }
    return -1;
}

/* the function rewrites the buffer given as (buf, bufsz) removing the chunked-
 * encoding headers.  When the function returns without an error, bufsz is
 * updated to the length of the decoded data available.  Applications should
 * repeatedly call the function while it returns -2 (incomplete) every time
 * supplying newly arrived data.  If the end of the chunked-encoded data is
 * found, the function returns a non-negative number indicating the number of
 * octets left undecoded, that starts from the offset returned by `*bufsz`.
 * Returns -1 on error.
 */
static isize phr_decode_chunked(PhrChunkedDecoder *decoder, char *buf, usize *_bufsz) {
    usize dst = 0, src = 0, bufsz = *_bufsz;
    isize ret = -2; /* incomplete */

    decoder->_total_read += bufsz;

    while (true) {
        switch (decoder->_state) {
        case CHUNKED_IN_CHUNK_SIZE :
            for (;; ++src) {
                int v;
                if (src == bufsz) goto Exit;
                if ((v = decode_hex(buf [src])) == -1) {
                    if (decoder->_hex_count == 0) {
                        ret = -1;
                        goto Exit;
                    }
                    /* the only characters that may appear after the chunk size are BWS,
                     * semicolon, or CRLF */
                    switch (buf [src]) {
                    case ' ' :
                    case '\011' :
                    case ';' :
                    case '\012' :
                    case '\015' :
                        break;
                    default :
                        ret = -1;
                        goto Exit;
                    }
                    break;
                }
                if (decoder->_hex_count == sizeof(usize) * 2) {
                    ret = -1;
                    goto Exit;
                }
                decoder->bytes_left_in_chunk = decoder->bytes_left_in_chunk * 16 + (usize)v;
                ++decoder->_hex_count;
            }
            decoder->_hex_count = 0;
            decoder->_state     = CHUNKED_IN_CHUNK_EXT;

            [[fallthrough]];
        case CHUNKED_IN_CHUNK_EXT :
            /* RFC 7230 A.2 "Line folding in chunk extensions is disallowed" */
            for (;; ++src) {
                if (src == bufsz) goto Exit;
                if (buf [src] == '\012') break;
            }
            ++src;
            if (decoder->bytes_left_in_chunk == 0) {
                if (decoder->consume_trailer) {
                    decoder->_state = CHUNKED_IN_TRAILERS_LINE_HEAD;
                    break;
                }
                goto Complete;
            }
            decoder->_state = CHUNKED_IN_CHUNK_DATA;

            [[fallthrough]];
        case CHUNKED_IN_CHUNK_DATA : {
            usize avail = bufsz - src;
            if (avail < decoder->bytes_left_in_chunk) {
                if (dst != src) memmove(buf + dst, buf + src, avail);
                src += avail;
                dst += avail;
                decoder->bytes_left_in_chunk -= avail;
                goto Exit;
            }
            if (dst != src) memmove(buf + dst, buf + src, decoder->bytes_left_in_chunk);
            src += decoder->bytes_left_in_chunk;
            dst += decoder->bytes_left_in_chunk;
            decoder->bytes_left_in_chunk = 0;
            decoder->_state              = CHUNKED_IN_CHUNK_CRLF;
        }

            [[fallthrough]];
        case CHUNKED_IN_CHUNK_CRLF :
            for (;; ++src) {
                if (src == bufsz) goto Exit;
                if (buf [src] != '\015') break;
            }
            if (buf [src] != '\012') {
                ret = -1;
                goto Exit;
            }
            ++src;
            decoder->_state = CHUNKED_IN_CHUNK_SIZE;
            break;
        case CHUNKED_IN_TRAILERS_LINE_HEAD :
            for (;; ++src) {
                if (src == bufsz) goto Exit;
                if (buf [src] != '\015') break;
            }
            if (buf [src++] == '\012') goto Complete;
            decoder->_state = CHUNKED_IN_TRAILERS_LINE_MIDDLE;

            [[fallthrough]];
        case CHUNKED_IN_TRAILERS_LINE_MIDDLE :
            for (;; ++src) {
                if (src == bufsz) goto Exit;
                if (buf [src] == '\012') break;
            }
            ++src;
            decoder->_state = CHUNKED_IN_TRAILERS_LINE_HEAD;
            break;
        default :
            assert(false, "decoder is corrupt");
        }
    }

Complete:
    ret = (isize)(bufsz - src);
Exit:
    if (dst != src) memmove(buf + dst, buf + src, bufsz - src);
    *_bufsz = dst;
    /* if incomplete but the overhead of the chunked encoding is >=100KB and >80%, signal an
     * error
     */
    if (ret == -2) {
        decoder->_total_overhead += bufsz - dst;
        if (decoder->_total_overhead >= 100 * 1024 &&
            decoder->_total_read - decoder->_total_overhead < decoder->_total_read / 4)
            ret = -1;
    }
    return ret;
}

/* returns if the chunked decoder is in middle of chunked data */
static int phr_decode_chunked_is_in_data(PhrChunkedDecoder *decoder) {
    return decoder->_state == CHUNKED_IN_CHUNK_DATA;
}

#undef CHECK_EOF
#undef EXPECT_CHAR
#undef ADVANCE_TOKEN
} // namespace __http

/// Returns number of bytes consumed if successful, -2 if request is partial, -1 if failed. */
static int phr_parse_request(
    const Slice<const char> &buf, Slice<const char> *method, Slice<const char> *path,
    int *minor_version, Slice<PhrHeader> *headers, usize last_len
) {
    return __http::phr_parse_request(
        buf.ptr, buf.len, &method->ptr, &method->len, &path->ptr, &path->len, minor_version,
        headers->ptr, &headers->len, last_len
    );
}

/// Ditto.
static int phr_parse_response(
    const Slice<const char> &buf, int *minor_version, int *status, Slice<const char> *msg,
    Slice<PhrHeader> headers, usize last_len
) {
    return __http::phr_parse_response(
        buf.ptr, buf.len, minor_version, status, &msg->ptr, &msg->len, headers.ptr, &headers.len,
        last_len
    );
}
/// Ditto.
static int
phr_parse_headers(const Slice<const char> &buf, Slice<PhrHeader> headers, usize last_len) {
    return __http::phr_parse_headers(buf.ptr, buf.len, headers.ptr, &headers.len, last_len);
}

/// The function rewrites the buffer given as Slice<char> removing the chunked-
/// encoding headers.  When the function returns without an error, buf.len is
/// updated to the length of the decoded data available.  Applications should
/// repeatedly call the function while it returns -2 (incomplete) every time
/// supplying newly arrived data.  If the end of the chunked-encoded data is
/// found, the function returns a non-negative number indicating the number of
/// octets left undecoded, that starts from the offset returned by `*buf.len`.
/// Returns -1 on error.
isize PhrChunkedDecoder::phr_decode_chunked(this PhrChunkedDecoder &self, Slice<char> *buf) {
    return __http::phr_decode_chunked(&self, buf->ptr, &buf->len);
}
/// Returns if the chunked decoder is in middle of chunked data.
int PhrChunkedDecoder::phr_decode_chunked_is_in_data(this PhrChunkedDecoder &self) {
    return __http::phr_decode_chunked_is_in_data(&self);
}
