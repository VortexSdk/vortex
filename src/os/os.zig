/// A wrapper around syscalls that uses the right module for the OS and CPU kind
const builtin = @import("builtin");
pub const linux = @import("linux/linux.zig");
pub const BufPrinter = @import("printer.zig").BufPrinter;

/// Page size based on the OS.
pub const page_size = switch (builtin.cpu.arch) {
    .wasm32, .wasm64 => 64 * 1024,
    .aarch64 => switch (builtin.os.tag) {
        .macos, .ios, .watchos, .tvos => 16 * 1024,
        else => 4 * 1024,
    },
    .sparc64 => 8 * 1024,
    else => 4 * 1024,
};

// Exits the program.
pub fn exit(status: u8) void {
    @setRuntimeSafety(false);

    if (builtin.os.tag == .linux) {
        _ = linux.syscall(.exit_group, .{
            @as(usize, @bitCast(@as(isize, status))),
        }) catch {};
    }
}

// Prints to stdout.
pub fn print(buf: []const u8) void {
    @setRuntimeSafety(false);

    if (builtin.os.tag == .linux) {
        _ = linux.syscall(.write, .{
            0,
            buf.ptr,
            buf.len,
        }) catch {};
    }
}
