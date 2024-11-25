#pragma once

#include "../../diagnostics.hpp"
#include "../../metap/metap.hpp"
#include "../../numbers.hpp"

DIAG_IGNORE_CLANG_PUSH("-Weverything", "-Wreserved-identifier")
DIAG_IGNORE_GCC_PUSH("-Wall", "-Wextra")

#include <asm-generic/poll.h>
#include <asm-generic/siginfo.h>
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
#include <linux/io_uring.h>
#include <linux/kexec.h>
#include <linux/landlock.h>
#include <linux/lsm.h>
#include <linux/mman.h>
#include <linux/mqueue.h>
#include <linux/msg.h>
#include <linux/openat2.h>
#include <linux/perf_event.h>
#include <linux/resource.h>
#include <linux/rseq.h>
#include <linux/sched.h>
#include <linux/sched/types.h>
#include <linux/sem.h>
#include <linux/shm.h>
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
//
#include <linux/posix_types.h>

using FdI          = int;
using FdL          = long;
using FdU          = unsigned int;
using FdUL         = unsigned long;
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

typedef struct msghdr msghdr;
typedef struct statx statx_t;

template <typename From, typename To> To fd_cast(From value) {
    static_assert(
        is_same<From, FdI>::value || is_same<From, FdU>::value || is_same<From, FdL>::value ||
            is_same<From, FdUL>::value || is_same<To, FdI>::value || is_same<To, FdU>::value ||
            is_same<To, FdL>::value || is_same<To, FdUL>::value,
        "Invalid fd cast!"
    );

#ifndef __OPTIMIZE__
    if constexpr (!is_unsigned<From>::value && is_unsigned<To>::value) {
        assert(value >= 0, "Negative value cannot be converted to unsigned type!");
    }
#endif

    return static_cast<To>(value);
}

typedef struct {
    const sigset_t *sigmask;
    usize sigsetsize;
} __aio_sigset;
typedef struct {
    int sched_priority;
} sched_param;
typedef struct {
    unsigned long blob [128 / sizeof(long)];
} getcpu_cache;
typedef struct {
    u64 d_ino;
    i64 d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name [];
} linux_dirent64;
typedef struct {
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
} user_msghdr;
typedef struct {
    user_msghdr msg_hdr;
    unsigned int msg_len;
} mmsghdr;
typedef struct {
    u32 handle_bytes;
    int handle_type;
    /// file identifier
    unsigned char f_handle [] __counted_by(handle_bytes);
} file_handle;

DIAG_IGNORE_POP
