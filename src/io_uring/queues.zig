const os = @import("../os/os.zig");
const mem = @import("../mem/mem.zig");
const posix = @import("../mem/posix.zig");

const IoUring = os.linux.IoUring;

inline fn off_cast(m: []align(os.page_size) u8, idx: u32) *u32 {
    return @as(*u32, @ptrCast(@alignCast(&m[idx])));
}

pub const SubmissionQueue = struct {
    head: *u32,
    tail: *u32,
    array: []u32,
    dropped: *u32,
    mask: u32,
    flags: *IoUring.SqringOffsetsFlags,

    sqe_head: u32 = 0,
    sqe_tail: u32 = 0,

    mem: []align(os.page_size) u8,
    sqes: []align(os.page_size) IoUring.Sqe,

    pub inline fn init(fd: i32, p: IoUring.Params) os.linux.Error!SubmissionQueue {
        @setRuntimeSafety(false);

        const off = p.sq_off;

        const m = try posix.mmap(
            u8,
            null,
            @max(
                off.array + p.sq_entries * @sizeOf(u32),
                p.cq_off.cqes + p.cq_entries * @sizeOf(IoUring.Sqe),
            ),
            os.linux.Mman.Mprotect{ .read = true, .write = true },
            os.linux.Mman.Mmap{ .visibility = .shared, .populate = true },
            fd,
            IoUring.mmap_offsets.sq_ring,
        );
        errdefer posix.munmap(m);

        // The purpose of the `sqes` and `array` indirection is to allow the application to preallocate static
        // linux.io_uring_sqe entries and replay them when needed.
        const sqes = try posix.mmap(
            IoUring.Sqe,
            null,
            p.sq_entries,
            os.linux.Mman.Mprotect{ .read = true, .write = true },
            os.linux.Mman.Mmap{ .visibility = .shared, .populate = true },
            fd,
            IoUring.mmap_offsets.sqes,
        );
        errdefer posix.munmap(sqes);

        return SubmissionQueue{
            .mem = m,
            .sqes = sqes,
            .head = off_cast(m, off.head),
            .tail = off_cast(m, off.tail),
            .dropped = off_cast(m, off.dropped),
            .mask = off_cast(m, off.ring_mask).*,
            .flags = @ptrCast(off_cast(m, @bitCast(off.flags))),
            .array = @as([*]u32, @ptrCast(off_cast(m, off.array)))[0..p.sq_entries],
        };
    }

    pub inline fn deinit(self: *SubmissionQueue) void {
        @setRuntimeSafety(false);

        posix.munmap(self.mem);
        posix.munmap(self.sqes);
    }
};

pub const CompletionQueue = struct {
    head: *u32,
    tail: *u32,
    overflow: *u32,
    mask: u32,
    cqes: []IoUring.Cqe,

    pub inline fn init(p: IoUring.Params, sq: SubmissionQueue) CompletionQueue {
        @setRuntimeSafety(false);

        const m = sq.mem;
        const off = p.cq_off;
        return CompletionQueue{
            .head = off_cast(m, off.head),
            .tail = off_cast(m, off.tail),
            .mask = off_cast(m, off.ring_mask).*,
            .overflow = off_cast(m, off.overflow),
            .cqes = @as([*]IoUring.Cqe, @alignCast(@ptrCast(off_cast(m, off.cqes))))[0..p.cq_entries],
        };
    }

    pub inline fn deinit(_: *CompletionQueue) void {}
};
