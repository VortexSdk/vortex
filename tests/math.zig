const std = @import("std");
const vortex = @import("vortex");

const math = vortex.math;
const testing = std.testing;

test "PowerOfTwo" {
    try testing.expect(math.ceilPowerOfTwo(@as(u8, 6)) == 8);
    try testing.expect(math.ceilPowerOfTwo(@as(i8, 8)) == 8);
    try testing.expect(math.ceilPowerOfTwo(@as(u16, 13)) == 16);
    try testing.expect(math.ceilPowerOfTwo(@as(i16, 256)) == 256);
    try testing.expect(math.ceilPowerOfTwo(@as(u32, 24)) == 32);
    try testing.expect(math.ceilPowerOfTwo(@as(i32, 23)) == 32);
    try testing.expect(math.ceilPowerOfTwo(@as(u64, 111)) == 128);
    try testing.expect(math.ceilPowerOfTwo(@as(i64, 127)) == 128);
    try testing.expect(math.ceilPowerOfTwo(@as(u128, 512)) == 512);
    try testing.expect(math.ceilPowerOfTwo(@as(i128, 1023)) == 1024);

    try testing.expect(!math.isPowerOfTwo(0));
    try testing.expect(math.isPowerOfTwo(2));
    try testing.expect(!math.isPowerOfTwo(5));
    try testing.expect(math.isPowerOfTwo(16));
    try testing.expect(!math.isPowerOfTwo(17));
    try testing.expect(math.isPowerOfTwo(128));
    try testing.expect(!math.isPowerOfTwo(129));
}

test "AlignForwardAndBackward" {
    try testing.expect(math.alignBackward(usize, 29, 8) == 24);
    try testing.expect(math.alignBackward(usize, 32, 8) == 32);
    try testing.expect(math.alignBackward(usize, 65, 2) == 64);

    try testing.expect(math.alignForward(usize, 31, 8) == 32);
    try testing.expect(math.alignForward(usize, 21, 8) == 24);
    try testing.expect(math.alignForward(usize, 63, 2) == 64);
}
