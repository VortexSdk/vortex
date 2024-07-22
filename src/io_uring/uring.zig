pub const os = @import("../os/os.zig");
pub const bindings = @import("bindings.zig");

const Uring = @This();

fd: i32,

fn io_uring_setup(entries: u32, p: *bindings.io_uring_params) os.linux.Error!void {
    return os.linux.syscall(.io_uring_setup, .{
        entries, p,
    });
}
fn io_uring_register(
    fd: i32,
    op: bindings.io_uring_reg,
    arg: ?*anyopaque,
    nr_args: c_uint,
) os.linux.Error!void {
    return os.linux.syscall(.io_uring_register, .{
        fd, op, @as([*]void, @ptrCast(arg)), nr_args,
    });
}
fn io_uring_enter(
    fd: i32,
    to_submit: u32,
    min_complete: u32,
    flags: bindings.io_uring_enter_flags,
    argp: ?*anyopaque,
    argsz: usize,
) os.linux.Error!void {
    return os.linux.syscall(.io_uring_enter, .{
        fd,
        to_submit,
        min_complete,
        @as(u32, @bitCast(flags)),
        argp,
        argsz,
    });
}

pub fn init(entries: u32) Uring {
    var params: bindings.io_uring_params = undefined;
    io_uring_setup(entries, &params);
}
