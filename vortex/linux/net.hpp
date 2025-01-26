#pragma once

#include "../mem/utils.hpp"
#include "../numbers.hpp"
#include "syscall/SysRes.hpp"
#include "syscall/wrapper.hpp"
#include "syscall/wrapperHelper.hpp"
#include <asm-generic/socket.h>
#include <linux/in.h>

enum {
    AF_INET        = 2,        // IPv4
    AF_INET6       = 10,       // IPv6
    AF_UNIX        = 1,        // Local unix domain
    AF_UNSPEC      = 0,        // Unspecified
    AF_NETLINK     = 16,       // Netlink sockets
    SOCK_STREAM    = 1,        // Sequenced, reliable, connection-based stream
    SOCK_DGRAM     = 2,        // Connectionless, unreliable datagrams
    SOCK_RAW       = 3,        // Raw protocol interface
    SOCK_SEQPACKET = 5,
    SOCK_NONBLOCK  = 04000,    // Nonblocking mode
    SOCK_CLOEXEC   = 02000000, // Set close-on-exec flag
    MSG_PEEK       = 2,        // Peek at incoming message
    MSG_WAITALL    = 0x100,    // Wait for full request or error
    MSG_TRUNC      = 32,
    SHUT_RD        = 0,
    SHUT_WR        = 1,
    SHUT_RDWR      = 2,
};

// Only little endian is supported.
static u16 htons(u16 x) {
    return static_cast<u16>((x << 8_u16) | (x >> 8_u16));
}
static u16 ntohs(u16 x) {
    return static_cast<u16>((x << 8_u16) | (x >> 8_u16));
}
static u32 htonl(u32 x) {
    return static_cast<u32>(
        ((x & 0xff000000_u32) >> 24_u32) | ((x & 0x00ff0000_u32) >> 8_u32) |
        ((x & 0x0000ff00_u32) << 8_u32) | ((x & 0x000000ff_u32) << 24_u32)
    );
}
static u32 ntohl(u32 x) {
    return static_cast<u32>(
        ((x & 0xff000000_u32) >> 24_u32) | ((x & 0x00ff0000_u32) >> 8_u32) |
        ((x & 0x0000ff00_u32) << 8_u32) | ((x & 0x000000ff_u32) << 24_u32)
    );
}
static usize inet_ntop4(const u8* src, char* dst, usize size) {
    if (size < 16) return 0;

    char* buf = dst;
    for (u8 i = 0; i < 4; i++) {
        u8 val      = src [i];

        u8 hundreds = static_cast<u8>(val / 100);
        if (hundreds) *buf++ = static_cast<char>(hundreds + '0');

        val     = static_cast<u8>(val % 100);
        u8 tens = static_cast<u8>(val / 10);
        if (tens || hundreds) *buf++ = static_cast<char>(tens + '0');

        val    = static_cast<u8>(val % 10);
        *buf++ = static_cast<char>(val + '0');

        if (i < 3) *buf++ = '.';
    }
    *buf = '\0';

    return static_cast<usize>(buf - dst);
}

static usize write_hex16(char* dst, u16 val) {
    static const char hex_chars [] = "0123456789abcdef";
    char* start                    = dst;
    bool skip_zeros                = true;

    for (int i = 12; i >= 0; i -= 4) {
        u8 digit = static_cast<u8>((val >> static_cast<u16>(i)) & 0xF_u16);
        if (digit || !skip_zeros || i == 0) {
            *dst++     = hex_chars [digit];
            skip_zeros = false;
        }
    }

    return static_cast<usize>(dst - start);
}

static usize inet_ntop6(const u8* src, char* dst, usize size) {
    if (size < 46) return 0;

    const u16* src16 = reinterpret_cast<const u16*>(src);
    char* buf        = dst;

    int zero_start   = -1;
    int zero_len     = 0;
    int cur_start    = -1;
    int cur_len      = 0;

    for (int i = 0; i < 8; i++) {
        if (ntohs(src16 [i]) == 0) {
            if (cur_start < 0) cur_start = i;
            cur_len++;
        } else if (cur_start >= 0) {
            if (cur_len > zero_len) {
                zero_len   = cur_len;
                zero_start = cur_start;
            }
            cur_start = -1;
            cur_len   = 0;
        }
    }

    if (cur_len > zero_len) {
        zero_len   = cur_len;
        zero_start = cur_start;
    }

    int words_before = zero_start;
    int words_after  = 8 - (zero_start + zero_len);
    bool need_sep    = false;

    if (zero_start == 0) {
        *buf++ = ':';
    }

    for (int i = 0; i < words_before; i++) {
        if (need_sep) *buf++ = ':';
        buf += static_cast<usize>(write_hex16(buf, ntohs(src16 [i])));
        need_sep = true;
    }

    if (zero_len > 0) {
        *buf++ = ':';
        if (words_after == 0) *buf++ = ':';
    }

    for (int i = zero_start + zero_len; i < 8; i++) {
        if (need_sep) *buf++ = ':';
        buf += static_cast<usize>(write_hex16(buf, ntohs(src16 [i])));
        need_sep = true;
    }

    *buf = '\0';
    return static_cast<usize>(buf - dst);
}
static bool inet_pton4(const char* src, u8* dst) {
    u8 tmp [4];
    u8* tp = tmp;
    u32 val;
    int ch;

    for (;;) {
        val        = 0;
        int digits = 0;
        while ((ch = static_cast<int>(*src++)) != '\0') {
            if (ch >= '0' && ch <= '9') {
                if (++digits > 3 || val > 255_u32) return false;
                val = (val * 10_u32) + static_cast<u32>(ch - '0');
                continue;
            }
            break;
        }
        if (digits == 0 || val > 255_u32) return false;
        *tp++ = static_cast<u8>(val);

        if (ch == '\0' && tp == tmp + 4) {
            memcpy(dst, tmp, sizeof(tmp));
            return true;
        }
        if (ch != '.' || tp >= tmp + 4) return false;
    }
}
static bool inet_pton6(const char* src, u8* dst) {
    u16 tmp [8];
    u16* tp          = tmp;
    u16 val          = 0;
    u16* endp        = nullptr;
    int seen_xdigits = 0;
    int ch;
    bool has_zero_compression = false;

    memset(tmp, 0, sizeof(tmp));

    if (*src == ':') {
        if (*++src != ':') return false;
    }

    while ((ch = *src++) != '\0') {
        if (ch == ':') {
            if (!seen_xdigits) {
                if (has_zero_compression) return false;
                has_zero_compression = true;
                endp                 = tp;
                continue;
            }
            if (*src == '\0') break;
            seen_xdigits = 0;
            if (tp >= tmp + 8) return false;
            *tp++ = val;
            val   = 0;
            continue;
        }

        ch |= 0x20;
        int digit = (ch >= 'a') ? ch - 'a' + 10 : ch - '0';
        if (digit >= 16) return false;
        val          = static_cast<u16>((val << 4) | static_cast<u16>(digit));
        seen_xdigits = 1;
    }

    if (seen_xdigits) {
        if (tp >= tmp + 8) return false;
        *tp++ = val;
    }

    if (has_zero_compression) {
        if (!endp) return false;
        const auto n = static_cast<usize>(tp - endp);
        const auto m = static_cast<usize>(8 - (endp - tmp));
        for (usize i = 1; i <= n; i++) {
            endp [m - i] = endp [n - i];
            endp [n - i] = 0;
        }
        tp = tmp + 8;
    }
    if (tp != tmp + 8) return false;

    for (unsigned short& i : tmp) i = htons(i);

    memcpy(dst, tmp, sizeof(tmp));
    return true;
}
static bool inet_pton(int af, const char* src, void* dst) {
    switch (af) {
    case AF_INET :
        return inet_pton4(src, reinterpret_cast<u8*>(dst));
    case AF_INET6 :
        return inet_pton6(src, reinterpret_cast<u8*>(dst));
    default :
        return false;
    }
}
static usize inet_ntop(int af, const void* src, char* dst, usize size) {
    switch (af) {
    case AF_INET :
        return inet_ntop4(reinterpret_cast<const u8*>(src), dst, size);
    case AF_INET6 :
        return inet_ntop6(reinterpret_cast<const u8*>(src), dst, size);
    default :
        return 0;
    }
}

static sockaddr_in
init_sockaddr_in(int domain = AF_INET, u16 port = 8080, const char* addr = null) {
    auto s       = zeroed<sockaddr_in>();
    s.sin_port   = htons(port);
    s.sin_family = static_cast<__kernel_sa_family_t>(domain);
    if (addr != null) inet_pton(domain, addr, reinterpret_cast<void*>(&s.sin_addr));
    return s;
}

static SysRes<Fd> init_tcp_server(
    sockaddr* sockaddrin, int sockopts = SO_REUSEADDR | SO_REUSEPORT, bool ipv6 = false,
    int max_queue = 4096
) {
    int domain   = !ipv6 ? AF_INET : AF_INET6;
    Fd socket_fd = TRY(socket(domain, SOCK_STREAM, 0));

    int opt      = 1;
    SysRes<usize> sockopt_r =
        setsockopt(socket_fd, SOL_SOCKET, sockopts, reinterpret_cast<char*>(&opt), sizeof(opt));
    if (sockopt_r.is_err()) {
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        return ErrSysRes(sockopt_r.kind);
    }

    SysRes<usize> bind_r = bind(socket_fd, sockaddrin, sizeof(sockaddr_in));
    if (bind_r.is_err()) {
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        return ErrSysRes(bind_r.kind);
    }

    SysRes<usize> listen_r = listen(socket_fd, max_queue);
    if (listen_r.is_err()) {
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        return ErrSysRes(listen_r.kind);
    }

    return socket_fd;
}

static SysRes<Fd> init_tcp_client(u16 port, const char* addr = null, bool ipv6 = false) {
    int domain            = !ipv6 ? AF_INET : AF_INET6;
    Fd socket_fd          = TRY(socket(domain, SOCK_STREAM, 0));

    sockaddr_in sock_addr = init_sockaddr_in(domain, port, addr);
    SysRes<usize> connect_r =
        connect(socket_fd, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sockaddr_in));
    if (connect_r.is_err()) {
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        return ErrSysRes(connect_r.kind);
    }

    return socket_fd;
}
