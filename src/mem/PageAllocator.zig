const builtin = @import("builtin");
const posix = @import("posix.zig");
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

    const size = n * os.page_size;
    if (builtin.os.tag == .linux) {
        return PageAllocator{
            .mem = posix.mmap(
                u8,
                null,
                size,
                os.linux.Mman.Mprotect{ .write = true, .read = true },
                os.linux.Mman.Mmap{ .visibility = .private, .anonymous = true },
                -1,
                0,
            ) catch return PageAllocatorInitError.FailedToInitialize,
        };
    }
}

/// Deallocates all the pages.
pub fn deinit(self: *PageAllocator) void {
    @setRuntimeSafety(false);

    if (builtin.os.tag == .linux) posix.munmap(self.mem);
}
