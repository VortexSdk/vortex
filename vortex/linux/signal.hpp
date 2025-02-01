#pragma once

#include "syscall/SysRes.hpp"
#include "syscall/wrapper.hpp"
#include "syscall/wrapperHelper.hpp"
#include "vortex/linux/io_uring.hpp"
#include "vortex/mem/utils.hpp"

struct SignalFd {
    Fd fd;
    signalfd_siginfo buf;

    static signalfd_siginfo zeroed_buf() {
        return {
            .ssi_signo     = 0_u32,
            .ssi_errno     = 0_i32,
            .ssi_code      = 0_u32,
            .ssi_pid       = 0_u32,
            .ssi_uid       = 0_u32,
            .ssi_fd        = 0_i32,
            .ssi_tid       = 0_u32,
            .ssi_band      = 0_u32,
            .ssi_overrun   = 0_u32,
            .ssi_trapno    = 0_u32,
            .ssi_status    = 0_i32,
            .ssi_int       = 0_i32,
            .ssi_ptr       = 0_u64,
            .ssi_utime     = 0_u64,
            .ssi_stime     = 0_u64,
            .ssi_addr      = 0_u64,
            .ssi_addr_lsb  = 0_u16,
            .__pad2        = 0_u16,
            .ssi_syscall   = 0_i32,
            .ssi_call_addr = 0_u64,
            .ssi_arch      = 0_u64,
            .__pad         = {0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8,
                              0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8,
                              0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8, 0_u8},
        };
    }

    SignalFd() : fd(Fd(-1)), buf(SignalFd::zeroed_buf()) {}

    static SysRes<SignalFd> init(sigset_t mask) {
        SignalFd self;
        sigset_t m = mask;

        rt_sigprocmask(SIG_BLOCK, &m, null, sizeof(sigset_t));
        self.fd = TRY(signalfd(-1, &m, sizeof(sigset_t), 0));

        return self;
    }

    SysRes<io_uring_sqe *> io_uring_read(this SignalFd &self, IoUring *r, u64 user_data) {
        return IO_TRY_ADD(r->read(user_data, self.fd, &self.buf, sizeof(signalfd_siginfo), false));
    }

    static SysRes<io_uring_sqe *>
    init_and_read(SignalFd *self, sigset_t mask, IoUring *r, u64 user_data) {
        rt_sigprocmask(SIG_BLOCK, &mask, null, sizeof(sigset_t));
        self->fd = TRY(signalfd(-1, &mask, sizeof(sigset_t), 0));
        memset(&self->buf, 0, sizeof(signalfd_siginfo));

        return self->io_uring_read(r, user_data);
    }

    SysRes<None> deinit(this SignalFd &self) {
        TRY(close(self.fd));
        return None();
    }
};
