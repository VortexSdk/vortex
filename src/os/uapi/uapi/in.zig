const Socket = @import("socket.zig");

pub const SockaddrIn= extern struct  {
    family: Socket.Family,
    __be16 sin_port,
    addr: in_addr,

    
    __pad: [sizeof(Socket.Storage) - sizeof(c_short) - sizeof(c_ushort) -
                         sizeof(struct in_addr)] i8
};