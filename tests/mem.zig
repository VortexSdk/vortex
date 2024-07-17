const std = @import("std");
const swiftzig = @import("swiftzig");

test "AllocatorDuplicate" {
    var pages1 = try swiftzig.mem.PageAllocator.init(2);
    defer pages1.deinit();
    var arena1 = swiftzig.mem.ArenaAllocator.init(pages1.mem);
    const allocator1 = arena1.allocator();
    _ = try allocator1.alloc(u128, 12);
    _ = try allocator1.alloc(u64, 14);

    var pages2 = try swiftzig.mem.PageAllocator.init(4);
    defer pages2.deinit();
    var arena2 = swiftzig.mem.ArenaAllocator.init(pages2.mem);
    const allocator2 = arena2.allocator();

    const duplicate = try allocator2.dupe(u8, pages1.mem);
    try std.testing.expect(swiftzig.mem.eql(u8, pages1.mem, duplicate));
}

test "PageAllocator" {
    var pages = try swiftzig.mem.PageAllocator.init(84);
    defer pages.deinit();

    try std.testing.expect(pages.mem.len == swiftzig.os.page_size * 84);
}

test "ArenaAllocator" {
    var pages = try swiftzig.mem.PageAllocator.init(84);
    defer pages.deinit();
    var arena = swiftzig.mem.ArenaAllocator.init(pages.mem);
    const allocator = arena.allocator();

    _ = try allocator.alloc(u128, 64);
    _ = try allocator.alloc(u64, 54);

    try std.testing.expect(arena.pos ==
        (swiftzig.math.alignForward(usize, @sizeOf(u128) * 64, 8) +
        swiftzig.math.alignForward(usize, @sizeOf(u64) * 54, 8)));
}

test "FrelistAllocator" {
    var pages = try swiftzig.mem.PageAllocator.init(1);
    defer pages.deinit();
    var freelist = swiftzig.mem.FreelistAllocator.init(pages.mem);
    var allocator = freelist.allocator();

    try std.testing.expect(freelist.freelist == null);

    const allocated1 = try allocator.alloc(u8, 64);
    const allocated2 = try allocator.alloc(u128, 64);
    allocator.free(allocated1);
    allocator.free(allocated2);

    try std.testing.expect(freelist.freelist != null);
    try std.testing.expect(freelist.freelist.?.next != null);
    try std.testing.expect(freelist.freelist.?.len == @sizeOf(u128) * 64);
    try std.testing.expect(freelist.freelist.?.next.?.len == @sizeOf(u8) * 64);
}

test "ArrayList" {
    var pages = try swiftzig.mem.PageAllocator.init(5);
    defer pages.deinit();
    var arena = swiftzig.mem.ArenaAllocator.init(pages.mem);
    const allocator = arena.allocator();
    var arraylist = try swiftzig.mem.ArrayList(usize).init(allocator);

    inline for (0..30) |idx| {
        try std.testing.expect(arraylist.pos == idx);
        try arraylist.push(idx);
    }

    inline for (0..30) |idx| {
        try std.testing.expect(arraylist.get(idx).? == idx);
    }
}
