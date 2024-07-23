// Manually translated uapi's io_uring header from Linux 6.9.
// Reference: https://github.com/torvalds/linux/blob/a38297e3fb012ddfa7ce0321a7e5a8daeb1872b6/include/uapi/linux/stat.h

/// Timestamp structure for the timestamps in struct statx.
///
/// tv_sec holds the number of seconds before (negative) or after (positive)
/// 00:00:00 1st January 1970 UTC.
///
/// tv_nsec holds a number of nanoseconds (0..999,999,999) after the tv_sec time.
///
/// __reserved is held in case we need a yet finer resolution.
pub const StatxTimestamp = extern struct {
    tv_sec: i64 = 0,
    tv_nsec: u32 = 0,
    __reserved: i32 = 0,
};

/// Flags to be stx_mask
///
/// Query request/result mask for statx() and struct statx::stx_mask.
///
/// These bits should be set in the mask argument of statx() to request
/// particular items when calling statx().
pub const StatxMask = packed struct {
    /// Want/got stx_mode & S_IFMT
    type: bool = false,
    /// Want/got stx_mode & ~S_IFMT
    mode: bool = false,
    /// Want/got stx_nlink
    nlink: bool = false,
    /// Want/got stx_uid
    uid: bool = false,
    /// Want/got stx_gid
    gid: bool = false,
    /// Want/got stx_atime
    atime: bool = false,
    /// Want/got stx_mtime
    mtime: bool = false,
    /// Want/got stx_ctime
    ctime: bool = false,
    /// Want/got stx_ino
    ino: bool = false,
    /// Want/got stx_size
    size: bool = false,
    /// Want/got stx_blocks
    blocks: bool = false,

    // STATX_BASIC_STATS = 0x000007ffU // The stuff in the normal stat struct

    /// Want/got stx_btime
    btime: bool = false,
    /// Got stx_mnt_id
    mnt_id: bool = false,
    /// Want/got direct I/O alignment info
    dioalign: bool = false,
    /// Want/got extended stx_mount_id
    mnt_id_unique: bool = false,
    _: u17 = 0,
};

/// Attributes to be found in stx_attributes and masked in stx_attributes_mask.
///
/// These give information about the features or the state of a file that might
/// be of use to ordinary userspace programs such as GUIs or ls rather than
/// specialised tools.
///
/// Note that the flags marked [I] correspond to the FS_IOC_SETFLAGS flags
/// semantically.  Where possible, the numerical value is picked to correspond
/// also.  Note that the DAX attribute indicates that the file is in the CPU
/// direct access state.  It does not correspond to the per-inode flag that
/// some filesystems support.
pub const StatxAttributes = packed struct {
    _: u2 = 0,
    /// [I] File is compressed by the fs
    compressed: bool = false,
    __: u1 = 0,
    /// [I] File is marked immutable
    immutable: bool = false,
    /// [I] File is append-only
    append: bool = false,
    /// [I] File is not to be dumped
    nodump: bool = false,
    ___: u4 = 0,
    /// [I] File requires key to decrypt in fs
    encrypted: bool = false,
    /// Dir: Automount trigger
    automount: bool = false,
    /// Root of a mount
    mount_root: bool = false,
    ____: u6 = 0,
    /// [I] Verity protected file
    verity: bool = false,
    /// File is currently in DAX state
    dax: bool = false,
    _____: u42 = 0,
};

/// Structures for the extended file attribute retrieval system call
/// (statx()).
///
/// The caller passes a mask of what they're specifically interested in as a
/// parameter to statx().  What statx() actually got will be indicated in
/// st_mask upon return.
///
/// For each bit in the mask argument:
///
/// - if the datum is not supported:
///
///   - the bit will be cleared, and
///
///   - the datum will be set to an appropriate fabricated value if one is
///     available (eg. CIFS can take a default uid and gid), otherwise
///
///   - the field will be cleared;
///
/// - otherwise, if explicitly requested:
///
///   - the datum will be synchronised to the server if AT_STATX_FORCE_SYNC is
///     set or if the datum is considered out of date, and
///
///   - the field will be filled in and the bit will be set;
///
/// - otherwise, if not requested, but available in approximate form without any
///   effort, it will be filled in anyway, and the bit will be set upon return
///   (it might not be up to date, however, and no attempt will be made to
///   synchronise the internal state first);
///
/// - otherwise the field and the bit will be cleared before returning.
///
/// Items in STATX_BASIC_STATS may be marked unavailable on return, but they
/// will have values installed for compatibility purposes so that stat() and
/// co. can be emulated in userspace.
pub const Statx = extern struct {
    // 0x00
    /// What results were written [uncond]
    mask: StatxMask = .{},
    /// Preferred general I/O size [uncond]
    blksize: u32 = 0,
    /// Flags conveying information about the file [uncond]
    attributes: StatxAttributes = .{},

    // 0x10
    /// Number of hard links
    nlink: u32 = 0,
    /// User ID of owner
    uid: u32 = 0,
    /// Group ID of owner
    gid: u32 = 0,
    /// File mode
    mode: u16 = 0,
    __spare0: [1]u16 = .{0},

    // 0x20
    /// Inode number
    ino: u64 = 0,
    /// File size
    size: u64 = 0,
    /// Number of 512-byte blocks allocated
    blocks: u64 = 0,
    /// Mask to show what's supported in stx_attributes
    attributes_mask: StatxAttributes = .{},

    // 0x40
    /// Last access time
    atime: StatxTimestamp = .{},
    /// File creation time
    btime: StatxTimestamp = .{},
    /// Last attribute change time
    ctime: StatxTimestamp = .{},
    /// Last data modification time
    mtime: StatxTimestamp = .{},

    // 0x80
    /// Device ID of special file [if bdev/cdev]
    rdev_major: u32 = 0,
    rdev_minor: u32 = 0,
    /// ID of device containing file [uncond]
    dev_major: u32 = 0,
    dev_minor: u32 = 0,

    // 0x90
    mnt_id: u64 = 0,
    /// Memory buffer alignment for direct I/O
    dio_mem_align: u32 = 0,
    /// File offset alignment for direct I/O
    dio_offset_align: u32 = 0,

    // 0xa0
    /// Spare space for future expansion
    __spare3: [12]u64 = .{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    // 0x100
};
