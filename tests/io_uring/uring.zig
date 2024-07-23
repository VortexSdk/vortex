const std = @import("std");
const vortex = @import("vortex");

pub usingnamespace vortex.start;

const linux = vortex.os.linux;

pub fn main(_: [][*:0]u8) !void {
    var uring = try vortex.IoUring.init(8, .{}, 0, 0);
    defer uring.deinit();
}
