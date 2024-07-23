const os = @import("../os/os.zig");
const math = @import("../math.zig");
const queues = @import("queues.zig");
const posix = @import("../mem/posix.zig");

const Statx = os.linux.Statx;
const Fnctl = os.linux.Fnctl;
const IoUring = os.linux.IoUring;

pub const SubmissionQueue = queues.SubmissionQueue;
pub const CompletionQueue = queues.CompletionQueue;

const Uring = @This();

fd: i32,
sq: SubmissionQueue,
cq: CompletionQueue,
flags: IoUring.SetupFlags,
features: IoUring.ParamsFeatures,

// Three somewhat useful setup flags:
// SQPOLL
// NO_SQARRAY
// SINGLE_ISSUER

/// Initiates an IoUring interface. The number of entries should be a power of two.
pub fn init(e: u32, flags: IoUring.SetupFlags, thread_idle: u32, thread_cpu: u32) os.linux.Error!Uring {
    @setRuntimeSafety(false);

    const entries = math.ceilPowerOfTwo(e);

    // Only these three fields are used to configure io_uring at startup. We're making sure the rest of the
    // fields are zeroed out to be filled by the kerenl as the result.
    var p = IoUring.Params{
        .flags = flags,
        .sq_thread_cpu = thread_cpu,
        .sq_thread_idle = thread_idle,
    };
    const fd = @as(i32, @intCast(try os.linux.syscall(.io_uring_setup, .{
        entries, &p,
    })));

    //     since kernel 5.19
    if (!p.features.nodrop or
        // since kernel 5.17
        !p.features.cqe_skip or
        // since kernel 5.11
        !p.features.sqpoll_nonfixed or
        // since kernel 5.7
        !p.features.fast_poll or
        // since kernel 5.4
        !p.features.single_mmap) return os.linux.Error.NOSYS;

    var sq = try SubmissionQueue.init(fd, p);
    errdefer sq.deinit();
    var cq = CompletionQueue.init(p, sq);
    errdefer cq.deinit();

    return Uring{
        .fd = fd,
        .sq = sq,
        .cq = cq,
        .flags = p.flags,
        .features = p.features,
    };
}

/// Deinitiates the IoUring interface.
pub fn deinit(self: *Uring) void {
    @setRuntimeSafety(false);

    // Important ordering.
    self.cq.deinit();
    self.sq.deinit();
    posix.close(self.fd) catch {};

    self.fd = -1;
}
