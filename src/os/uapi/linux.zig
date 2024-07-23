const builtin = @import("builtin");
const arch_syscall = if (builtin.cpu.arch == .x86_64) @import("x86_64.zig") else @import("aarch64.zig");

pub const Uio = @import("uapi/uio.zig");
pub const Mman = @import("uapi/mman.zig");
pub const Fnctl = @import("uapi/fnctl.zig");
pub const Statx = @import("uapi/statx.zig");
pub const Signal = @import("uapi/signal.zig");
pub const Socket = @import("uapi/socket.zig");
pub const IoUring = @import("uapi/io_uring.zig");

pub const SyscallType = arch_syscall.SyscallType;

pub const Errorno = enum(u16) {
    /// No error occurred.
    /// Same code used for `NSROK`.
    SUCCESS = 0,
    /// Operation not permitted
    PERM = 1,
    /// No such file or directory
    NOENT = 2,
    /// No such process
    SRCH = 3,
    /// Interrupted system call
    INTR = 4,
    /// I/O error
    IO = 5,
    /// No such device or address
    NXIO = 6,
    /// Arg list too long
    @"2BIG" = 7,
    /// Exec format error
    NOEXEC = 8,
    /// Bad file number
    BADF = 9,
    /// No child processes
    CHILD = 10,
    /// Try again
    /// Also means: WOULDBLOCK: operation would block
    AGAIN = 11,
    /// Out of memory
    NOMEM = 12,
    /// Permission denied
    ACCES = 13,
    /// Bad address
    FAULT = 14,
    /// Block device required
    NOTBLK = 15,
    /// Device or resource busy
    BUSY = 16,
    /// File exists
    EXIST = 17,
    /// Cross-device link
    XDEV = 18,
    /// No such device
    NODEV = 19,
    /// Not a directory
    NOTDIR = 20,
    /// Is a directory
    ISDIR = 21,
    /// Invalid argument
    INVAL = 22,
    /// File table overflow
    NFILE = 23,
    /// Too many open files
    MFILE = 24,
    /// Not a typewriter
    NOTTY = 25,
    /// Text file busy
    TXTBSY = 26,
    /// File too large
    FBIG = 27,
    /// No space left on device
    NOSPC = 28,
    /// Illegal seek
    SPIPE = 29,
    /// Read-only file system
    ROFS = 30,
    /// Too many links
    MLINK = 31,
    /// Broken pipe
    PIPE = 32,
    /// Math argument out of domain of func
    DOM = 33,
    /// Math result not representable
    RANGE = 34,
    /// Resource deadlock would occur
    DEADLK = 35,
    /// File name too long
    NAMETOOLONG = 36,
    /// No record locks available
    NOLCK = 37,
    /// Function not implemented
    NOSYS = 38,
    /// Directory not empty
    NOTEMPTY = 39,
    /// Too many symbolic links encountered
    LOOP = 40,
    /// No message of desired type
    NOMSG = 42,
    /// Identifier removed
    IDRM = 43,
    /// Channel number out of range
    CHRNG = 44,
    /// Level 2 not synchronized
    L2NSYNC = 45,
    /// Level 3 halted
    L3HLT = 46,
    /// Level 3 reset
    L3RST = 47,
    /// Link number out of range
    LNRNG = 48,
    /// Protocol driver not attached
    UNATCH = 49,
    /// No CSI structure available
    NOCSI = 50,
    /// Level 2 halted
    L2HLT = 51,
    /// Invalid exchange
    BADE = 52,
    /// Invalid request descriptor
    BADR = 53,
    /// Exchange full
    XFULL = 54,
    /// No anode
    NOANO = 55,
    /// Invalid request code
    BADRQC = 56,
    /// Invalid slot
    BADSLT = 57,
    /// Bad font file format
    BFONT = 59,
    /// Device not a stream
    NOSTR = 60,
    /// No data available
    NODATA = 61,
    /// Timer expired
    TIME = 62,
    /// Out of streams resources
    NOSR = 63,
    /// Machine is not on the network
    NONET = 64,
    /// Package not installed
    NOPKG = 65,
    /// Object is remote
    REMOTE = 66,
    /// Link has been severed
    NOLINK = 67,
    /// Advertise error
    ADV = 68,
    /// Srmount error
    SRMNT = 69,
    /// Communication error on send
    COMM = 70,
    /// Protocol error
    PROTO = 71,
    /// Multihop attempted
    MULTIHOP = 72,
    /// RFS specific error
    DOTDOT = 73,
    /// Not a data message
    BADMSG = 74,
    /// Value too large for defined data type
    OVERFLOW = 75,
    /// Name not unique on network
    NOTUNIQ = 76,
    /// File descriptor in bad state
    BADFD = 77,
    /// Remote address changed
    REMCHG = 78,
    /// Can not access a needed shared library
    LIBACC = 79,
    /// Accessing a corrupted shared library
    LIBBAD = 80,
    /// .lib section in a.out corrupted
    LIBSCN = 81,
    /// Attempting to link in too many shared libraries
    LIBMAX = 82,
    /// Cannot exec a shared library directly
    LIBEXEC = 83,
    /// Illegal byte sequence
    ILSEQ = 84,
    /// Interrupted system call should be restarted
    RESTART = 85,
    /// Streams pipe error
    STRPIPE = 86,
    /// Too many users
    USERS = 87,
    /// Socket operation on non-socket
    NOTSOCK = 88,
    /// Destination address required
    DESTADDRREQ = 89,
    /// Message too long
    MSGSIZE = 90,
    /// Protocol wrong type for socket
    PROTOTYPE = 91,
    /// Protocol not available
    NOPROTOOPT = 92,
    /// Protocol not supported
    PROTONOSUPPORT = 93,
    /// Socket type not supported
    SOCKTNOSUPPORT = 94,
    /// Operation not supported on transport endpoint
    /// This code also means `NOTSUP`.
    OPNOTSUPP = 95,
    /// Protocol family not supported
    PFNOSUPPORT = 96,
    /// Address family not supported by protocol
    AFNOSUPPORT = 97,
    /// Address already in use
    ADDRINUSE = 98,
    /// Cannot assign requested address
    ADDRNOTAVAIL = 99,
    /// Network is down
    NETDOWN = 100,
    /// Network is unreachable
    NETUNREACH = 101,
    /// Network dropped connection because of reset
    NETRESET = 102,
    /// Software caused connection abort
    CONNABORTED = 103,
    /// Connection reset by peer
    CONNRESET = 104,
    /// No buffer space available
    NOBUFS = 105,
    /// Transport endpoint is already connected
    ISCONN = 106,
    /// Transport endpoint is not connected
    NOTCONN = 107,
    /// Cannot send after transport endpoint shutdown
    SHUTDOWN = 108,
    /// Too many references: cannot splice
    TOOMANYREFS = 109,
    /// Connection timed out
    TIMEDOUT = 110,
    /// Connection refused
    CONNREFUSED = 111,
    /// Host is down
    HOSTDOWN = 112,
    /// No route to host
    HOSTUNREACH = 113,
    /// Operation already in progress
    ALREADY = 114,
    /// Operation now in progress
    INPROGRESS = 115,
    /// Stale NFS file handle
    STALE = 116,
    /// Structure needs cleaning
    UCLEAN = 117,
    /// Not a XENIX named type file
    NOTNAM = 118,
    /// No XENIX semaphores available
    NAVAIL = 119,
    /// Is a named type file
    ISNAM = 120,
    /// Remote I/O error
    REMOTEIO = 121,
    /// Quota exceeded
    DQUOT = 122,
    /// No medium found
    NOMEDIUM = 123,
    /// Wrong medium type
    MEDIUMTYPE = 124,
    /// Operation canceled
    CANCELED = 125,
    /// Required key not available
    NOKEY = 126,
    /// Key has expired
    KEYEXPIRED = 127,
    /// Key has been revoked
    KEYREVOKED = 128,
    /// Key was rejected by service
    KEYREJECTED = 129,

    // for robust mutexes

    /// Owner died
    OWNERDEAD = 130,
    /// State not recoverable
    NOTRECOVERABLE = 131,
    /// Operation not possible due to RF-kill
    RFKILL = 132,
    /// Memory page has hardware error
    HWPOISON = 133,

    // nameserver query return codes

    /// DNS server returned answer with no data
    NSRNODATA = 160,
    /// DNS server claims query was misformatted
    NSRFORMERR = 161,
    /// DNS server returned general failure
    NSRSERVFAIL = 162,
    /// Domain name not found
    NSRNOTFOUND = 163,
    /// DNS server does not implement requested operation
    NSRNOTIMP = 164,
    /// DNS server refused query
    NSRREFUSED = 165,
    /// Misformatted DNS query
    NSRBADQUERY = 166,
    /// Misformatted domain name
    NSRBADNAME = 167,
    /// Unsupported address family
    NSRBADFAMILY = 168,
    /// Misformatted DNS reply
    NSRBADRESP = 169,
    /// Could not contact DNS servers
    NSRCONNREFUSED = 170,
    /// Timeout while contacting DNS servers
    NSRTIMEOUT = 171,
    /// End of file
    NSROF = 172,
    /// Error reading file
    NSRFILE = 173,
    /// Out of memory
    NSRNOMEM = 174,
    /// Application terminated lookup
    NSRDESTRUCTION = 175,
    /// Domain name is too long
    NSRQUERYDOMAINTOOLONG = 176,
    /// Domain name is too long
    NSRCNAMELOOP = 177,

    _,
};

pub const Error = error{
    /// Operation not permitted
    PERM,
    /// No such file or directory
    NOENT,
    /// No such process
    SRCH,
    /// Interrupted system call
    INTR,
    /// I/O error
    IO,
    /// No such device or address
    NXIO,
    /// Arg list too long
    BIG,
    /// Exec format error
    NOEXEC,
    /// Bad file number
    BADF,
    /// No child processes
    CHILD,
    /// Try again
    /// Also means: WOULDBLOCK: operation would block
    AGAIN,
    /// Out of memory
    NOMEM,
    /// Permission denied
    ACCES,
    /// Bad address
    FAULT,
    /// Block device required
    NOTBLK,
    /// Device or resource busy
    BUSY,
    /// File exists
    EXIST,
    /// Cross-device link
    XDEV,
    /// No such device
    NODEV,
    /// Not a directory
    NOTDIR,
    /// Is a directory
    ISDIR,
    /// Invalid argument
    INVAL,
    /// File table overflow
    NFILE,
    /// Too many open files
    MFILE,
    /// Not a typewriter
    NOTTY,
    /// Text file busy
    TXTBSY,
    /// File too large
    FBIG,
    /// No space left on device
    NOSPC,
    /// Illegal seek
    SPIPE,
    /// Read-only file system
    ROFS,
    /// Too many links
    MLINK,
    /// Broken pipe
    PIPE,
    /// Math argument out of domain of func
    DOM,
    /// Math result not representable
    RANGE,
    /// Resource deadlock would occur
    DEADLK,
    /// File name too long
    NAMETOOLONG,
    /// No record locks available
    NOLCK,
    /// Function not implemented
    NOSYS,
    /// Directory not empty
    NOTEMPTY,
    /// Too many symbolic links encountered
    LOOP,
    /// No message of desired type
    NOMSG,
    /// Identifier removed
    IDRM,
    /// Channel number out of range
    CHRNG,
    /// Level 2 not synchronized
    L2NSYNC,
    /// Level 3 halted
    L3HLT,
    /// Level 4 reset
    L3RST,
    /// Link number out of range
    LNRNG,
    /// Protocol driver not attached
    UNATCH,
    /// No CSI structure available
    NOCSI,
    /// Level 2 halted
    Level,
    /// Invalid exchange
    BADE,
    /// Invalid request descriptor
    BADR,
    /// Exchange full
    XFULL,
    /// No anode
    NOANO,
    /// Invalid request code
    BADRQC,
    /// Invalid slot
    BADSLT,
    /// Bad font file format
    BFONT,
    /// Device not a stream
    NOSTR,
    /// No data available
    NODATA,
    /// Timer expired
    TIME,
    /// Out of streams resources
    NOSR,
    /// Machine is not on the network
    NONET,
    /// Package not installed
    NOPKG,
    /// Object is remote
    REMOTE,
    /// Link has been severed
    NOLINK,
    /// Advertise error
    ADV,
    /// Srmount error
    SRMNT,
    /// Communication error on send
    COMM,
    /// Protocol error
    PROTO,
    /// Multihop attempted
    MULTIHOP,
    /// RFS specific error
    DOTDOT,
    /// Not a data message
    BADMSG,
    /// Value too large for defined data type
    OVERFLOW,
    /// Name not unique on network
    NOTUNIQ,
    /// File descriptor in bad state
    BADFD,
    /// Remote address changed
    REMCHG,
    /// Can not access a needed shared library
    LIBACC,
    /// Accessing a corrupted shared library
    LIBBAD,
    /// .lib section in a.out corrupted
    LIBSCN,
    /// Attempting to link in too many shared libraries
    LIBMAX,
    /// Cannot exec a shared library directly
    LIBEXEC,
    /// Illegal byte sequence
    ILSEQ,
    /// Interrupted system call should be restarted
    RESTART,
    /// Streams pipe error
    STRPIPE,
    /// Too many users
    USERS,
    /// Socket operation on non-socket
    NOTSOCK,
    /// Destination address required
    DESTADDRREQ,
    /// Message too long
    MSGSIZE,
    /// Protocol wrong type for socket
    PROTOTYPE,
    /// Protocol not available
    NOPROTOOPT,
    /// Protocol not supported
    PROTONOSUPPORT,
    /// Socket type not supported
    SOCKTNOSUPPORT,
    /// Operation not supported on transport endpoint
    /// This code also means `NOTSUP`.
    OPNOTSUPP,
    /// Protocol family not supported
    PFNOSUPPORT,
    /// Address family not supported by protocol
    AFNOSUPPORT,
    /// Address already in use
    ADDRINUSE,
    /// Cannot assign requested address
    ADDRNOTAVAIL,
    /// Network is down
    NETDOWN,
    /// Network is unreachable
    NETUNREACH,
    /// Network dropped connection because of reset
    NETRESET,
    /// Software caused connection abort
    CONNABORTED,
    /// Connection reset by peer
    CONNRESET,
    /// No buffer space available
    NOBUFS,
    /// Transport endpoint is already connected
    ISCONN,
    /// Transport endpoint is not connected
    NOTCONN,
    /// Cannot send after transport endpoint shutdown
    SHUTDOWN,
    /// Too many references: cannot splice
    TOOMANYREFS,
    /// Connection timed out
    TIMEDOUT,
    /// Connection refused
    CONNREFUSED,
    /// Host is down
    HOSTDOWN,
    /// No route to host
    HOSTUNREACH,
    /// Operation already in progress
    ALREADY,
    /// Operation now in progress
    INPROGRESS,
    /// Stale NFS file handle
    STALE,
    /// Structure needs cleaning
    UCLEAN,
    /// Not a XENIX named type file
    NOTNAM,
    /// No XENIX semaphores available
    NAVAIL,
    /// Is a named type file
    ISNAM,
    /// Remote I/O error
    REMOTEIO,
    /// Quota exceeded
    DQUOT,
    /// No medium found
    NOMEDIUM,
    /// Wrong medium type
    MEDIUMTYPE,
    /// Operation canceled
    CANCELED,
    /// Required key not available
    NOKEY,
    /// Key has expired
    KEYEXPIRED,
    /// Key has been revoked
    KEYREVOKED,
    /// Key was rejected by service
    KEYREJECTED,

    // for robust mutexes

    /// Owner died
    OWNERDEAD,
    /// State not recoverable
    NOTRECOVERABLE,
    /// Operation not possible due to RF-kill
    RFKILL,
    /// Memory page has hardware error
    HWPOISON,

    // nameserver query return codes

    /// DNS server returned answer with no data
    NSRNODATA,
    /// DNS server claims query was misformatted
    NSRFORMERR,
    /// DNS server returned general failure
    NSRSERVFAIL,
    /// Domain name not found
    NSRNOTFOUND,
    /// DNS server does not implement requested operation
    NSRNOTIMP,
    /// DNS server refused query
    NSRREFUSED,
    /// Misformatted DNS query
    NSRBADQUERY,
    /// Misformatted domain name
    NSRBADNAME,
    /// Unsupported address family
    NSRBADFAMILY,
    /// Misformatted DNS reply
    NSRBADRESP,
    /// Could not contact DNS servers
    NSRCONNREFUSED,
    /// Timeout while contacting DNS servers
    NSRTIMEOUT,
    /// End of file
    NSROF,
    /// Error reading file
    NSRFILE,
    /// Out of memory
    NSRNOMEM,
    /// Application terminated lookup
    NSRDESTRUCTION,
    /// Domain name is too long
    NSRQUERYDOMAINTOOLONG,
    /// Domain name is too long
    NSRCNAMELOOP,

    _,
};

/// Get the errno from a syscall return value. Zero means no error.
pub fn get_errno(r: usize) Errorno {
    @setRuntimeSafety(false);

    const signed_r = @as(isize, @bitCast(r));
    const int = if (signed_r > -4096 and signed_r < 0) -signed_r else 0;
    return @as(Errorno, @enumFromInt(int));
}

/// Performs a linux syscall.
pub fn syscall(syscall_type: SyscallType, args: anytype) Error!usize {
    @setRuntimeSafety(false);

    const res = arch_syscall.syscall(syscall_type, args);
    const errno = get_errno(res);

    if (errno != .SUCCESS) return @errorCast(@errorFromInt(@intFromEnum(errno) - 1));
    return res;
}
