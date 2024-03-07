#pragma once

#include "../../numbers.h"

#if defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)
    #include "aarch64-syscall.h"
#else
    #include "x86_64-syscall.h"
#endif

typedef enum SyscallError
{
    /// No error occurred.
    /// Same code used for `NSROK`.
    SE_SUCCESS               = 0,
    /// Operation not permitted
    SE_PERM                  = 1,
    /// No such file or directory
    SE_NOENT                 = 2,
    /// No such process
    SE_SRCH                  = 3,
    /// Interrupted system call
    SE_INTR                  = 4,
    /// I/O error
    SE_IO                    = 5,
    /// No such device or address
    SE_NXIO                  = 6,
    /// Arg list too long
    SE_2BIG                  = 7,
    /// Exec format error
    SE_NOEXEC                = 8,
    /// Bad file number
    SE_BADF                  = 9,
    /// No child processes
    SE_CHILD                 = 10,
    /// Try again
    /// Also means: WOULDBLOCK: operation would block
    SE_AGAIN                 = 11,
    /// Out of memory
    SE_NOMEM                 = 12,
    /// Permission denied
    SE_ACCES                 = 13,
    /// Bad address
    SE_FAULT                 = 14,
    /// Block device required
    SE_NOTBLK                = 15,
    /// Device or resource busy
    SE_BUSY                  = 16,
    /// File exists
    SE_EXIST                 = 17,
    /// Cross-device link
    SE_XDEV                  = 18,
    /// No such device
    SE_NODEV                 = 19,
    /// Not a directory
    SE_NOTDIR                = 20,
    /// Is a directory
    SE_ISDIR                 = 21,
    /// Invalid argument
    SE_INVAL                 = 22,
    /// File table overflow
    SE_NFILE                 = 23,
    /// Too many open files
    SE_MFILE                 = 24,
    /// Not a typewriter
    SE_NOTTY                 = 25,
    /// Text file busy
    SE_TXTBSY                = 26,
    /// File too large
    SE_FBIG                  = 27,
    /// No space left on device
    SE_NOSPC                 = 28,
    /// Illegal seek
    SE_SPIPE                 = 29,
    /// Read-only file system
    SE_ROFS                  = 30,
    /// Too many links
    SE_MLINK                 = 31,
    /// Broken pipe
    SE_PIPE                  = 32,
    /// Math argument out of domain of func
    SE_DOM                   = 33,
    /// Math result not representable
    SE_RANGE                 = 34,
    /// Resource deadlock would occur
    SE_DEADLK                = 35,
    /// File name too long
    SE_NAMETOOLONG           = 36,
    /// No record locks available
    SE_NOLCK                 = 37,
    /// Function not implemented
    SE_NOSYS                 = 38,
    /// Directory not empty
    SE_NOTEMPTY              = 39,
    /// Too many symbolic links encountered
    SE_LOOP                  = 40,
    /// No message of desired type
    SE_NOMSG                 = 42,
    /// Identifier removed
    SE_IDRM                  = 43,
    /// Channel number out of range
    SE_CHRNG                 = 44,
    /// Level 2 not synchronized
    SE_L2NSYNC               = 45,
    /// Level 3 halted
    SE_L3HLT                 = 46,
    /// Level 3 reset
    SE_L3RST                 = 47,
    /// Link number out of range
    SE_LNRNG                 = 48,
    /// Protocol driver not attached
    SE_UNATCH                = 49,
    /// No CSI structure available
    SE_NOCSI                 = 50,
    /// Level 2 halted
    SE_L2HLT                 = 51,
    /// Invalid exchange
    SE_BADE                  = 52,
    /// Invalid request descriptor
    SE_BADR                  = 53,
    /// Exchange full
    SE_XFULL                 = 54,
    /// No anode
    SE_NOANO                 = 55,
    /// Invalid request code
    SE_BADRQC                = 56,
    /// Invalid slot
    SE_BADSLT                = 57,
    /// Bad font file format
    SE_BFONT                 = 59,
    /// Device not a stream
    SE_NOSTR                 = 60,
    /// No data available
    SE_NODATA                = 61,
    /// Timer expired
    SE_TIME                  = 62,
    /// Out of streams resources
    SE_NOSR                  = 63,
    /// Machine is not on the network
    SE_NONET                 = 64,
    /// Package not installed
    SE_NOPKG                 = 65,
    /// Object is remote
    SE_REMOTE                = 66,
    /// Link has been severed
    SE_NOLINK                = 67,
    /// Advertise error
    SE_ADV                   = 68,
    /// Srmount error
    SE_SRMNT                 = 69,
    /// Communication error on send
    SE_COMM                  = 70,
    /// Protocol error
    SE_PROTO                 = 71,
    /// Multihop attempted
    SE_MULTIHOP              = 72,
    /// RFS specific error
    SE_DOTDOT                = 73,
    /// Not a data message
    SE_BADMSG                = 74,
    /// Value too large for defined data type
    SE_OVERFLOW              = 75,
    /// Name not unique on network
    SE_NOTUNIQ               = 76,
    /// File descriptor in bad state
    SE_BADFD                 = 77,
    /// Remote address changed
    SE_REMCHG                = 78,
    /// Can not access a needed shared library
    SE_LIBACC                = 79,
    /// Accessing a corrupted shared library
    SE_LIBBAD                = 80,
    /// .lib section in a.out corrupted
    SE_LIBSCN                = 81,
    /// Attempting to link in too many shared libraries
    SE_LIBMAX                = 82,
    /// Cannot exec a shared library directly
    SE_LIBEXEC               = 83,
    /// Illegal byte sequence
    SE_ILSEQ                 = 84,
    /// Interrupted system call should be restarted
    SE_RESTART               = 85,
    /// Streams pipe error
    SE_STRPIPE               = 86,
    /// Too many users
    SE_USERS                 = 87,
    /// Socket operation on non-socket
    SE_NOTSOCK               = 88,
    /// Destination address required
    SE_DESTADDRREQ           = 89,
    /// Message too long
    SE_MSGSIZE               = 90,
    /// Protocol wrong type for socket
    SE_PROTOTYPE             = 91,
    /// Protocol not available
    SE_NOPROTOOPT            = 92,
    /// Protocol not supported
    SE_PROTONOSUPPORT        = 93,
    /// Socket type not supported
    SE_SOCKTNOSUPPORT        = 94,
    /// Operation not supported on transport endpoint
    /// This code also means `NOTSUP`.
    SE_OPNOTSUPP             = 95,
    /// Protocol family not supported
    SE_PFNOSUPPORT           = 96,
    /// Address family not supported by protocol
    SE_AFNOSUPPORT           = 97,
    /// Address already in use
    SE_ADDRINUSE             = 98,
    /// Cannot assign requested address
    SE_ADDRNOTAVAIL          = 99,
    /// Network is down
    SE_NETDOWN               = 100,
    /// Network is unreachable
    SE_NETUNREACH            = 101,
    /// Network dropped connection because of reset
    SE_NETRESET              = 102,
    /// Software caused connection abort
    SE_CONNABORTED           = 103,
    /// Connection reset by peer
    SE_CONNRESET             = 104,
    /// No buffer space available
    SE_NOBUFS                = 105,
    /// Transport endpoint is already connected
    SE_ISCONN                = 106,
    /// Transport endpoint is not connected
    SE_NOTCONN               = 107,
    /// Cannot send after transport endpoint shutdown
    SE_SHUTDOWN              = 108,
    /// Too many references: cannot splice
    SE_TOOMANYREFS           = 109,
    /// Connection timed out
    SE_TIMEDOUT              = 110,
    /// Connection refused
    SE_CONNREFUSED           = 111,
    /// Host is down
    SE_HOSTDOWN              = 112,
    /// No route to host
    SE_HOSTUNREACH           = 113,
    /// Operation already in progress
    SE_ALREADY               = 114,
    /// Operation now in progress
    SE_INPROGRESS            = 115,
    /// Stale NFS file handle
    SE_STALE                 = 116,
    /// Structure needs cleaning
    SE_UCLEAN                = 117,
    /// Not a XENIX named type file
    SE_NOTNAM                = 118,
    /// No XENIX semaphores available
    SE_NAVAIL                = 119,
    /// Is a named type file
    SE_ISNAM                 = 120,
    /// Remote I/O error
    SE_REMOTEIO              = 121,
    /// Quota exceeded
    SE_DQUOT                 = 122,
    /// No medium found
    SE_NOMEDIUM              = 123,
    /// Wrong medium type
    SE_MEDIUMTYPE            = 124,
    /// Operation canceled
    SE_CANCELED              = 125,
    /// Required key not available
    SE_NOKEY                 = 126,
    /// Key has expired
    SE_KEYEXPIRED            = 127,
    /// Key has been revoked
    SE_KEYREVOKED            = 128,
    /// Key was rejected by service
    SE_KEYREJECTED           = 129,

    // for robust mutexes

    /// Owner died
    SE_OWNERDEAD             = 130,
    /// State not recoverable
    SE_NOTRECOVERABLE        = 131,
    /// Operation not possible due to RF-kill
    SE_RFKILL                = 132,
    /// Memory page has hardware error
    SE_HWPOISON              = 133,

    // nameserver query return codes

    /// DNS server returned answer with no data
    SE_NSRNODATA             = 160,
    /// DNS server claims query was misformatted
    SE_NSRFORMERR            = 161,
    /// DNS server returned general failure
    SE_NSRSERVFAIL           = 162,
    /// Domain name not found
    SE_NSRNOTFOUND           = 163,
    /// DNS server does not implement requested operation
    SE_NSRNOTIMP             = 164,
    /// DNS server refused query
    SE_NSRREFUSED            = 165,
    /// Misformatted DNS query
    SE_NSRBADQUERY           = 166,
    /// Misformatted domain name
    SE_NSRBADNAME            = 167,
    /// Unsupported address family
    SE_NSRBADFAMILY          = 168,
    /// Misformatted DNS reply
    SE_NSRBADRESP            = 169,
    /// Could not contact DNS servers
    SE_NSRCONNREFUSED        = 170,
    /// Timeout while contacting DNS servers
    SE_NSRTIMEOUT            = 171,
    /// End of file
    SE_NSROF                 = 172,
    /// Error reading file
    SE_NSRFILE               = 173,
    /// Out of memory
    SE_NSRNOMEM              = 174,
    /// Application terminated lookup
    SE_NSRDESTRUCTION        = 175,
    /// Domain name is too long
    SE_NSRQUERYDOMAINTOOLONG = 176,
    /// Domain name is too long
    SE_NSRCNAMELOOP          = 177,
} SyscallError;

/// O: read only
#define O_RDONLY            00
/// O: write only
#define O_WRONLY            01
/// O: read and write
#define O_RDWR              02

/// PROT: page can not be accessed
#define PROT_NONE           0x0
/// PROT: page can be read
#define PROT_READ           0x1
/// PROT: page can be written
#define PROT_WRITE          0x2
/// PROT: page can be executed
#define PROT_EXEC           0x4
/// PROT: page may be used for atomic ops
#define PROT_SEM            0x8
/// PROT: mprotect flag: extend change to start of growsdown vma
#define PROT_GROWSDOWN      0x01000000
/// PROT: mprotect flag: extend change to end of growsup vma
#define PROT_GROWSUP        0x02000000

/// Share changes
#define MAP_SHARED          0x01
/// Changes are private
#define MAP_PRIVATE         0x02
/// share + validate extension flags
#define MAP_SHARED_VALIDATE 0x03
/// Mask for type of mapping
#define MAP_TYPE            0x0f
/// Interpret addr exactly
#define MAP_FIXED           0x10
/// don't use a file
#define MAP_ANONYMOUS       0x20
// MAP_ 0x0100 - 0x4000 flags are per architecture
/// populate (prefault) pagetables
#define MAP_POPULATE        0x8000
/// do not block on IO
#define MAP_NONBLOCK        0x10000
/// give out an address that is best suited for process/thread stacks
#define MAP_STACK           0x20000
/// create a huge page mapping
#define MAP_HUGETLB         0x40000
/// perform synchronous page faults for the mapping
#define MAP_SYNC            0x80000
/// MAP_FIXED which doesn't unmap underlying mapping
#define MAP_FIXED_NOREPLACE 0x100000
/// For anonymous mmap, memory could be uninitialized
#define MAP_UNINITIALIZED   0x4000000

struct stat {
    unsigned short st_dev;
    unsigned short st_ino;
    unsigned short st_mode;
    unsigned short st_nlink;
    unsigned short st_uid;
    unsigned short st_gid;
    unsigned short st_rdev;
    unsigned long st_size;
    unsigned long st_atime;
    unsigned long st_mtime;
    unsigned long st_ctime;
};

/// Get the errno from a syscall return value. Zero means no error.
FNDECL_PREFIX SyscallError linux_get_syserrno(usize r) {
    i64 signed_r = (i64)r;
    return (SyscallError)((signed_r > -4096 && signed_r < 0) ? -signed_r : 0);
}

#define SYSCALL(type, n, ...) syscall##n(type, __VA_ARGS__)
