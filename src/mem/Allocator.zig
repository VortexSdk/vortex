const math = @import("../math.zig");

/// This is an allocator interface which relies on storing data with 8-byte alignment. That means types
/// that aren't aligned are stored with padding.
const Allocator = @This();

pub const AllocErr = error{OutOfMemory};
pub const ResizeErr = error{FailedToResize};

ptr: *anyopaque,
/// buf: ptr and len: non-zero and new_len: zero => free.
/// buf: null and len: non-zero and new_len: zero => alloc.
/// buf: ptr and len: non-zero and new_len: non-zero => realloc.
realloc: *const fn (ctx: *anyopaque, buf: ?[*]align(8) u8, len: usize, new_len: usize) ?[*]align(8) u8,

/// Aligns forward all types to make everything align on 8 bytes.
fn align_len(size: comptime_int, len: usize) usize {
    return math.alignForward(usize, size * len, 8);
}

/// Allocates a slice. Types that aren't aligned on 8 bytes will be stored with padding.
pub fn alloc(self: Allocator, comptime T: type, len: usize) AllocErr![]T {
    if (self.realloc(self.ptr, null, align_len(@sizeOf(T), len), 0)) |buf| {
        return @as([*]T, @ptrFromInt(@intFromPtr(buf)))[0..len];
    }

    return AllocErr.OutOfMemory;
}

/// Allocates an element.
pub fn create(self: Allocator, comptime T: type) AllocErr!T {
    return (try self.alloc(T, 1))[0];
}

/// Resizes the slice in place.
pub fn resize(self: Allocator, comptime T: type, buf: []T, new_len: usize) ResizeErr![]T {
    if (self.realloc(
        self.ptr,
        @as([*]align(8) u8, @ptrFromInt(@intFromPtr(buf.ptr))),
        buf.len,
        align_len(@sizeOf(T), new_len),
    )) |_| return buf[0..new_len];

    return ResizeErr.FailedToResize;
}

/// Resizes the slice in place or allocates a new slice if failed.
pub fn resize_or_alloc(self: Allocator, comptime T: type, buf: []T, new_size: usize) AllocErr![]T {
    if (self.resize(T, buf, new_size)) |resized| {
        return resized;
    } else |_| {
        self.free(buf);
        const allocated = try self.alloc(T, new_size);
        @memcpy(allocated[0..buf.len], buf);
        return allocated;
    }
}

/// Duplicates the slice.
pub fn dupe(self: Allocator, comptime T: type, buf: []const T) AllocErr![]T {
    const new_buf = try self.alloc(T, buf.len);
    @memcpy(new_buf, buf);

    return new_buf;
}

/// Frees the slice.
pub fn free(self: Allocator, buf: anytype) void {
    _ = self.realloc(
        self.ptr,
        @as([*]align(8) u8, @ptrFromInt(@intFromPtr(buf.ptr))),
        align_len(@sizeOf(@typeInfo(@TypeOf(buf)).Pointer.child), buf.len),
        0,
    );
}
