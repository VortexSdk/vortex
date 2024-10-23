#pragma once

#include "../../diagnostics.hpp"
#include "../../mem/utils.hpp"
#include "../../metap/metap.hpp"
#include "../../numbers.hpp"
#include "../../panic.hpp"

// #define IS_ARM defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
#if defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
#define IF_ARM_OR(aarch64_code, other_code) aarch64_code;
// #define IF_ARM_OR_EXP(aarch64_code, other_code) (aarch64_code)
#else
#define IF_ARM_OR(aarch64_code, other_code) other_code;
// #define IF_ARM_OR_EXP(aarch64_code, other_code) (other_code)
#endif

using Sfd = int;
using Ufd = unsigned int;

enum class SysResKind : u8 {
    /// No error occurred.
    /// Same code used for `NSROK`.
    SUCCESS               = 0,
    /// Operation not permitted
    PERM                  = 1,
    /// No such file or directory
    NOENT                 = 2,
    /// No such process
    SRCH                  = 3,
    /// Interrupted system call
    INTR                  = 4,
    /// I/O error
    IO                    = 5,
    /// No such device or address
    NXIO                  = 6,
    /// Arg list too long
    A2BIG                 = 7,
    /// Exec format error
    NOEXEC                = 8,
    /// Bad file number
    BADF                  = 9,
    /// No child processes
    CHILD                 = 10,
    /// Try again
    /// Also means: WOULDBLOCK: operation would block
    AGAIN                 = 11,
    /// Out of memory
    NOMEM                 = 12,
    /// Permission denied
    ACCES                 = 13,
    /// Bad address
    FAULT                 = 14,
    /// Block device required
    NOTBLK                = 15,
    /// Device or resource busy
    BUSY                  = 16,
    /// File exists
    EXIST                 = 17,
    /// Cross-device link
    XDEV                  = 18,
    /// No such device
    NODEV                 = 19,
    /// Not a directory
    NOTDIR                = 20,
    /// Is a directory
    ISDIR                 = 21,
    /// Invalid argument
    INVAL                 = 22,
    /// File table overflow
    NFILE                 = 23,
    /// Too many open files
    MFILE                 = 24,
    /// Not a typewriter
    NOTTY                 = 25,
    /// Text file busy
    TXTBSY                = 26,
    /// File too large
    FBIG                  = 27,
    /// No space left on device
    NOSPC                 = 28,
    /// Illegal seek
    SPIPE                 = 29,
    /// Read-only file system
    ROFS                  = 30,
    /// Too many links
    MLINK                 = 31,
    /// Broken pipe
    PIPE                  = 32,
    /// Math argument out of domain of func
    DOM                   = 33,
    /// Math result not representable
    RANGE                 = 34,
    /// Resource deadlock would occur
    DEADLK                = 35,
    /// File name too long
    NAMETOOLONG           = 36,
    /// No record locks available
    NOLCK                 = 37,
    /// Function not implemented
    NOSYS                 = 38,
    /// Directory not empty
    NOTEMPTY              = 39,
    /// Too many symbolic links encountered
    LOOP                  = 40,
    /// No message of desired type
    NOMSG                 = 42,
    /// Identifier removed
    IDRM                  = 43,
    /// Channel number out of range
    CHRNG                 = 44,
    /// Level 2 not synchronized
    L2NSYNC               = 45,
    /// Level 3 halted
    L3HLT                 = 46,
    /// Level 3 reset
    L3RST                 = 47,
    /// Link number out of range
    LNRNG                 = 48,
    /// Protocol driver not attached
    UNATCH                = 49,
    /// No CSI structure available
    NOCSI                 = 50,
    /// Level 2 halted
    L2HLT                 = 51,
    /// Invalid exchange
    BADE                  = 52,
    /// Invalid request descriptor
    BADR                  = 53,
    /// Exchange full
    XFULL                 = 54,
    /// No anode
    NOANO                 = 55,
    /// Invalid request code
    BADRQC                = 56,
    /// Invalid slot
    BADSLT                = 57,
    /// Bad font file format
    BFONT                 = 59,
    /// Device not a stream
    NOSTR                 = 60,
    /// No data available
    NODATA                = 61,
    /// Timer expired
    TIME                  = 62,
    /// Out of streams resources
    NOSR                  = 63,
    /// Machine is not on the network
    NONET                 = 64,
    /// Package not installed
    NOPKG                 = 65,
    /// Object is remote
    REMOTE                = 66,
    /// Link has been severed
    NOLINK                = 67,
    /// Advertise error
    ADV                   = 68,
    /// Srmount error
    SRMNT                 = 69,
    /// Communication error on send
    COMM                  = 70,
    /// Protocol error
    PROTO                 = 71,
    /// Multihop attempted
    MULTIHOP              = 72,
    /// RFS specific error
    DOTDOT                = 73,
    /// Not a data message
    BADMSG                = 74,
    /// Value too large for defined data type
    OVERFLOW              = 75,
    /// Name not unique on network
    NOTUNIQ               = 76,
    /// File descriptor in bad state
    BADFD                 = 77,
    /// Remote address changed
    REMCHG                = 78,
    /// Can not access a needed shared library
    LIBACC                = 79,
    /// Accessing a corrupted shared library
    LIBBAD                = 80,
    /// .lib section in a.out corrupted
    LIBSCN                = 81,
    /// Attempting to link in too many shared libraries
    LIBMAX                = 82,
    /// Cannot exec a shared library directly
    LIBEXEC               = 83,
    /// Illegal byte sequence
    ILSEQ                 = 84,
    /// Interrupted system call should be restarted
    RESTART               = 85,
    /// Streams pipe error
    STRPIPE               = 86,
    /// Too many users
    USERS                 = 87,
    /// Socket operation on non-socket
    NOTSOCK               = 88,
    /// Destination address required
    DESTADDRREQ           = 89,
    /// Message too long
    MSGSIZE               = 90,
    /// Protocol wrong type for socket
    PROTOTYPE             = 91,
    /// Protocol not available
    NOPROTOOPT            = 92,
    /// Protocol not supported
    PROTONOSUPPORT        = 93,
    /// Socket type not supported
    SOCKTNOSUPPORT        = 94,
    /// Operation not supported on transport endpoint
    /// This code also means `NOTSUP`.
    OPNOTSUPP             = 95,
    /// Protocol family not supported
    PFNOSUPPORT           = 96,
    /// Address family not supported by protocol
    AFNOSUPPORT           = 97,
    /// Address already in use
    ADDRINUSE             = 98,
    /// Cannot assign requested address
    ADDRNOTAVAIL          = 99,
    /// Network is down
    NETDOWN               = 100,
    /// Network is unreachable
    NETUNREACH            = 101,
    /// Network dropped connection because of reset
    NETRESET              = 102,
    /// Software caused connection abort
    CONNABORTED           = 103,
    /// Connection reset by peer
    CONNRESET             = 104,
    /// No buffer space available
    NOBUFS                = 105,
    /// Transport endpoint is already connected
    ISCONN                = 106,
    /// Transport endpoint is not connected
    NOTCONN               = 107,
    /// Cannot send after transport endpoint shutdown
    SHUTDOWN              = 108,
    /// Too many references: cannot splice
    TOOMANYREFS           = 109,
    /// Connection timed out
    TIMEDOUT              = 110,
    /// Connection refused
    CONNREFUSED           = 111,
    /// Host is down
    HOSTDOWN              = 112,
    /// No route to host
    HOSTUNREACH           = 113,
    /// Operation already in progress
    ALREADY               = 114,
    /// Operation now in progress
    INPROGRESS            = 115,
    /// Stale NFS file handle
    STALE                 = 116,
    /// Structure needs cleaning
    UCLEAN                = 117,
    /// Not a XENIX named type file
    NOTNAM                = 118,
    /// No XENIX semaphores available
    NAVAIL                = 119,
    /// Is a named type file
    ISNAM                 = 120,
    /// Remote I/O error
    REMOTEIO              = 121,
    /// Quota exceeded
    DQUOT                 = 122,
    /// No medium found
    NOMEDIUM              = 123,
    /// Wrong medium type
    MEDIUMTYPE            = 124,
    /// Operation canceled
    CANCELED              = 125,
    /// Required key not available
    NOKEY                 = 126,
    /// Key has expired
    KEYEXPIRED            = 127,
    /// Key has been revoked
    KEYREVOKED            = 128,
    /// Key was rejected by service
    KEYREJECTED           = 129,

    // for robust mutexes

    /// Owner died
    OWNERDEAD             = 130,
    /// State not recoverable
    NOTRECOVERABLE        = 131,
    /// Operation not possible due to RF-kill
    RFKILL                = 132,
    /// Memory page has hardware error
    HWPOISON              = 133,

    // nameserver query return codes

    /// DNS server returned answer with no data
    NSRNODATA             = 160,
    /// DNS server claims query was misformatted
    NSRFORMERR            = 161,
    /// DNS server returned general failure
    NSRSERVFAIL           = 162,
    /// Domain name not found
    NSRNOTFOUND           = 163,
    /// DNS server does not implement requested operation
    NSRNOTIMP             = 164,
    /// DNS server refused query
    NSRREFUSED            = 165,
    /// Misformatted DNS query
    NSRBADQUERY           = 166,
    /// Misformatted domain name
    NSRBADNAME            = 167,
    /// Unsupported address family
    NSRBADFAMILY          = 168,
    /// Misformatted DNS reply
    NSRBADRESP            = 169,
    /// Could not contact DNS servers
    NSRCONNREFUSED        = 170,
    /// Timeout while contacting DNS servers
    NSRTIMEOUT            = 171,
    /// End of file
    NSROF                 = 172,
    /// Error reading file
    NSRFILE               = 173,
    /// Out of memory
    NSRNOMEM              = 174,
    /// Application terminated lookup
    NSRDESTRUCTION        = 175,
    /// Domain name is too long
    NSRQUERYDOMAINTOOLONG = 176,
    /// Domain name is too long
    NSRCNAMELOOP          = 177,
};

template <typename T> struct SysRes {
  private:
    T res;
    bool safety_called{false};

    SysRes(T r, SysResKind k) : res(move(r)), kind(k) {}

    static void err_panic() {
        panic("Accessing the value inside SysRes when it contains an error. "
              "Panicing to prevent undefined behavior.\n");
    }

  public:
    SysResKind kind{SysResKind::SUCCESS};

    static SysRes<T> from_kind(T r, SysResKind k) {
        return SysRes<T>(move(r), k);
    }
    static SysRes<T> from_kind(T r, u8 k) {
        return SysRes<T>::from_kind(move(r), static_cast<SysResKind>(k));
    }
    static SysRes<T> from_successful(T r) {
        return SysRes<T>(move(r), SysResKind::SUCCESS);
    }
    static SysRes<T> from_kind_err(T r, SysResKind k) {
        return SysRes<T>(move(r), k);
    }
    static SysRes<T> from_err(SysResKind k) {
        return SysRes<T>(move(T()), k);
    }

    static SysRes<T> from_sys(usize r) {
        static_assert(is_same<T, usize>::value, "T is not usize");

        const i64 signed_r = static_cast<i64>(r);
        return SysRes<T>::from_kind(
            r, static_cast<u8>((signed_r > -4096 && signed_r < 0) ? -signed_r : 0)
        );
    }

    [[nodiscard]]
    bool is_ok(this SysRes &self) {
        self.safety_called = true;

        return self.kind == SysResKind::SUCCESS;
    }

    [[nodiscard]]
    bool is_err(this SysRes &self) {
        self.safety_called = true;

        return !self.is_ok();
    }

    DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast", "-Wcast-align")
    template <typename Y> SysRes<Y> unsafe_cast(this const SysRes &self) {
        return SysRes<Y>::from_kind((Y)self.res, self.kind);
    }
    DIAG_IGNORE_POP

    template <typename Y> SysRes<Y> unsafe_cast_aligned(this const SysRes &self) {
        static_assert(is_pointer<T>::value && is_pointer<Y>::value, "Not a pointer!");

        constexpr usize alignment = alignof(remove_pointer_t<Y>);
        usize addr                = reinterpret_cast<usize>(self.res);
        const usize misalignment  = addr % alignment;
        if (misalignment != 0) addr += (alignment - misalignment);

        return SysRes<Y>::from_kind(reinterpret_cast<Y>(addr), self.kind);
    }

    template <typename Y> SysRes<Y> unsafe_swap(this const SysRes &self, Y d) {
        return SysRes<Y>::from_kind(move(d), self.kind);
    }

    template <typename Y> SysRes<Y> return_err(this const SysRes &self) {
        return self.unsafe_swap(move(Y()));
    }

    T unsafe_unwrap(this SysRes &self) {
        return move(self.res);
    }

    T unwrap(this SysRes &self) {
#ifndef __OPTIMIZE__
        if (self.is_err()) SysRes<T>::err_panic();
#else
        if (!self.safety_called && self.is_err()) SysRes<T>::err_panic();
#endif

        return move(self.unsafe_unwrap());
    }
};

DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast")
template <typename... T> static SysRes<usize> syscall(usize n, T... args) {
    constexpr usize n_args = sizeof...(T);
    static_assert(n_args <= 6, "Too much syscall arguments! You can only pass up to 6.");

    const usize args_array [] = {(usize)(args)...};
    IF_ARM_OR(register usize r10 __asm__("x4"), register usize r10 __asm__("r10"))
    IF_ARM_OR(register usize r8 __asm__("x5"), register usize r8 __asm__("r8"))
    IF_ARM_OR(register usize r9 __asm__("x8"), register usize r9 __asm__("r9"))
    if constexpr (n_args > 3) r10 = args_array [3];
    if constexpr (n_args > 4) r8 = args_array [4];
    if constexpr (n_args > 5) r9 = args_array [5];

    usize ret;
    if constexpr (n_args == 0) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0" : "=r"(_r0) : "r"(_r0) : "memory"),
            __asm__ __volatile__("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory")
        )
    } else if constexpr (n_args == 1) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0" : "=r"(_r0) : "r"(_r0), "r"(_r1) : "memory"),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 2) {
        IF_ARM_OR(
            __asm__ __volatile__("svc #0" : "=r"(_r0) : "r"(_r0), "r"(_r1), "r"(_r2) : "memory"),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]) : "rcx",
                "r11", "memory"
            )
        )
    } else if constexpr (n_args == 3) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(_r0) : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3) : "memory"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 4) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(_r0) : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(r10) : "memory"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]), "r"(r10) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 5) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(_r0) : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(r10),
                "r"(r8), : "memory"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]), "r"(r10), "r"(r8) : "rcx", "r11", "memory"
            )
        )
    } else if constexpr (n_args == 6) {
        IF_ARM_OR(
            __asm__ __volatile__(
                "svc #0" : "=r"(_r0) : "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3), "r"(r10), "r"(r8),
                "r"(r9) : "memory"
            ),
            __asm__ __volatile__(
                "syscall" : "=a"(ret) : "a"(n), "D"(args_array [0]), "S"(args_array [1]),
                "d"(args_array [2]), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory"
            )
        )
    }

    return SysRes<usize>::from_sys(ret);
}
DIAG_IGNORE_CLANG_POP

#include "wrapper.hpp"
