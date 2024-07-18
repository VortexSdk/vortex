const Allocator = @import("allocator/Allocator.zig");

pub fn ArrayList(comptime T: type) type {
    return struct {
        /// An ArrayList implementation that relies on the caller for handling the lifetime.
        const ArrayListInner = @This();

        mem: []T,
        pos: usize = 0,
        allocator: *Allocator,

        /// Initializes an ArrayList.
        pub fn init(allocator: *Allocator) Allocator.AllocErr!ArrayListInner {
            return init_with_capacity(allocator, 10);
        }

        /// Initializes an ArrayList with an inital capacity.
        pub fn init_with_capacity(allocator: *Allocator, capacity: usize) Allocator.AllocErr!ArrayListInner {
            return ArrayListInner{ .allocator = allocator, .mem = try allocator.alloc(T, capacity), .pos = 0 };
        }

        /// Pushes a new item to the ArrayList.
        pub fn push(self: *ArrayListInner, entity: T) Allocator.AllocErr!void {
            if (self.pos >= self.mem.len) {
                self.mem = try self.allocator.resize_or_alloc(T, self.mem, self.mem.len * 2);
            }

            self.mem[self.pos] = entity;
            self.pos += 1;
        }

        /// Fetches the item by index from the ArrayList.
        pub fn get(self: *ArrayListInner, idx: usize) ?T {
            if (idx > self.pos) return null;

            return self.mem[idx];
        }

        /// Returns an slice that contains all the pushed elements.
        pub fn get_all(self: *ArrayListInner) []T {
            return self.mem[0..self.pos];
        }
    };
}
