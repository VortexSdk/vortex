const builtin = @import("builtin");
const os = @import("../os/os.zig");
const math = @import("../math.zig");
const Allocator = @import("allocator/Allocator.zig");

const PageAllocatorInitError = error{
    FailedToInitialize,
};

mem: []align(os.page_size) u8,

/// A simple page allocator.
const PageAllocator = @This();

/// Allocates n pages.
pub fn init(n: usize) PageAllocatorInitError!PageAllocator {
    @setRuntimeSafety(false);
    const len = n * os.page_size;
    if (builtin.os.tag == .linux) {
        const mmapsys_res = os.syscall.syscall(.mmap, .{
            @as(usize, 0),
            len,
            os.syscall.PROT.READ | os.syscall.PROT.WRITE,
            os.syscall.MAP.ANONYMOUS | os.syscall.MAP.PRIVATE,
            0,
            @as(u64, 0),
        });

        if (os.syscall.get_errno(mmapsys_res) == .SUCCESS) {
            return PageAllocator{
                .mem = @as(
                    [*]align(os.page_size) u8,
                    @ptrFromInt(mmapsys_res),
                )[0..len],
            };
        }

        return PageAllocatorInitError.FailedToInitialize;
    } else @compileError("Unsupported OS/CPU!");
}

/// Deallocates all the pages.
pub fn deinit(self: *PageAllocator) void {
    @setRuntimeSafety(false);
    if (builtin.os.tag == .linux) {
        _ = os.syscall.syscall(.munmap, .{ self.mem.ptr, self.mem.len });
    }
}
