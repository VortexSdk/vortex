// Export Allocators
pub const PageAllocator = @import("PageAllocator.zig");
pub const Allocator = @import("allocator/Allocator.zig");
pub const ArrayList = @import("ArrayList.zig").ArrayList;

/// Returns the length of a null-terminated string.
pub fn strlen(ptr: [*:0]const u8) usize {
    @setRuntimeSafety(false);

    var i: usize = 0;
    while (ptr[i] != 0) i += 1;
    return i;
}

/// Converts a null-terminated string to a buffer.
pub fn span(ptr: [*:0]const u8) []const u8 {
    @setRuntimeSafety(false);

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
    @setRuntimeSafety(false);

    for (a, b) |a_elem, b_elem| {
        if (a_elem != b_elem) return false;
    }

    return true;
}
/// Checks if a and b are equal.
pub fn eql(comptime T: type, a: []const T, b: []const T) bool {
    @setRuntimeSafety(false);

    if (a.len != b.len) return false;
    if (a.ptr == b.ptr) return true;
    if (a.len == 0) return true;
    if (if_scalar_unequal(T, a, b)) return false;

    return eql_nocheck(T, a, b);
}

/// Given a pointer to a single item, returns a slice of the underlying bytes, preserving pointer attributes.
pub fn asBytes(ptr: anytype) *[@sizeOf(@typeInfo(@TypeOf(ptr)).Pointer.child)]u8 {
    return @ptrCast(@alignCast(ptr));
}

/// Initializes an instance of T where the binary representation is all zeroes.
pub fn zeroes(comptime T: type) T {
    switch (@typeInfo(T)) {
        .Void => return {},
        .Bool => return false,
        .Optional, .Null => return null,
        .Enum => return @as(T, @enumFromInt(0)),
        .ComptimeInt, .Int, .ComptimeFloat, .Float => return @as(T, 0),
        .Struct => |struct_info| {
            if (@sizeOf(T) == 0) return undefined;
            if (struct_info.layout == .@"extern") {
                var item: T = undefined;
                @memset(asBytes(&item), 0);
                return item;
            } else {
                var structure: T = undefined;
                inline for (struct_info.fields) |field| {
                    if (!field.is_comptime) {
                        @field(structure, field.name) = zeroes(field.type);
                    }
                }
                return structure;
            }
        },
        .Pointer => |ptr_info| {
            switch (ptr_info.size) {
                .Slice => {
                    if (ptr_info.sentinel) |sentinel| {
                        if (ptr_info.child == u8 and @as(*const u8, @ptrCast(sentinel)).* == 0) {
                            return ""; // A special case for the most common use-case: null-terminated strings.
                        }
                        @compileError("Can't set a sentinel slice to zero. This would require allocating memory.");
                    } else {
                        return &[_]ptr_info.child{};
                    }
                },
                .C => {
                    return null;
                },
                .One, .Many => {
                    if (ptr_info.is_allowzero) return @ptrFromInt(0);
                    @compileError("Only nullable and allowzero pointers can be set to zero.");
                },
            }
        },
        .Array => |info| {
            if (info.sentinel) |sentinel_ptr| {
                const sentinel = @as(*align(1) const info.child, @ptrCast(sentinel_ptr)).*;
                return [_:sentinel]info.child{zeroes(info.child)} ** info.len;
            }
            return [_]info.child{zeroes(info.child)} ** info.len;
        },
        .Vector => |info| {
            return @splat(zeroes(info.child));
        },
        .Union => |info| {
            if (info.layout == .@"extern" or info.layout == .@"packed") {
                var item: T = undefined;
                @memset(asBytes(&item), 0);
                return item;
            }
            @compileError("Can't set a " ++ @typeName(T) ++ " to zero.");
        },
        .EnumLiteral,
        .ErrorUnion,
        .ErrorSet,
        .Fn,
        .Type,
        .NoReturn,
        .Undefined,
        .Opaque,
        .Frame,
        .AnyFrame,
        => {
            @compileError("Can't set a " ++ @typeName(T) ++ " to zero.");
        },
    }
}
