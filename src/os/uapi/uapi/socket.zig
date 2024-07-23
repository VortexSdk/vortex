pub const Family = c_ushort;

pub const Sockaddr = extern struct {
    family: Family,
    data: [14]u8,
};

pub const SockaddrStorage = extern struct {
    pub const PaddingSize: comptime_int = 128 - @sizeOf(Family);

    family: Family align(@alignOf(*anyopaque)) = 0,
    padding: [PaddingSize]u8 = .{0} ** PaddingSize,
};
