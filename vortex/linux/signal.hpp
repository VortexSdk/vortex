#pragma once

#include "syscall/SysRes.hpp"
#include "syscall/wrapper.hpp"
#include "syscall/wrapperHelper.hpp"
#include "vortex/linux/io_uring.hpp"
#include "vortex/mem/utils.hpp"

struct SignalFd {
    Fd fd;
    signalfd_siginfo buf;

    SignalFd() : fd(Fd(-1)) {
        buf = zeroed<signalfd_siginfo>();
    }

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
        self->fd  = TRY(signalfd(-1, &mask, sizeof(sigset_t), 0));
        self->buf = zeroed<signalfd_siginfo>();

        return self->io_uring_read(r, user_data);
    }

    SysRes<None> deinit(this SignalFd &self) {
        TRY(close(self.fd));
        return None();
    }
};
