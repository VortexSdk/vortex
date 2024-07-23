const std = @import("std");

pub fn assert(T: type, h: anytype) !void {
    const int = @Type(.{ .Int = .{ .signedness = .unsigned, .bits = @bitSizeOf(T) } });

    try std.testing.expect(@as(int, @bitCast(T{})) == 0);

    inline for (@typeInfo(T).Struct.fields) |d| {
        if (d.type != bool) continue;

        var s = T{};
        @field(s, d.name) = true;

        try std.testing.expect(@as((int), @bitCast(s)) == @field(h, d.name));
    }
}
