const os = @import("os.zig");

pub fn BufPrinter(comptime size: comptime_int) type {
    return struct {
        buf: [size]u8 = undefined,
        pos: u16 = 0,

        const BufPrinterInner = @This();

        /// Initializes a BufPrinter interface.
        pub fn init() BufPrinterInner {
            return BufPrinterInner{
                .buf = undefined,
                .pos = 0,
            };
        }

        /// Pushes to the buffer and/or flushes if the maximum reaches.
        pub fn print(self: *BufPrinterInner, msg: []const u8) void {
            @setRuntimeSafety(false);

            const new_pos = self.pos + msg.len;
            if (new_pos > self.buf.len) {
                self.flush();
                return self.print(msg);
            }

            @memcpy(self.buf[self.pos..new_pos], msg);
            self.pos += @intCast(msg.len);
        }

        /// Pushes multiple slices to the buffer and/or flushes if the maximum reaches.
        pub fn print_many(self: *BufPrinterInner, comptime n: comptime_int, msgs: [n][]const u8) void {
            @setRuntimeSafety(false);

            inline for (msgs) |msg| self.print(msg);
        }

        /// Pushes to the buffer with a newline character and/or flushes if the maximum reaches.
        pub fn println(self: *BufPrinterInner, msg: []const u8) void {
            @setRuntimeSafety(false);

            self.print_many(2, .{ msg, "\n" });
        }

        /// Pushes multiple slices to the buffer with a newline character and/or flushes if the
        /// maximum reaches.
        pub fn println_many(self: *BufPrinterInner, comptime n: comptime_int, msgs: [n][]const u8) void {
            @setRuntimeSafety(false);

            self.print_many(n + 1, msgs ++ .{"\n"});
        }

        /// Flushes the buffer.
        pub fn flush(self: *BufPrinterInner) void {
            @setRuntimeSafety(false);

            os.print(self.buf[0..self.pos]);
            self.pos = 0;
        }
    };
}
