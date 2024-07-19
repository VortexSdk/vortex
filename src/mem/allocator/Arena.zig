const Allocator = @import("Allocator.zig");

pub const Context = struct { pos: usize = 0 };

pub inline fn alloc(self: *Allocator, size: usize) ?[*]align(8) u8 {
    @setRuntimeSafety(false);

    const new_pos = self.ctx.arena.pos + size;
    if (new_pos > self.mem.len) return null;

    const ret: [*]align(8) u8 = @alignCast(@ptrCast(&self.mem[self.ctx.arena.pos]));
    self.ctx.arena.pos = new_pos;

    return ret;
}

pub inline fn resize(self: *Allocator, buf: ?[*]align(8) u8, size: usize, new_size: usize) ?[*]align(8) u8 {
    @setRuntimeSafety(false);

    if (@intFromPtr(self.mem[self.ctx.arena.pos..].ptr) == (@intFromPtr(buf) + size)) {
        const new_pos = self.ctx.arena.pos + (new_size - size);
        if (new_pos < self.mem.len) {
            self.ctx.arena.pos = new_pos;
            return buf;
        }
    }

    return null;
}

pub inline fn free(_: *Allocator, _: ?[*]align(8) u8, _: usize) void {}
