/// Aligns the given number backward.
pub fn alignBackward(comptime T: type, addr: T, alignment: T) T {
    // TODO: Panic
    // assert(isValidAlignGeneric(T, alignment));

    // 000010000 // example alignment
    // 000001111 // subtract 1
    // 111110000 // binary not
    return addr & ~(alignment - 1);
}

/// Aligns the given number forward.
pub fn alignForward(comptime T: type, addr: T, alignment: T) T {
    // TODO: Panic
    // assert(isValidAlignGeneric(T, alignment));

    return alignBackward(T, addr + (alignment - 1), alignment);
}

/// Checks if the given number is a power of two.
pub fn isPowerOfTwo(num: anytype) bool {
    return (num != 0) and ((num & (num - 1)) == 0);
}

/// Rounds to the next power of two.
pub fn ceilPowerOfTwo(num: anytype) @TypeOf(num) {
    var n = num;
    n -= 1;

    comptime var masks: [7]?usize = .{ null, null, null, null, null, null, null };
    comptime {
        masks[0] = 1;
        masks[1] = 2;
        masks[2] = 4;
        switch (@TypeOf(num)) {
            u8, i8 => {},
            u16, i16 => {
                masks[3] = 8;
            },
            u32, i32 => {
                masks[3] = 8;
                masks[4] = 16;
            },
            u64, i64, usize => {
                masks[3] = 8;
                masks[4] = 16;
                masks[5] = 32;
            },
            u128, i128 => {
                masks[3] = 8;
                masks[4] = 16;
                masks[5] = 32;
                masks[6] = 64;
            },
            else => @compileError("Invalid numeric value!"),
        }
    }
    inline for (masks) |null_mask| {
        if (null_mask) |mask| {
            n |= n >> mask;
        }
    }

    n += 1;
    return n;
}

/// Returns the numeric value as binary.
pub fn asBinary(comptime T: type, n: T) [@bitSizeOf(T)]u8 {
    const Bits = @bitSizeOf(T);
    const ones: @Vector(Bits, u8) = @splat('1');
    const zeroes: @Vector(Bits, u8) = @splat('0');
    const bits: @Vector(Bits, bool) = @bitCast(@bitReverse(n));

    return @select(u8, bits, ones, zeroes);
}
/// Returns the numeric value as binary with no leading.
pub fn asBinaryNoLeading(comptime T: type, n: T, buf: *[@bitSizeOf(T)]u8) []const u8 {
    buf.* = asBinary(T, n);
    return buf[@clz(n)..];
}

fn nibSwap(comptime T: type, n: T) T {
    const MaskVec = @Vector(@sizeOf(T), u8);
    const high: T = @bitCast(@as(MaskVec, @splat(0xF0)));
    const low: T = @bitCast(@as(MaskVec, @splat(0x0F)));

    return @byteSwap(((n << 4) & high) | ((n >> 4) & low));
}
/// Returns the numeric value as hex.
pub fn asHex(comptime T: type, n: T) [@sizeOf(T) * 2]u8 {
    const Nibs = @sizeOf(T) * 2;
    const nibs: @Vector(Nibs, u4) = @bitCast(nibSwap(T, n));
    const mask: @Vector(Nibs, u4) = @splat(0b1010);
    const zeroes: @Vector(Nibs, u8) = @splat('0');
    const alphas: @Vector(Nibs, u8) = @splat('a');

    return @select(
        u8,
        nibs < mask,
        nibs +% zeroes,
        nibs -% mask +% alphas,
    );
}
/// Returns the numeric value as hex with no leading.
pub fn asHexNoLeading(comptime T: type, n: T, buf: *[@sizeOf(T) * 2]u8) []const u8 {
    buf.* = asHex(T, n);
    return buf[@clz(n) / 4 ..];
}
