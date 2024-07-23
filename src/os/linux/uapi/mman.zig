pub const Mmap = packed struct(usize) {
    visibility: enum(u2) {
        shared = 1,
        private = 2,
        shared_validate = 3,
    } = .private,
    _: u2 = 0,
    fixed: bool = false,
    anonymous: bool = false,
    __: u2 = 0,
    grows_down: bool = false,
    ___: u2 = 0,
    deny_write: bool = false,
    executable: bool = false,
    locked: bool = false,
    no_reserve: bool = false,
    populate: bool = false,
    non_block: bool = false,
    stack: bool = false,
    hugetlb: bool = false,
    sync: bool = false,
    fixed_noreplace: bool = false,
    ____: u43 = 0,
};

pub const Mprotect = packed struct(usize) {
    read: bool = false,
    write: bool = false,
    exec: bool = false,
    _: u21 = 0,
    grows_down: bool = false,
    grows_up: bool = false,
    __: u38 = 0,
};
