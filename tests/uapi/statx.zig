const vortex = @import("vortex");
const assert = @import("common.zig").assert;

const statx = vortex.os.linux.Statx;

test "StatxMask" {
    try assert(statx.StatxMask, .{
        .type = 0x00000001,
        .mode = 0x00000002,
        .nlink = 0x00000004,
        .uid = 0x00000008,
        .gid = 0x00000010,
        .atime = 0x00000020,
        .mtime = 0x00000040,
        .ctime = 0x00000080,
        .ino = 0x00000100,
        .size = 0x00000200,
        .blocks = 0x00000400,
        .btime = 0x00000800,
        .mnt_id = 0x00001000,
        .dioalign = 0x00002000,
        .mnt_id_unique = 0x00004000,
    });
}

test "StatxAttributes" {
    try assert(statx.StatxAttributes, .{
        .compressed = 0x00000004,
        .immutable = 0x00000010,
        .append = 0x00000020,
        .nodump = 0x00000040,
        .encrypted = 0x00000800,
        .automount = 0x00001000,
        .mount_root = 0x00002000,
        .verity = 0x00100000,
        .dax = 0x00200000,
    });
}
