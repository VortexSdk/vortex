// Manually translated uapi's io_uring header from Linux 6.9.
// Reference: https://github.com/torvalds/linux/blob/a38297e3fb012ddfa7ce0321a7e5a8daeb1872b6/include/uapi/asm-generic/signal.h

pub const _nsig: comptime_int = 64;
pub const _nsig_bpw: comptime_int = @bitSizeOf(c_long);
pub const _nsig_words: comptime_int = (_nsig / _nsig_bpw);

pub const __sighandler_t = ?*const fn (signo: c_int) void;

pub const signal = enum(u32) {
    sighup = 1,
    sigint = 2,
    sigquit = 3,
    sigill = 4,
    sigtrap = 5,
    sigabrt = 6,
    sigiot = 6,
    sigbus = 7,
    sigfpe = 8,
    sigkill = 9,
    sigusr1 = 10,
    sigsegv = 11,
    sigusr2 = 12,
    sigpipe = 13,
    sigalrm = 14,
    sigterm = 15,
    sigstkflt = 16,
    sigchld = 17,
    sigcont = 18,
    sigstop = 19,
    sigtstp = 20,
    sigttin = 21,
    sigttou = 22,
    sigurg = 23,
    sigxcpu = 24,
    sigxfsz = 25,
    sigvtalrm = 26,
    sigprof = 27,
    sigwinch = 28,
    sigio = 29,

    // siglost  29,

    sigpwr = 30,
    sigsys = 31,
    sigunused = 31,

    // these should not be considered constatns from userland.
    sigrtmin = 32,
};

pub const sigrtmax: u32 = _nsig;

pub const sigpoll = signal.sigio;

pub const sigstksz: comptime_int = 2048;
pub const minsigstksz: comptime_int = 2048;

pub const sigset_t = extern struct {
    sig: [_nsig_words]c_ulong = .{0} ** _nsig_words,
};

pub const sigaction = extern struct {
    handler: __sighandler_t = null,
    flags: c_ulong = 0,
    sa_mask: sigset_t = .{},
};

pub const stack_t = extern struct {
    p: ?*anyopaque = null,
    flags: c_int = 0,
    size: usize = 0,
};
