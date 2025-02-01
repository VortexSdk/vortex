#pragma once

#include "../../metap/diagnostics.hpp"
#include "../../numbers.hpp"

DIAG_IGNORE_CLANG_PUSH("-Weverything")
DIAG_IGNORE_GCC_PUSH("-Wall", "-Wextra")

#include <asm-generic/poll.h>
#include <asm-generic/siginfo.h>
#include <asm-generic/socket.h>
#include <asm/signal.h>
#include <asm/stat.h>
#include <asm/statfs.h>
#include <asm/unistd_64.h>
#include <linux/aio_abi.h>
#include <linux/capability.h>
#include <linux/eventpoll.h>
#include <linux/fs.h>
#include <linux/futex.h>
#include <linux/in.h>
#include <linux/inotify.h>
#include <linux/io_uring.h>
#include <linux/kexec.h>
#include <linux/landlock.h>
#include <linux/lsm.h>
#include <linux/mman.h>
#include <linux/mqueue.h>
#include <linux/msg.h>
#include <linux/openat2.h>
#include <linux/perf_event.h>
#include <linux/posix_types.h>
#include <linux/resource.h>
#include <linux/rseq.h>
#include <linux/sched.h>
#include <linux/sched/types.h>
#include <linux/sem.h>
#include <linux/shm.h>
#include <linux/signal.h>
#include <linux/signalfd.h>
#include <linux/socket.h>
#include <linux/stat.h>
#include <linux/sysinfo.h>
#include <linux/time.h>
#include <linux/time_types.h>
#include <linux/times.h>
#include <linux/timex.h>
#include <linux/uio.h>
#include <linux/utime.h>
#include <linux/utsname.h>

#define INVALID_FD 4294967295_u32

struct Fd {
    unsigned int d;

    Fd() {
        d = INVALID_FD;
    }
    Fd(int _d) {
        d = static_cast<int>(_d);
    }
    Fd(long _d) {
        d = static_cast<int>(_d);
    }
    __attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow")))
    Fd(unsigned int _d) {
        d = static_cast<int>(_d);
    }
    __attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow")))
    Fd(unsigned long _d) {
        d = static_cast<int>(_d);
    }

    operator int() {
        return static_cast<int>(d);
    }
    __attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow"))
    ) operator unsigned int() {
        return static_cast<unsigned int>(d);
    }
    operator long() {
        return static_cast<long>(d);
    }
    __attribute__((no_sanitize("implicit-integer-sign-change", "unsigned-integer-overflow"))
    ) operator unsigned long() {
        return static_cast<unsigned long>(d);
    }
};

using fd_set       = __kernel_fd_set;
using off_t        = __kernel_off_t;
using loff_t       = __kernel_loff_t;
using pid_t        = __kernel_pid_t;
using uid_t        = __kernel_uid_t;
using gid_t        = __kernel_gid_t;
using qid_t        = __kernel_uid32_t;
using mqd_t        = __kernel_mqd_t;
using key_t        = __kernel_key_t;
using timer_t      = __kernel_timer_t;
using clockid_t    = __kernel_clockid_t;
using mode_t       = __kernel_mode_t;
using umode_t      = unsigned short;
using sigset_t     = unsigned long;
using key_serial_t = i32;
using socklen_t    = u32;
using id_t         = u32;
using rwf_t        = __kernel_rwf_t;
using sockaddr     = __kernel_sockaddr_storage;

#define S_IFDIR    0040000
#define S_IFCHR    0020000
#define S_IFBLK    0060000
#define S_IFREG    0100000
#define S_IFIFO    0010000
#define S_IFLNK    0120000
#define S_IFSOCK   0140000

#define S_IRWXU    00700
#define S_IRUSR    00400
#define S_IWUSR    00200
#define S_IXUSR    00100

#define S_IRWXG    00070
#define S_IRGRP    00040
#define S_IWGRP    00020
#define S_IXGRP    00010

#define S_IRWXO    00007
#define S_IROTH    00004
#define S_IWOTH    00002
#define S_IXOTH    00001

/* dirent types */
#define DT_UNKNOWN 0x0
#define DT_FIFO    0x1
#define DT_CHR     0x2
#define DT_DIR     0x4
#define DT_BLK     0x6
#define DT_REG     0x8
#define DT_LNK     0xa
#define DT_SOCK    0xc

/* commonly an fd_set represents 256 FDs */
#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

/* PATH_MAX and MAXPATHLEN are often used and found with plenty of different
 * values.
 */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN (PATH_MAX)
#endif

/* flags for mmap */
#ifndef MAP_FAILED
#define MAP_FAILED ((void *)-1)
#endif

/* whence values for lseek() */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

using statx_t      = struct statx;

using __aio_sigset = struct {
    const sigset_t *sigmask;
    usize sigsetsize;
};
using sched_param = struct {
    int sched_priority;
};
using getcpu_cache = struct {
    unsigned long blob [128 / sizeof(long)];
};
using linux_dirent64 = struct {
    u64 d_ino;
    i64 d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name [];
};
using user_msghdr = struct {
    /// ptr to socket address structure
    void *msg_name;
    /// size of socket address structure
    int msg_namelen;
    /// scatter/gather array
    iovec *msg_iov;
    /// # elements in msg_iov
    __kernel_size_t msg_iovlen;
    /// ancillary data
    void *msg_control;
    /// ancillary data buffer length
    __kernel_size_t msg_controllen;
    /// flags on received message
    unsigned int msg_flags;
};
using mmsghdr = struct {
    user_msghdr msg_hdr;
    unsigned int msg_len;
};
using file_handle = struct {
    u32 handle_bytes;
    int handle_type;
    /// file identifier
    unsigned char f_handle [] __counted_by(handle_bytes);
};

using msghdr = user_msghdr;

DIAG_IGNORE_POP
