pub const math = @import("math.zig");
pub const mem = @import("mem/mem.zig");
pub const os = @import("os/os.zig");
pub const start = @import("os/start.zig");

export fn hi() void {
    os.print("hi\n");
    os.exit(0);
}
