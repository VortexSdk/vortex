// Export Allocators
pub const PageAllocator = @import("PageAllocator.zig");
pub const Allocator = @import("allocator/Allocator.zig");
pub const ArrayList = @import("ArrayList.zig").ArrayList;

/// Returns the length of a null-terminated string.
pub fn strlen(ptr: [*:0]const u8) usize {
    var i: usize = 0;
    while (ptr[i] != 0) i += 1;
    return i;
}
/// Converts a null-terminated string to a buffer.
pub fn span(ptr: [*:0]const u8) []const u8 {
    return ptr[0..strlen(ptr)];
}

inline fn if_scalar_unequal(comptime T: type, left: []const T, right: []const T) bool {
    if (@typeInfo(T) == .Float or @typeInfo(T) == .Int) {
        if (left[0] != right[0]) {
            return true;
        }
    }
    return false;
}
pub fn eql_nocheck(comptime T: type, a: []const T, b: []const T) bool {
    for (a, b) |a_elem, b_elem| {
        if (a_elem != b_elem) return false;
    }
    return true;
}
/// Checks if a and b are equal.
pub fn eql(comptime T: type, a: []const T, b: []const T) bool {
    if (a.len != b.len) return false;
    if (a.ptr == b.ptr) return true;
    if (a.len == 0) return true;
    if (if_scalar_unequal(T, a, b)) return false;

    return eql_nocheck(T, a, b);
}
