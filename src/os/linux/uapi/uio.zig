pub const Iovec = extern struct {
    base: [*]u8,
    len: usize,
};

pub const IovecConst = extern struct {
    base: [*]const u8,
    len: usize,
};
