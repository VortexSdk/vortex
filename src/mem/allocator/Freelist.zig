const math = @import("../../math.zig");
const Allocator = @import("Allocator.zig");

pub const FreeListNode = packed struct {
    len: usize,
    next: ?*FreeListNode,
};
pub const Context = struct { pos: usize = 0, freelist: ?*FreeListNode = null };

pub inline fn alloc(self: *Allocator, size: usize) ?[*]align(8) u8 {
    if (self.ctx.freelist.freelist) |freelist| {
        if (freelist.len >= size) {
            var freelist_slice: []u8 = undefined;
            freelist_slice.len = freelist.len;
            freelist_slice.ptr = @alignCast(@ptrCast(freelist));

            if (freelist.len - size != 0) {
                @memset(freelist_slice[size..freelist.len], 0);
            }

            self.ctx.freelist.freelist = freelist.next;

            return @alignCast(@ptrCast(freelist_slice[0..size].ptr));
        }
    }

    const new_pos = self.ctx.freelist.pos + size;
    if (new_pos > self.mem.len) return null;

    const ret: [*]align(8) u8 = @alignCast(@ptrCast(&self.mem[self.ctx.freelist.pos]));
    self.ctx.freelist.pos = new_pos;
    return ret;
}

pub inline fn resize(self: *Allocator, buf: ?[*]align(8) u8, size: usize, new_size: usize) ?[*]align(8) u8 {
    if (@intFromPtr(self.mem[self.ctx.freelist.pos..].ptr) == (@intFromPtr(buf) + size)) {
        const new_pos = self.ctx.freelist.pos + (new_size - size);
        if (new_pos < self.mem.len) {
            self.ctx.freelist.pos = new_pos;
            return buf;
        }
    }

    return null;
}

pub inline fn free(self: *Allocator, buf: ?[*]align(8) u8, size: usize) void {
    const freelist = @as(*FreeListNode, @alignCast(@ptrCast(buf)));

    if (size > 16) {
        freelist.*.len = @intCast(math.alignForward(usize, size, 8));
    } else {
        freelist.*.len = 16;
    }

    freelist.*.next = self.ctx.freelist.freelist;
    self.ctx.freelist.freelist = freelist;
}
