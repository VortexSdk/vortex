const Socket = @import("socket.zig");

pub const UNIX_PATH_MAX = 108;

pub const SockaddrUn = extern struct {
    family: Socket.Family,
    path: [UNIX_PATH_MAX]u8,
};
