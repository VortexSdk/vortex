const math = @import("../math.zig");
const Allocator = @import("Allocator.zig");

mem: []u8,
pos: usize = 0,

/// A simple arena implementation.
const ArenaAllocator = @This();

pub fn unlikely() void {
    @setCold(true);
}

/// Initializes an arena from a memory slice.
pub fn init(mem: []u8) ArenaAllocator {
    return ArenaAllocator{ .mem = mem, .pos = 0 };
}

fn realloc(ctx: *anyopaque, buf: ?[*]align(8) u8, len: usize, new_len: usize) ?[*]align(8) u8 {
    const self: *ArenaAllocator = @alignCast(@ptrCast(ctx));
    if (buf == null) {
        // Alloc
        const new_pos = self.pos + len;

        if (new_pos > self.mem.len) {
            unlikely();
            return null;
        }

        const ret: [*]align(8) u8 = @alignCast(@ptrCast(&self.mem[self.pos]));
        self.pos = new_pos;
        return ret;
    } else if (new_len != 0) {
        // Resize
        if (@intFromPtr(self.mem[self.pos..].ptr) == (@intFromPtr(buf) + len)) {
            const new_pos = self.pos + (new_len - len);

            if (new_pos < self.mem.len) {
                self.pos = new_pos;
                return buf;
            } else unlikely();
        } // Falls to `return null;` if failes.
    }

    // Free
    return null;
}

/// Returns an allocator interface.
pub fn allocator(self: *ArenaAllocator) Allocator {
    return Allocator{ .ptr = self, .realloc = realloc };
}

/// Resets the Arena.
pub fn reset(self: *ArenaAllocator) void {
    self.pos = 0;
}
