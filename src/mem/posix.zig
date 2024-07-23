const mem = @import("mem.zig");
const os = @import("../os/os.zig");
const math = @import("../math.zig");

pub fn mmap(
    as: type,
    addr: ?*anyopaque,
    size: usize,
    protect: os.linux.Mman.Mprotect,
    map: os.linux.Mman.Mmap,
    fd: i32,
    off: usize,
) os.linux.Error![]align(os.page_size) as {
    @setRuntimeSafety(false);

    const s = size * @sizeOf(as);
    const m: [*]align(os.page_size) as = @ptrFromInt(try os.linux.syscall(.mmap, .{
        @intFromPtr(addr),
        s,
        protect,
        map,
        fd,
        off,
    }));

    return m[0..size];
}

pub fn munmap(m: anytype) void {
    @setRuntimeSafety(false);

    _ = os.linux.syscall(.munmap, .{ m.ptr, m.len * @sizeOf(@typeInfo(@TypeOf(m)).Pointer.child) }) catch {};
}

pub fn close(fd: i32) os.linux.Error!void {
    @setRuntimeSafety(false);

    _ = try os.linux.syscall(.close, .{@as(usize, @bitCast(@as(isize, fd)))});
}
