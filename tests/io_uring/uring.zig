const std = @import("std");
const vortex = @import("vortex");

// pub usingnamespace vortex.start;

const mem = vortex.mem;
const IoUring = vortex.IoUring;
const linux = vortex.os.linux;

// pub fn main(_: [][*:0]u8) !void {}

pub fn main() !void {
    var pages = try mem.PageAllocator.init(10);
    defer pages.deinit();
    var allocator = mem.Allocator.init(pages.mem, .arena);

    var uring = try vortex.IoUring.init(4, .{ .sqpoll = true }, 50 * 1000, 0);
    defer uring.deinit();

    // var b: [1024]u8 = undefined;
    _ = try uring.write(28, 0, "Wtf\n");
    _ = try uring.write(25, 0, "Wt5f\n");

    _ = try uring.submit_and_wait(2);

    const cqe = try allocator.alloc(linux.IoUring.Cqe, 2);
    _ = try uring.copy_cqes(cqe, 0);

    std.debug.print("{any}\n", .{cqe[0]});
    std.debug.print("{any}\n", .{cqe[1]});
}
