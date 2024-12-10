#pragma once

#include "../linux/io_uring.hpp"
#include "../linux/syscall/SysRes.hpp"
#include "../linux/syscall/wrapperHelper.hpp"
#include "../mem/Allocator.hpp"
#include "../mem/utils.hpp"
#include "../metap/structs.hpp"
#include "../numbers.hpp"
#include "vortex/writer.hpp"

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
        const usize n = static_cast<usize>(tp - endp);
        const usize m = static_cast<usize>(8 - (endp - tmp));
        for (usize i = 1; i <= n; i++) {
            endp [m - i] = endp [n - i];
            endp [n - i] = 0;
        }
        tp = tmp + 8;
    }
    if (tp != tmp + 8) return false;

    for (int i = 0; i < 8; i++) {
        tmp [i] = htons(tmp [i]);
    }
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

enum class SocketDomain {
    Ipv4,
    Ipv6,
    Unix,
    Netlink,
};
enum class SocketType {
    TCP,
    UDP,
    Raw,
    SCTP,
};

struct SocketConfig {
    VAL_STRUCT(
        SocketConfig, domain, SocketDomain::Ipv4, type, SocketType::TCP, addr,
        reinterpret_cast<const char*>(0), port, 8080_u16, max_queued_conns, 4096
    )

    u16 port_to_net(this const SocketConfig& self) {
        return htons(self.port);
    }

    int domain_as_int(this const SocketConfig& self) {
        int d;
        switch (self.domain) {
        case SocketDomain::Ipv4 :
            d = AF_INET;
            break;
        case SocketDomain::Ipv6 :
            d = AF_INET6;
            break;
        case SocketDomain::Unix :
            d = AF_UNIX;
            break;
        case SocketDomain::Netlink :
            d = AF_NETLINK;
            break;
        }
        return d;
    }

    int type_as_int(this const SocketConfig& self) {
        int t;
        switch (self.type) {
        case SocketType::TCP :
            t = SOCK_STREAM;
            break;
        case SocketType::UDP :
            t = SOCK_DGRAM;
            break;
        case SocketType::Raw :
            t = SOCK_RAW;
            break;
        case SocketType::SCTP :
            t = SOCK_SEQPACKET;
            break;
        }
        return t;
    }

    void fill_addr(this const SocketConfig& self, sockaddr_in* s) {
        if (self.addr != null)
            inet_pton(self.domain_as_int(), self.addr, reinterpret_cast<void*>(&s->sin_addr));
    }

    sockaddr_in to_sockaddr_in(this const SocketConfig& self) {
        auto s       = zeroed<sockaddr_in>();
        s.sin_port   = self.port_to_net();
        s.sin_family = static_cast<__kernel_sa_family_t>(self.domain_as_int());
        self.fill_addr(&s);
        return s;
    }
};

struct MultishotOp {
    u64 user_data;
    bool active{false};

    MultishotOp(u64 ud) : user_data(ud), active(true) {}
};

struct SocketUserDataMap {
    VAL_STRUCT(
        SocketUserDataMap, socket, 18446744073709551615_u64, bind, 18446744073709551614_u64, listen,
        18446744073709551613_u64, connect, 18446744073709551612_u64, socket_fd_close,
        18446744073709551611_u64, accept_fd_close, 18446744073709551610_u64, recv_cancel,
        18446744073709551609_u64, accept, 18446744073709551608_u64, recv, 18446744073709551607_u64,
        send, 18446744073709551606_u64, accept_multishot, 18446744073709551605_u64, recv_multishot,
        18446744073709551604_u64
    )
};

struct Socket {
    PIN_STRUCT(Socket, socket_fd, -1, conf, SocketConfig{}, udm, SocketUserDataMap{})

    static SysRes<Socket>
    init(IoUring* r, SocketConfig config, SocketUserDataMap _udm = {}, bool is_server = true) {
        Socket self(-1, config, _udm);
        sockaddr_in sock = config.to_sockaddr_in();

        self.socket_fd   = static_cast<FdI>(
            IO_TRY_ADD_AND_GET(
                r, r->socket(I64_MAX, config.domain_as_int(), config.type_as_int(), 0, 0), Socket
            )
                .res
        );

        auto wait_nr = 1;
        if (is_server) {
            if (config.type == SocketType::TCP || config.type == SocketType::SCTP) {
                IO_TRY_ADD(
                    r->link(r->bind(
                        _udm.bind, self.socket_fd, reinterpret_cast<sockaddr*>(&sock),
                        sizeof(sockaddr_in)
                    )),
                    Socket
                );
                IO_TRY_ADD(r->listen(_udm.listen, self.socket_fd, config.max_queued_conns), Socket);
                wait_nr = 2;
            } else {
                IO_TRY_ADD(
                    r->bind(
                        _udm.bind, self.socket_fd, reinterpret_cast<sockaddr*>(&sock),
                        sizeof(sockaddr_in)
                    ),
                    Socket
                );
            }
        } else {
            IO_TRY_ADD(
                r->connect(
                    _udm.connect, self.socket_fd, reinterpret_cast<sockaddr*>(&sock),
                    sizeof(sockaddr_in)
                ),
                Socket
            );
        }
        IO_TRY_ALL_AND_SUBMIT(r, static_cast<usize>(wait_nr == 2 ? 2 : 1), Socket);

        return move(self);
    }
    static SysRes<Socket>
    init_server(IoUring* r, SocketConfig config, SocketUserDataMap udm = SocketUserDataMap()) {
        return Socket::init(r, move(config), move(udm), true);
    }
    static SysRes<Socket>
    init_client(IoUring* r, SocketConfig config, SocketUserDataMap udm = SocketUserDataMap()) {
        return Socket::init(r, move(config), move(udm), false);
    }

    SysRes<None> deinit(this Socket& self, IoUring* r) {
        if (self.socket_fd != -1) {
            self.cancel_accept_multishot(r);
            IO_TRY_ADD_AND_GET(r, r->close(self.udm.socket_fd_close, self.socket_fd), None);
        }
        return None();
    }

    SysRes<FdU> accept(this Socket& self, IoUring* r) {
        return static_cast<FdU>(
            IO_TRY_ADD_AND_GET(r, r->accept(self.udm.accept, self.socket_fd, null, null, 0), FdU)
                .res
        );
    }

    SysRes<None> accept_multishot(this Socket& self, IoUring* r) {
        IO_TRY_ADD(
            r->accept_multishot(self.udm.accept_multishot, self.socket_fd, null, null, 0), None
        );
        return None();
    }
    SysRes<None> cancel_accept_multishot(this Socket& self, IoUring* r) {
        IO_TRY_ADD_AND_GET(r, r->cancel_fd(self.udm.accept_fd_close, self.socket_fd, 0), None);
        return None();
    }

    template <typename T>
    SysRes<None> send(this Socket& self, IoUring* r, const T* data, usize len) {
        if (IO_TRY_ADD_AND_GET(
                r,
                r->send_zc(
                    self.udm.send, self.socket_fd, reinterpret_cast<const u8*>(data),
                    len * sizeof(T), 0, 0
                ),
                None
            )
                .flags &
            IORING_CQE_F_MORE) {
            while (true) {
                auto submitted = TRY(r->submit_and_get_one(), None);
                if (TRY(submitted, None).flags & IORING_CQE_F_NOTIF) {
                    break;
                }
            }
        }
        return None();
    }

    SysRes<usize> recv(this Socket& self, IoUring* r, FdI fd, Slice<u8>* buf) {
        return static_cast<usize>(
            IO_TRY_ADD_AND_GET(
                r,
                r->recv(self.udm.recv, fd, reinterpret_cast<void*>(buf->ptr), buf->len, MSG_TRUNC),
                usize
            )
                .res
        );
    }
    SysRes<usize> recv(this Socket& self, IoUring* r, Slice<u8>* buf) {
        return self.recv(r, self.socket_fd, move(buf));
    }

    template <AllocatorStrategy U>
    SysRes<usize>
    recv(this Socket& self, Allocator<U>* a, IoUring* r, FdI fd, u32 bucket_size = 4096) {
        auto m      = TRY(Vec<u8>::init(a, bucket_size), usize);
        auto gotten = TRY(self.recv(r, fd, m.as_slice()));
        if (gotten > bucket_size) {
            u32 gap = gotten - bucket_size;
            TRY(m.resize(a, gotten), usize, SysResKind::NOMEM);
            TRY(self.recv(r, fd, Slice<u8>::init(gap, m.ptr + bucket_size)));
        }

        return gotten;
    }
    template <AllocatorStrategy U>
    SysRes<usize> recv(this Socket& self, Allocator<U>* a, IoUring* r, u32 bucket_size = 4096) {
        return self.recv(a, r, self.socket_fd, bucket_size);
    }

    SysRes<None> recv_multishot(this Socket& self, IoUring* r, FdI fd, u32 flags = 0) {
        IO_TRY_ADD(
            r->recv_multishot(
                self.udm.recv_multishot, fd, null, 0, static_cast<int>(flags | MSG_TRUNC)
            ),
            None
        );
        return None();
    }
    SysRes<None> recv_multishot(this Socket& self, IoUring* r, u32 flags = 0) {
        return self.recv_multishot(r, self.socket_fd, flags);
    }
    SysRes<None> cancel_recv_multishot(this Socket& self, IoUring* r) {
        IO_TRY_ADD_AND_GET(r, r->cancel(self.udm.recv_cancel, self.udm.recv_multishot, 0), None);
        return None();
    }
};
