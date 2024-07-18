const std = @import("std");
const vortex = @import("vortex");

const mem = vortex.mem;
const testing = std.testing;

test "AllocatorDuplicate" {
    var pages1 = try mem.PageAllocator.init(2);
    defer pages1.deinit();
    var arena1 = mem.Allocator.init(pages1.mem, .arena);
    _ = try arena1.alloc(u128, 12);
    _ = try arena1.alloc(u64, 14);

    var pages2 = try mem.PageAllocator.init(4);
    defer pages2.deinit();
    var arena2 = mem.Allocator.init(pages2.mem, .arena);

    const duplicate = try arena2.dupe(u8, pages1.mem);
    try testing.expect(mem.eql(u8, pages1.mem, duplicate));
}

test "PageAllocator" {
    var pages = try mem.PageAllocator.init(84);
    defer pages.deinit();

    try testing.expect(pages.mem.len == vortex.os.page_size * 84);
}

test "ArenaAllocator" {
    var pages = try mem.PageAllocator.init(84);
    defer pages.deinit();
    var arena = mem.Allocator.init(pages.mem, .arena);

    _ = try arena.alloc(u128, 64);
    _ = try arena.alloc(u64, 54);

    try testing.expect(arena.ctx.arena.pos ==
        (vortex.math.alignForward(usize, @sizeOf(u128) * 64, 8) +
        vortex.math.alignForward(usize, @sizeOf(u64) * 54, 8)));
}

test "FrelistAllocator" {
    var pages = try mem.PageAllocator.init(1);
    defer pages.deinit();
    var freelist = mem.Allocator.init(pages.mem, .freelist);

    try testing.expect(freelist.ctx.freelist.freelist == null);

    const allocated1 = try freelist.alloc(u8, 64);
    const allocated2 = try freelist.alloc(u128, 64);
    freelist.free(allocated1);
    freelist.free(allocated2);

    try testing.expect(freelist.ctx.freelist.freelist != null);
    try testing.expect(freelist.ctx.freelist.freelist.?.next != null);
    try testing.expect(freelist.ctx.freelist.freelist.?.len == @sizeOf(u128) * 64);
    try testing.expect(freelist.ctx.freelist.freelist.?.next.?.len == @sizeOf(u8) * 64);
}

test "ArrayList" {
    var pages = try mem.PageAllocator.init(5);
    defer pages.deinit();
    var arena = mem.Allocator.init(pages.mem, .arena);
    var arraylist = try mem.ArrayList(usize).init(&arena);

    inline for (0..30) |idx| {
        try testing.expect(arraylist.pos == idx);
        try arraylist.push(idx);
    }

    inline for (0..30) |idx| {
        try testing.expect(arraylist.get(idx).? == idx);
    }
}
