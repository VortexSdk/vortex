const os = @import("../os/os.zig");
const math = @import("../math.zig");
const queues = @import("queues.zig");
const posix = @import("../mem/posix.zig");

const Uio = os.linux.Uio;
const Statx = os.linux.Statx;
const Fnctl = os.linux.Fnctl;
const Socket = os.linux.Socket;
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

inline fn load_sq_flags(self: *Uring) IoUring.SqringOffsetsFlags {
    @setRuntimeSafety(false);

    return @atomicLoad(IoUring.SqringOffsetsFlags, self.sq.flags, .unordered);
}

pub fn cq_ring_needs_flush(self: *Uring) bool {
    @setRuntimeSafety(false);

    const f = self.load_sq_flags();
    return f.cq_overflow or f.taskrun;
}
pub fn cq_ring_needs_enter(self: *Uring) bool {
    @setRuntimeSafety(false);

    return self.flags.iopoll or self.cq_ring_needs_flush();
}

pub fn sq_ring_needs_enter(self: *Uring, flags: *IoUring.EnterFlags) bool {
    @setRuntimeSafety(false);

    if (!self.flags.sqpoll) return true;

    if (self.load_sq_flags().need_wakeup) {
        flags.sq_wakeup = true;
        return true;
    }

    return false;
}

pub fn sq_ready(self: *Uring) u32 {
    @setRuntimeSafety(false);

    // https://github.com/axboe/liburing/issues/92.
    // Uses the shared ring state (i.e. head and not sqe_head) to not go out of sync.
    return self.sq.sqe_tail -% @atomicLoad(u32, self.sq.head, .acquire);
}

pub fn flush_sq(self: *Uring) u32 {
    if (self.sq.sqe_head != self.sq.sqe_tail) {
        // Fill in SQEs that have been queued up. Adding them to the kernel ring.
        const to_submit = self.sq.sqe_tail -% self.sq.sqe_head;
        var tail = self.sq.tail.*;

        var i: usize = 0;
        while (i < to_submit) : (i += 1) {
            self.sq.array[tail & self.sq.mask] = self.sq.sqe_head & self.sq.mask;

            tail +%= 1;
            self.sq.sqe_head +%= 1;
        }

        @atomicStore(u32, self.sq.tail, tail, .release);
    }

    return self.sq_ready();
}

fn enter(
    self: *Uring,
    to_submit: u32,
    min_complete: u32,
    flags: IoUring.EnterFlags,
    sig: ?*os.linux.Signal.sigset_t,
) os.linux.Error!usize {
    @setRuntimeSafety(false);

    return os.linux.syscall(.io_uring_enter, .{
        self.fd, to_submit, min_complete, @as(u32, @bitCast(flags)), sig,
    });
}

pub fn submit_and_wait(self: *Uring, wait_nr: u32) os.linux.Error!usize {
    @setRuntimeSafety(false);

    const submitted = self.flush_sq();
    var flags = IoUring.EnterFlags{};
    if (self.sq_ring_needs_enter(&flags) or wait_nr > 0) {
        if (wait_nr > 0 or self.cq_ring_needs_enter()) {
            flags.getevents = true;
        }

        return self.enter(submitted, wait_nr, flags, null);
    }

    return submitted;
}

pub fn submit(self: *Uring) os.linux.Error!usize {
    @setRuntimeSafety(false);

    return self.submit_and_wait(0);
}

pub fn cq_ready(self: *Uring) u32 {
    @setRuntimeSafety(false);

    return @atomicLoad(u32, self.cq.tail, .acquire) -% self.cq.head.*;
}
pub fn cq_advance(self: *Uring, count: u32) void {
    @setRuntimeSafety(false);

    // Ensures the kernel only sees the new head value after the CQEs have been read.
    if (count > 0) @atomicStore(u32, self.cq.head, self.cq.head.* +% count, .release);
}

fn copy_cqes_ready(self: *Uring, cqes: []IoUring.Cqe) u32 {
    @setRuntimeSafety(false);

    const ready = self.cq_ready();
    const count = @min(cqes.len, ready);
    const head = self.cq.head.* & self.cq.mask;

    const n = @min(self.cq.cqes.len - head, count);
    @memcpy(cqes[0..n], self.cq.cqes[head..][0..n]);

    if (count > n) {
        const w = count - n;
        @memcpy(cqes[n..][0..w], self.cq.cqes[0..w]);
    }

    self.cq_advance(count);

    return count;
}
pub fn copy_cqes(self: *Uring, cqes: []IoUring.Cqe, wait_nr: u32) os.linux.Error!u32 {
    @setRuntimeSafety(false);

    const count = self.copy_cqes_ready(cqes);
    if (count > 0) return count;

    if (self.cq_ring_needs_flush() or wait_nr > 0) {
        _ = try self.enter(0, wait_nr, .{ .getevents = true }, null);

        return self.copy_cqes_ready(cqes);
    }

    return 0;
}
pub fn copy_cqe(ring: *Uring) os.linux.Error!IoUring.Cqe {
    var cqes: [1]IoUring.Cqe = undefined;
    while (true) {
        const count = try ring.copy_cqes(&cqes, 1);

        if (count > 0) return cqes[0];
    }
}

pub const QueueError = error{
    SubmissionQueueFull,
};

pub fn get_sqe(self: *Uring, op: IoUring.Op, user_data: u64) QueueError!*IoUring.Sqe {
    const head = @atomicLoad(u32, self.sq.head, .acquire);

    // Remember that these head and tail offsets wrap around every four billion operations.
    // We must therefore use wrapping addition and subtraction to avoid a runtime crash.
    const next = self.sq.sqe_tail +% 1;

    if (next -% head > self.sq.sqes.len) return QueueError.SubmissionQueueFull;

    const sqe = &self.sq.sqes[self.sq.sqe_tail & self.sq.mask];
    self.sq.sqe_tail = next;

    sqe.opcode = op;
    sqe.user_data = user_data;

    return sqe;
}

pub fn nop(self: *Uring, user_data: u64) QueueError!*IoUring.Sqe {
    return self.get_sqe(.nop, user_data);
}

pub fn preadv2(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const Uio.Iovec,
    off: i64,
    flags: c_int,
) QueueError!*IoUring.Sqe {
    const sqe = try self.get_sqe(.readv, user_data);
    sqe.fd = fd;
    sqe.off.off = @bitCast(off);
    sqe.op_flags.rw_flags = flags;
    sqe.len = @truncate(buffer.len);
    sqe.addr.addr = @intFromPtr(buffer);

    return sqe;
}
pub fn preadv(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const Uio.Iovec,
    off: i64,
) QueueError!*IoUring.Sqe {
    return self.preadv2(user_data, fd, buffer, off, 0);
}
pub fn readv(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const Uio.Iovec,
) QueueError!*IoUring.Sqe {
    return self.preadv(user_data, fd, buffer, 0);
}

pub fn pwritev2(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const Uio.Iovec,
    off: i64,
    flags: c_int,
) QueueError!*IoUring.Sqe {
    const sqe = try self.preadv2(user_data, fd, buffer, off, flags);
    sqe.opcode = .writev;

    return sqe;
}
pub fn pwritev(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const Uio.Iovec,
    off: i64,
) QueueError!*IoUring.Sqe {
    return self.preadv2(user_data, fd, buffer, off, 0);
}
pub fn writev(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const Uio.Iovec,
) QueueError!*IoUring.Sqe {
    return self.pwritev(user_data, fd, buffer, 0);
}

pub fn read(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []u8,
) QueueError!*IoUring.Sqe {
    const sqe = try self.get_sqe(.read, user_data);
    sqe.fd = fd;
    sqe.len = @truncate(buffer.len);
    sqe.addr.addr = @intFromPtr(buffer.ptr);

    return sqe;
}

pub fn write(
    self: *Uring,
    user_data: u64,
    fd: i32,
    buffer: []const u8,
) QueueError!*IoUring.Sqe {
    const sqe = try self.get_sqe(.write, user_data);
    sqe.fd = fd;
    sqe.len = @intCast(buffer.len);
    sqe.addr.addr = @intFromPtr(buffer.ptr);

    return sqe;
}

// pub fn accept(
//     self: *Uring,
//     user_data: u64,
//     fd: i32,
//     addr: ?*Socket.Storage,
//     addrlen: ?*,
//     flags: u32,
//     file_index: u32,
// ) QueueError!*IoUring.Sqe {
//     const sqe = try self.get_sqe(.write, user_data);
//     sqe.fd = fd;
//     sqe.len = @intCast(buffer.len);
//     sqe.addr.addr = @intFromPtr(buffer.ptr);

//     return sqe;
// }
