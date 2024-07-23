// Automatically translated uapi's io_uring header from Linux 6.9 by Zig's translate-c feature.
// Reference: https://github.com/torvalds/linux/blob/a38297e3fb012ddfa7ce0321a7e5a8daeb1872b6/include/uapi/linux/io_uring.h

pub const __builtin_bswap16 = @import("std").zig.c_builtins.__builtin_bswap16;
pub const __builtin_bswap32 = @import("std").zig.c_builtins.__builtin_bswap32;
pub const __builtin_bswap64 = @import("std").zig.c_builtins.__builtin_bswap64;
pub const __builtin_signbit = @import("std").zig.c_builtins.__builtin_signbit;
pub const __builtin_signbitf = @import("std").zig.c_builtins.__builtin_signbitf;
pub const __builtin_popcount = @import("std").zig.c_builtins.__builtin_popcount;
pub const __builtin_ctz = @import("std").zig.c_builtins.__builtin_ctz;
pub const __builtin_clz = @import("std").zig.c_builtins.__builtin_clz;
pub const __builtin_sqrt = @import("std").zig.c_builtins.__builtin_sqrt;
pub const __builtin_sqrtf = @import("std").zig.c_builtins.__builtin_sqrtf;
pub const __builtin_sin = @import("std").zig.c_builtins.__builtin_sin;
pub const __builtin_sinf = @import("std").zig.c_builtins.__builtin_sinf;
pub const __builtin_cos = @import("std").zig.c_builtins.__builtin_cos;
pub const __builtin_cosf = @import("std").zig.c_builtins.__builtin_cosf;
pub const __builtin_exp = @import("std").zig.c_builtins.__builtin_exp;
pub const __builtin_expf = @import("std").zig.c_builtins.__builtin_expf;
pub const __builtin_exp2 = @import("std").zig.c_builtins.__builtin_exp2;
pub const __builtin_exp2f = @import("std").zig.c_builtins.__builtin_exp2f;
pub const __builtin_log = @import("std").zig.c_builtins.__builtin_log;
pub const __builtin_logf = @import("std").zig.c_builtins.__builtin_logf;
pub const __builtin_log2 = @import("std").zig.c_builtins.__builtin_log2;
pub const __builtin_log2f = @import("std").zig.c_builtins.__builtin_log2f;
pub const __builtin_log10 = @import("std").zig.c_builtins.__builtin_log10;
pub const __builtin_log10f = @import("std").zig.c_builtins.__builtin_log10f;
pub const __builtin_abs = @import("std").zig.c_builtins.__builtin_abs;
pub const __builtin_labs = @import("std").zig.c_builtins.__builtin_labs;
pub const __builtin_llabs = @import("std").zig.c_builtins.__builtin_llabs;
pub const __builtin_fabs = @import("std").zig.c_builtins.__builtin_fabs;
pub const __builtin_fabsf = @import("std").zig.c_builtins.__builtin_fabsf;
pub const __builtin_floor = @import("std").zig.c_builtins.__builtin_floor;
pub const __builtin_floorf = @import("std").zig.c_builtins.__builtin_floorf;
pub const __builtin_ceil = @import("std").zig.c_builtins.__builtin_ceil;
pub const __builtin_ceilf = @import("std").zig.c_builtins.__builtin_ceilf;
pub const __builtin_trunc = @import("std").zig.c_builtins.__builtin_trunc;
pub const __builtin_truncf = @import("std").zig.c_builtins.__builtin_truncf;
pub const __builtin_round = @import("std").zig.c_builtins.__builtin_round;
pub const __builtin_roundf = @import("std").zig.c_builtins.__builtin_roundf;
pub const __builtin_strlen = @import("std").zig.c_builtins.__builtin_strlen;
pub const __builtin_strcmp = @import("std").zig.c_builtins.__builtin_strcmp;
pub const __builtin_object_size = @import("std").zig.c_builtins.__builtin_object_size;
pub const __builtin___memset_chk = @import("std").zig.c_builtins.__builtin___memset_chk;
pub const __builtin_memset = @import("std").zig.c_builtins.__builtin_memset;
pub const __builtin___memcpy_chk = @import("std").zig.c_builtins.__builtin___memcpy_chk;
pub const __builtin_memcpy = @import("std").zig.c_builtins.__builtin_memcpy;
pub const __builtin_expect = @import("std").zig.c_builtins.__builtin_expect;
pub const __builtin_nanf = @import("std").zig.c_builtins.__builtin_nanf;
pub const __builtin_huge_valf = @import("std").zig.c_builtins.__builtin_huge_valf;
pub const __builtin_inff = @import("std").zig.c_builtins.__builtin_inff;
pub const __builtin_isnan = @import("std").zig.c_builtins.__builtin_isnan;
pub const __builtin_isinf = @import("std").zig.c_builtins.__builtin_isinf;
pub const __builtin_isinf_sign = @import("std").zig.c_builtins.__builtin_isinf_sign;
pub const __has_builtin = @import("std").zig.c_builtins.__has_builtin;
pub const __builtin_assume = @import("std").zig.c_builtins.__builtin_assume;
pub const __builtin_unreachable = @import("std").zig.c_builtins.__builtin_unreachable;
pub const __builtin_constant_p = @import("std").zig.c_builtins.__builtin_constant_p;
pub const __builtin_mul_overflow = @import("std").zig.c_builtins.__builtin_mul_overflow;
pub const __s8 = i8;
pub const __u8 = u8;
pub const __s16 = c_short;
pub const __u16 = c_ushort;
pub const __s32 = c_int;
pub const __u32 = c_uint;
pub const __s64 = c_longlong;
pub const __u64 = c_ulonglong;
pub const __kernel_fd_set = extern struct {
    fds_bits: [16]c_ulong = @import("std").mem.zeroes([16]c_ulong),
};
pub const __kernel_sighandler_t = ?*const fn (c_int) callconv(.C) void;
pub const __kernel_key_t = c_int;
pub const __kernel_mqd_t = c_int;
pub const __kernel_old_uid_t = c_ushort;
pub const __kernel_old_gid_t = c_ushort;
pub const __kernel_old_dev_t = c_ulong;
pub const __kernel_long_t = c_long;
pub const __kernel_ulong_t = c_ulong;
pub const __kernel_ino_t = __kernel_ulong_t;
pub const __kernel_mode_t = c_uint;
pub const __kernel_pid_t = c_int;
pub const __kernel_ipc_pid_t = c_int;
pub const __kernel_uid_t = c_uint;
pub const __kernel_gid_t = c_uint;
pub const __kernel_suseconds_t = __kernel_long_t;
pub const __kernel_daddr_t = c_int;
pub const __kernel_uid32_t = c_uint;
pub const __kernel_gid32_t = c_uint;
pub const __kernel_size_t = __kernel_ulong_t;
pub const __kernel_ssize_t = __kernel_long_t;
pub const __kernel_ptrdiff_t = __kernel_long_t;
pub const __kernel_fsid_t = extern struct {
    val: [2]c_int = @import("std").mem.zeroes([2]c_int),
};
pub const __kernel_off_t = __kernel_long_t;
pub const __kernel_loff_t = c_longlong;
pub const __kernel_old_time_t = __kernel_long_t;
pub const __kernel_time_t = __kernel_long_t;
pub const __kernel_time64_t = c_longlong;
pub const __kernel_clock_t = __kernel_long_t;
pub const __kernel_timer_t = c_int;
pub const __kernel_clockid_t = c_int;
pub const __kernel_caddr_t = [*c]u8;
pub const __kernel_uid16_t = c_ushort;
pub const __kernel_gid16_t = c_ushort;
pub const __s128 = i128;
pub const __u128 = u128;
pub const __le16 = __u16;
pub const __be16 = __u16;
pub const __le32 = __u32;
pub const __be32 = __u32;
pub const __le64 = __u64;
pub const __be64 = __u64;
pub const __sum16 = __u16;
pub const __wsum = __u32;
pub const __poll_t = c_uint;
pub const struct_fscrypt_policy_v1 = extern struct {
    version: __u8 = @import("std").mem.zeroes(__u8),
    contents_encryption_mode: __u8 = @import("std").mem.zeroes(__u8),
    filenames_encryption_mode: __u8 = @import("std").mem.zeroes(__u8),
    flags: __u8 = @import("std").mem.zeroes(__u8),
    master_key_descriptor: [8]__u8 = @import("std").mem.zeroes([8]__u8),
};
pub const struct_fscrypt_key = extern struct {
    mode: __u32 = @import("std").mem.zeroes(__u32),
    raw: [64]__u8 = @import("std").mem.zeroes([64]__u8),
    size: __u32 = @import("std").mem.zeroes(__u32),
};
pub const struct_fscrypt_policy_v2 = extern struct {
    version: __u8 = @import("std").mem.zeroes(__u8),
    contents_encryption_mode: __u8 = @import("std").mem.zeroes(__u8),
    filenames_encryption_mode: __u8 = @import("std").mem.zeroes(__u8),
    flags: __u8 = @import("std").mem.zeroes(__u8),
    log2_data_unit_size: __u8 = @import("std").mem.zeroes(__u8),
    __reserved: [3]__u8 = @import("std").mem.zeroes([3]__u8),
    master_key_identifier: [16]__u8 = @import("std").mem.zeroes([16]__u8),
};
const union_unnamed_1 = extern union {
    version: __u8,
    v1: struct_fscrypt_policy_v1,
    v2: struct_fscrypt_policy_v2,
};
pub const struct_fscrypt_get_policy_ex_arg = extern struct {
    policy_size: __u64 = @import("std").mem.zeroes(__u64),
    policy: union_unnamed_1 = @import("std").mem.zeroes(union_unnamed_1),
};
const union_unnamed_2 = extern union {
    __reserved: [32]__u8,
    descriptor: [8]__u8,
    identifier: [16]__u8,
};
pub const struct_fscrypt_key_specifier = extern struct {
    type: __u32 = @import("std").mem.zeroes(__u32),
    __reserved: __u32 = @import("std").mem.zeroes(__u32),
    u: union_unnamed_2 = @import("std").mem.zeroes(union_unnamed_2),
};
pub const struct_fscrypt_provisioning_key_payload = extern struct {
    type: __u32 align(4) = @import("std").mem.zeroes(__u32),
    __reserved: __u32 = @import("std").mem.zeroes(__u32),
    pub fn raw(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 8)));
    }
};
pub const struct_fscrypt_add_key_arg = extern struct {
    key_spec: struct_fscrypt_key_specifier align(4) = @import("std").mem.zeroes(struct_fscrypt_key_specifier),
    raw_size: __u32 = @import("std").mem.zeroes(__u32),
    key_id: __u32 = @import("std").mem.zeroes(__u32),
    __reserved: [8]__u32 = @import("std").mem.zeroes([8]__u32),
    pub fn raw(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 80)));
    }
};
pub const struct_fscrypt_remove_key_arg = extern struct {
    key_spec: struct_fscrypt_key_specifier = @import("std").mem.zeroes(struct_fscrypt_key_specifier),
    removal_status_flags: __u32 = @import("std").mem.zeroes(__u32),
    __reserved: [5]__u32 = @import("std").mem.zeroes([5]__u32),
};
pub const struct_fscrypt_get_key_status_arg = extern struct {
    key_spec: struct_fscrypt_key_specifier = @import("std").mem.zeroes(struct_fscrypt_key_specifier),
    __reserved: [6]__u32 = @import("std").mem.zeroes([6]__u32),
    status: __u32 = @import("std").mem.zeroes(__u32),
    status_flags: __u32 = @import("std").mem.zeroes(__u32),
    user_count: __u32 = @import("std").mem.zeroes(__u32),
    __out_reserved: [13]__u32 = @import("std").mem.zeroes([13]__u32),
};
pub const FSCONFIG_SET_FLAG: c_int = 0;
pub const FSCONFIG_SET_STRING: c_int = 1;
pub const FSCONFIG_SET_BINARY: c_int = 2;
pub const FSCONFIG_SET_PATH: c_int = 3;
pub const FSCONFIG_SET_PATH_EMPTY: c_int = 4;
pub const FSCONFIG_SET_FD: c_int = 5;
pub const FSCONFIG_CMD_CREATE: c_int = 6;
pub const FSCONFIG_CMD_RECONFIGURE: c_int = 7;
pub const FSCONFIG_CMD_CREATE_EXCL: c_int = 8;
pub const enum_fsconfig_command = c_uint;
pub const struct_mount_attr = extern struct {
    attr_set: __u64 = @import("std").mem.zeroes(__u64),
    attr_clr: __u64 = @import("std").mem.zeroes(__u64),
    propagation: __u64 = @import("std").mem.zeroes(__u64),
    userns_fd: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_statmount = extern struct {
    size: __u32 align(8) = @import("std").mem.zeroes(__u32),
    __spare1: __u32 = @import("std").mem.zeroes(__u32),
    mask: __u64 = @import("std").mem.zeroes(__u64),
    sb_dev_major: __u32 = @import("std").mem.zeroes(__u32),
    sb_dev_minor: __u32 = @import("std").mem.zeroes(__u32),
    sb_magic: __u64 = @import("std").mem.zeroes(__u64),
    sb_flags: __u32 = @import("std").mem.zeroes(__u32),
    fs_type: __u32 = @import("std").mem.zeroes(__u32),
    mnt_id: __u64 = @import("std").mem.zeroes(__u64),
    mnt_parent_id: __u64 = @import("std").mem.zeroes(__u64),
    mnt_id_old: __u32 = @import("std").mem.zeroes(__u32),
    mnt_parent_id_old: __u32 = @import("std").mem.zeroes(__u32),
    mnt_attr: __u64 = @import("std").mem.zeroes(__u64),
    mnt_propagation: __u64 = @import("std").mem.zeroes(__u64),
    mnt_peer_group: __u64 = @import("std").mem.zeroes(__u64),
    mnt_master: __u64 = @import("std").mem.zeroes(__u64),
    propagate_from: __u64 = @import("std").mem.zeroes(__u64),
    mnt_root: __u32 = @import("std").mem.zeroes(__u32),
    mnt_point: __u32 = @import("std").mem.zeroes(__u32),
    __spare2: [50]__u64 = @import("std").mem.zeroes([50]__u64),
    pub fn str(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 512)));
    }
};
pub const struct_mnt_id_req = extern struct {
    size: __u32 = @import("std").mem.zeroes(__u32),
    spare: __u32 = @import("std").mem.zeroes(__u32),
    mnt_id: __u64 = @import("std").mem.zeroes(__u64),
    param: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_file_clone_range = extern struct {
    src_fd: __s64 = @import("std").mem.zeroes(__s64),
    src_offset: __u64 = @import("std").mem.zeroes(__u64),
    src_length: __u64 = @import("std").mem.zeroes(__u64),
    dest_offset: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_fstrim_range = extern struct {
    start: __u64 = @import("std").mem.zeroes(__u64),
    len: __u64 = @import("std").mem.zeroes(__u64),
    minlen: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_fsuuid2 = extern struct {
    len: __u8 = @import("std").mem.zeroes(__u8),
    uuid: [16]__u8 = @import("std").mem.zeroes([16]__u8),
};
pub const struct_fs_sysfs_path = extern struct {
    len: __u8 = @import("std").mem.zeroes(__u8),
    name: [128]__u8 = @import("std").mem.zeroes([128]__u8),
};
pub const struct_file_dedupe_range_info = extern struct {
    dest_fd: __s64 = @import("std").mem.zeroes(__s64),
    dest_offset: __u64 = @import("std").mem.zeroes(__u64),
    bytes_deduped: __u64 = @import("std").mem.zeroes(__u64),
    status: __s32 = @import("std").mem.zeroes(__s32),
    reserved: __u32 = @import("std").mem.zeroes(__u32),
};
pub const struct_file_dedupe_range = extern struct {
    src_offset: __u64 align(8) = @import("std").mem.zeroes(__u64),
    src_length: __u64 = @import("std").mem.zeroes(__u64),
    dest_count: __u16 = @import("std").mem.zeroes(__u16),
    reserved1: __u16 = @import("std").mem.zeroes(__u16),
    reserved2: __u32 = @import("std").mem.zeroes(__u32),
    pub fn info(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), struct_file_dedupe_range_info) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), struct_file_dedupe_range_info);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 24)));
    }
};
pub const struct_files_stat_struct = extern struct {
    nr_files: c_ulong = @import("std").mem.zeroes(c_ulong),
    nr_free_files: c_ulong = @import("std").mem.zeroes(c_ulong),
    max_files: c_ulong = @import("std").mem.zeroes(c_ulong),
};
pub const struct_inodes_stat_t = extern struct {
    nr_inodes: c_long = @import("std").mem.zeroes(c_long),
    nr_unused: c_long = @import("std").mem.zeroes(c_long),
    dummy: [5]c_long = @import("std").mem.zeroes([5]c_long),
};
pub const struct_fsxattr = extern struct {
    fsx_xflags: __u32 = @import("std").mem.zeroes(__u32),
    fsx_extsize: __u32 = @import("std").mem.zeroes(__u32),
    fsx_nextents: __u32 = @import("std").mem.zeroes(__u32),
    fsx_projid: __u32 = @import("std").mem.zeroes(__u32),
    fsx_cowextsize: __u32 = @import("std").mem.zeroes(__u32),
    fsx_pad: [8]u8 = @import("std").mem.zeroes([8]u8),
};
pub const __kernel_rwf_t = c_int;
pub const struct_page_region = extern struct {
    start: __u64 = @import("std").mem.zeroes(__u64),
    end: __u64 = @import("std").mem.zeroes(__u64),
    categories: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_pm_scan_arg = extern struct {
    size: __u64 = @import("std").mem.zeroes(__u64),
    flags: __u64 = @import("std").mem.zeroes(__u64),
    start: __u64 = @import("std").mem.zeroes(__u64),
    end: __u64 = @import("std").mem.zeroes(__u64),
    walk_end: __u64 = @import("std").mem.zeroes(__u64),
    vec: __u64 = @import("std").mem.zeroes(__u64),
    vec_len: __u64 = @import("std").mem.zeroes(__u64),
    max_pages: __u64 = @import("std").mem.zeroes(__u64),
    category_inverted: __u64 = @import("std").mem.zeroes(__u64),
    category_mask: __u64 = @import("std").mem.zeroes(__u64),
    category_anyof_mask: __u64 = @import("std").mem.zeroes(__u64),
    return_mask: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct___kernel_timespec = extern struct {
    tv_sec: __kernel_time64_t = @import("std").mem.zeroes(__kernel_time64_t),
    tv_nsec: c_longlong = @import("std").mem.zeroes(c_longlong),
};
pub const struct___kernel_itimerspec = extern struct {
    it_interval: struct___kernel_timespec = @import("std").mem.zeroes(struct___kernel_timespec),
    it_value: struct___kernel_timespec = @import("std").mem.zeroes(struct___kernel_timespec),
};
pub const struct___kernel_old_timeval = extern struct {
    tv_sec: __kernel_long_t = @import("std").mem.zeroes(__kernel_long_t),
    tv_usec: __kernel_long_t = @import("std").mem.zeroes(__kernel_long_t),
};
pub const struct___kernel_old_timespec = extern struct {
    tv_sec: __kernel_old_time_t = @import("std").mem.zeroes(__kernel_old_time_t),
    tv_nsec: c_long = @import("std").mem.zeroes(c_long),
};
pub const struct___kernel_old_itimerval = extern struct {
    it_interval: struct___kernel_old_timeval = @import("std").mem.zeroes(struct___kernel_old_timeval),
    it_value: struct___kernel_old_timeval = @import("std").mem.zeroes(struct___kernel_old_timeval),
};
pub const struct___kernel_sock_timeval = extern struct {
    tv_sec: __s64 = @import("std").mem.zeroes(__s64),
    tv_usec: __s64 = @import("std").mem.zeroes(__s64),
};
const struct_unnamed_4 = extern struct {
    cmd_op: __u32 = @import("std").mem.zeroes(__u32),
    __pad1: __u32 = @import("std").mem.zeroes(__u32),
};
const union_unnamed_3 = extern union {
    off: __u64,
    addr2: __u64,
    unnamed_0: struct_unnamed_4,
};
const struct_unnamed_6 = extern struct {
    level: __u32 = @import("std").mem.zeroes(__u32),
    optname: __u32 = @import("std").mem.zeroes(__u32),
};
const union_unnamed_5 = extern union {
    addr: __u64,
    splice_off_in: __u64,
    unnamed_0: struct_unnamed_6,
};
const union_unnamed_7 = extern union {
    rw_flags: __kernel_rwf_t,
    fsync_flags: __u32,
    poll_events: __u16,
    poll32_events: __u32,
    sync_range_flags: __u32,
    msg_flags: __u32,
    timeout_flags: __u32,
    accept_flags: __u32,
    cancel_flags: __u32,
    open_flags: __u32,
    statx_flags: __u32,
    fadvise_advice: __u32,
    splice_flags: __u32,
    rename_flags: __u32,
    unlink_flags: __u32,
    hardlink_flags: __u32,
    xattr_flags: __u32,
    msg_ring_flags: __u32,
    uring_cmd_flags: __u32,
    waitid_flags: __u32,
    futex_flags: __u32,
    install_fd_flags: __u32,
};
const union_unnamed_8 = extern union {
    buf_index: __u16 align(1),
    buf_group: __u16 align(1),
};
const struct_unnamed_10 = extern struct {
    addr_len: __u16 = @import("std").mem.zeroes(__u16),
    __pad3: [1]__u16 = @import("std").mem.zeroes([1]__u16),
};
const union_unnamed_9 = extern union {
    splice_fd_in: __s32,
    file_index: __u32,
    optlen: __u32,
    unnamed_0: struct_unnamed_10,
};
const struct_unnamed_12 = extern struct {
    addr3: __u64 = @import("std").mem.zeroes(__u64),
    __pad2: [1]__u64 = @import("std").mem.zeroes([1]__u64),
};
const union_unnamed_11 = extern union {
    unnamed_0: struct_unnamed_12 align(8),
    optval: __u64,
    pub fn cmd(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 0)));
    }
};
pub const struct_io_uring_sqe = extern struct {
    opcode: __u8 = @import("std").mem.zeroes(__u8),
    flags: __u8 = @import("std").mem.zeroes(__u8),
    ioprio: __u16 = @import("std").mem.zeroes(__u16),
    fd: __s32 = @import("std").mem.zeroes(__s32),
    unnamed_0: union_unnamed_3 = @import("std").mem.zeroes(union_unnamed_3),
    unnamed_1: union_unnamed_5 = @import("std").mem.zeroes(union_unnamed_5),
    len: __u32 = @import("std").mem.zeroes(__u32),
    unnamed_2: union_unnamed_7 = @import("std").mem.zeroes(union_unnamed_7),
    user_data: __u64 = @import("std").mem.zeroes(__u64),
    unnamed_3: union_unnamed_8 = @import("std").mem.zeroes(union_unnamed_8),
    personality: __u16 = @import("std").mem.zeroes(__u16),
    unnamed_4: union_unnamed_9 = @import("std").mem.zeroes(union_unnamed_9),
    unnamed_5: union_unnamed_11 = @import("std").mem.zeroes(union_unnamed_11),
};
pub const IOSQE_FIXED_FILE_BIT: c_int = 0;
pub const IOSQE_IO_DRAIN_BIT: c_int = 1;
pub const IOSQE_IO_LINK_BIT: c_int = 2;
pub const IOSQE_IO_HARDLINK_BIT: c_int = 3;
pub const IOSQE_ASYNC_BIT: c_int = 4;
pub const IOSQE_BUFFER_SELECT_BIT: c_int = 5;
pub const IOSQE_CQE_SKIP_SUCCESS_BIT: c_int = 6;
const enum_unnamed_13 = c_uint;
pub const IORING_OP_NOP: c_int = 0;
pub const IORING_OP_READV: c_int = 1;
pub const IORING_OP_WRITEV: c_int = 2;
pub const IORING_OP_FSYNC: c_int = 3;
pub const IORING_OP_READ_FIXED: c_int = 4;
pub const IORING_OP_WRITE_FIXED: c_int = 5;
pub const IORING_OP_POLL_ADD: c_int = 6;
pub const IORING_OP_POLL_REMOVE: c_int = 7;
pub const IORING_OP_SYNC_FILE_RANGE: c_int = 8;
pub const IORING_OP_SENDMSG: c_int = 9;
pub const IORING_OP_RECVMSG: c_int = 10;
pub const IORING_OP_TIMEOUT: c_int = 11;
pub const IORING_OP_TIMEOUT_REMOVE: c_int = 12;
pub const IORING_OP_ACCEPT: c_int = 13;
pub const IORING_OP_ASYNC_CANCEL: c_int = 14;
pub const IORING_OP_LINK_TIMEOUT: c_int = 15;
pub const IORING_OP_CONNECT: c_int = 16;
pub const IORING_OP_FALLOCATE: c_int = 17;
pub const IORING_OP_OPENAT: c_int = 18;
pub const IORING_OP_CLOSE: c_int = 19;
pub const IORING_OP_FILES_UPDATE: c_int = 20;
pub const IORING_OP_STATX: c_int = 21;
pub const IORING_OP_READ: c_int = 22;
pub const IORING_OP_WRITE: c_int = 23;
pub const IORING_OP_FADVISE: c_int = 24;
pub const IORING_OP_MADVISE: c_int = 25;
pub const IORING_OP_SEND: c_int = 26;
pub const IORING_OP_RECV: c_int = 27;
pub const IORING_OP_OPENAT2: c_int = 28;
pub const IORING_OP_EPOLL_CTL: c_int = 29;
pub const IORING_OP_SPLICE: c_int = 30;
pub const IORING_OP_PROVIDE_BUFFERS: c_int = 31;
pub const IORING_OP_REMOVE_BUFFERS: c_int = 32;
pub const IORING_OP_TEE: c_int = 33;
pub const IORING_OP_SHUTDOWN: c_int = 34;
pub const IORING_OP_RENAMEAT: c_int = 35;
pub const IORING_OP_UNLINKAT: c_int = 36;
pub const IORING_OP_MKDIRAT: c_int = 37;
pub const IORING_OP_SYMLINKAT: c_int = 38;
pub const IORING_OP_LINKAT: c_int = 39;
pub const IORING_OP_MSG_RING: c_int = 40;
pub const IORING_OP_FSETXATTR: c_int = 41;
pub const IORING_OP_SETXATTR: c_int = 42;
pub const IORING_OP_FGETXATTR: c_int = 43;
pub const IORING_OP_GETXATTR: c_int = 44;
pub const IORING_OP_SOCKET: c_int = 45;
pub const IORING_OP_URING_CMD: c_int = 46;
pub const IORING_OP_SEND_ZC: c_int = 47;
pub const IORING_OP_SENDMSG_ZC: c_int = 48;
pub const IORING_OP_READ_MULTISHOT: c_int = 49;
pub const IORING_OP_WAITID: c_int = 50;
pub const IORING_OP_FUTEX_WAIT: c_int = 51;
pub const IORING_OP_FUTEX_WAKE: c_int = 52;
pub const IORING_OP_FUTEX_WAITV: c_int = 53;
pub const IORING_OP_FIXED_FD_INSTALL: c_int = 54;
pub const IORING_OP_FTRUNCATE: c_int = 55;
pub const IORING_OP_LAST: c_int = 56;
pub const enum_io_uring_op = c_uint;
pub const IORING_MSG_DATA: c_int = 0;
pub const IORING_MSG_SEND_FD: c_int = 1;
const enum_unnamed_14 = c_uint;
pub const struct_io_uring_cqe = extern struct {
    user_data: __u64 align(8) = @import("std").mem.zeroes(__u64),
    res: __s32 = @import("std").mem.zeroes(__s32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
    pub fn big_cqe(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), c_ulonglong) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), c_ulonglong);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 16)));
    }
};
pub const IORING_CQE_BUFFER_SHIFT: c_int = 16;
const enum_unnamed_15 = c_uint;
pub const struct_io_sqring_offsets = extern struct {
    head: __u32 = @import("std").mem.zeroes(__u32),
    tail: __u32 = @import("std").mem.zeroes(__u32),
    ring_mask: __u32 = @import("std").mem.zeroes(__u32),
    ring_entries: __u32 = @import("std").mem.zeroes(__u32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
    dropped: __u32 = @import("std").mem.zeroes(__u32),
    array: __u32 = @import("std").mem.zeroes(__u32),
    resv1: __u32 = @import("std").mem.zeroes(__u32),
    user_addr: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_io_cqring_offsets = extern struct {
    head: __u32 = @import("std").mem.zeroes(__u32),
    tail: __u32 = @import("std").mem.zeroes(__u32),
    ring_mask: __u32 = @import("std").mem.zeroes(__u32),
    ring_entries: __u32 = @import("std").mem.zeroes(__u32),
    overflow: __u32 = @import("std").mem.zeroes(__u32),
    cqes: __u32 = @import("std").mem.zeroes(__u32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
    resv1: __u32 = @import("std").mem.zeroes(__u32),
    user_addr: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_io_uring_params = extern struct {
    sq_entries: __u32 = @import("std").mem.zeroes(__u32),
    cq_entries: __u32 = @import("std").mem.zeroes(__u32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
    sq_thread_cpu: __u32 = @import("std").mem.zeroes(__u32),
    sq_thread_idle: __u32 = @import("std").mem.zeroes(__u32),
    features: __u32 = @import("std").mem.zeroes(__u32),
    wq_fd: __u32 = @import("std").mem.zeroes(__u32),
    resv: [3]__u32 = @import("std").mem.zeroes([3]__u32),
    sq_off: struct_io_sqring_offsets = @import("std").mem.zeroes(struct_io_sqring_offsets),
    cq_off: struct_io_cqring_offsets = @import("std").mem.zeroes(struct_io_cqring_offsets),
};
pub const IORING_REGISTER_BUFFERS: c_int = 0;
pub const IORING_UNREGISTER_BUFFERS: c_int = 1;
pub const IORING_REGISTER_FILES: c_int = 2;
pub const IORING_UNREGISTER_FILES: c_int = 3;
pub const IORING_REGISTER_EVENTFD: c_int = 4;
pub const IORING_UNREGISTER_EVENTFD: c_int = 5;
pub const IORING_REGISTER_FILES_UPDATE: c_int = 6;
pub const IORING_REGISTER_EVENTFD_ASYNC: c_int = 7;
pub const IORING_REGISTER_PROBE: c_int = 8;
pub const IORING_REGISTER_PERSONALITY: c_int = 9;
pub const IORING_UNREGISTER_PERSONALITY: c_int = 10;
pub const IORING_REGISTER_RESTRICTIONS: c_int = 11;
pub const IORING_REGISTER_ENABLE_RINGS: c_int = 12;
pub const IORING_REGISTER_FILES2: c_int = 13;
pub const IORING_REGISTER_FILES_UPDATE2: c_int = 14;
pub const IORING_REGISTER_BUFFERS2: c_int = 15;
pub const IORING_REGISTER_BUFFERS_UPDATE: c_int = 16;
pub const IORING_REGISTER_IOWQ_AFF: c_int = 17;
pub const IORING_UNREGISTER_IOWQ_AFF: c_int = 18;
pub const IORING_REGISTER_IOWQ_MAX_WORKERS: c_int = 19;
pub const IORING_REGISTER_RING_FDS: c_int = 20;
pub const IORING_UNREGISTER_RING_FDS: c_int = 21;
pub const IORING_REGISTER_PBUF_RING: c_int = 22;
pub const IORING_UNREGISTER_PBUF_RING: c_int = 23;
pub const IORING_REGISTER_SYNC_CANCEL: c_int = 24;
pub const IORING_REGISTER_FILE_ALLOC_RANGE: c_int = 25;
pub const IORING_REGISTER_PBUF_STATUS: c_int = 26;
pub const IORING_REGISTER_NAPI: c_int = 27;
pub const IORING_UNREGISTER_NAPI: c_int = 28;
pub const IORING_REGISTER_LAST: c_int = 29;
pub const IORING_REGISTER_USE_REGISTERED_RING: c_uint = 2147483648;
const enum_unnamed_16 = c_uint;
pub const IO_WQ_BOUND: c_int = 0;
pub const IO_WQ_UNBOUND: c_int = 1;
const enum_unnamed_17 = c_uint;
pub const struct_io_uring_files_update = extern struct {
    offset: __u32 = @import("std").mem.zeroes(__u32),
    resv: __u32 = @import("std").mem.zeroes(__u32),
    fds: __u64 align(8) = @import("std").mem.zeroes(__u64),
};
pub const struct_io_uring_rsrc_register = extern struct {
    nr: __u32 = @import("std").mem.zeroes(__u32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
    resv2: __u64 = @import("std").mem.zeroes(__u64),
    data: __u64 align(8) = @import("std").mem.zeroes(__u64),
    tags: __u64 align(8) = @import("std").mem.zeroes(__u64),
};
pub const struct_io_uring_rsrc_update = extern struct {
    offset: __u32 = @import("std").mem.zeroes(__u32),
    resv: __u32 = @import("std").mem.zeroes(__u32),
    data: __u64 align(8) = @import("std").mem.zeroes(__u64),
};
pub const struct_io_uring_rsrc_update2 = extern struct {
    offset: __u32 = @import("std").mem.zeroes(__u32),
    resv: __u32 = @import("std").mem.zeroes(__u32),
    data: __u64 align(8) = @import("std").mem.zeroes(__u64),
    tags: __u64 align(8) = @import("std").mem.zeroes(__u64),
    nr: __u32 = @import("std").mem.zeroes(__u32),
    resv2: __u32 = @import("std").mem.zeroes(__u32),
};
pub const struct_io_uring_probe_op = extern struct {
    op: __u8 = @import("std").mem.zeroes(__u8),
    resv: __u8 = @import("std").mem.zeroes(__u8),
    flags: __u16 = @import("std").mem.zeroes(__u16),
    resv2: __u32 = @import("std").mem.zeroes(__u32),
};
pub const struct_io_uring_probe = extern struct {
    last_op: __u8 align(4) = @import("std").mem.zeroes(__u8),
    ops_len: __u8 = @import("std").mem.zeroes(__u8),
    resv: __u16 = @import("std").mem.zeroes(__u16),
    resv2: [3]__u32 = @import("std").mem.zeroes([3]__u32),
    pub fn ops(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), struct_io_uring_probe_op) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), struct_io_uring_probe_op);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 16)));
    }
};
const union_unnamed_18 = extern union {
    register_op: __u8,
    sqe_op: __u8,
    sqe_flags: __u8,
};
pub const struct_io_uring_restriction = extern struct {
    opcode: __u16 = @import("std").mem.zeroes(__u16),
    unnamed_0: union_unnamed_18 = @import("std").mem.zeroes(union_unnamed_18),
    resv: __u8 = @import("std").mem.zeroes(__u8),
    resv2: [3]__u32 = @import("std").mem.zeroes([3]__u32),
};
pub const struct_io_uring_buf = extern struct {
    addr: __u64 = @import("std").mem.zeroes(__u64),
    len: __u32 = @import("std").mem.zeroes(__u32),
    bid: __u16 = @import("std").mem.zeroes(__u16),
    resv: __u16 = @import("std").mem.zeroes(__u16),
};
const struct_unnamed_20 = extern struct {
    resv1: __u64 = @import("std").mem.zeroes(__u64),
    resv2: __u32 = @import("std").mem.zeroes(__u32),
    resv3: __u16 = @import("std").mem.zeroes(__u16),
    tail: __u16 = @import("std").mem.zeroes(__u16),
};
const struct_unnamed_22 = extern struct {};
const struct_unnamed_21 = extern struct {
    __empty_bufs: struct_unnamed_22 align(8) = @import("std").mem.zeroes(struct_unnamed_22),
    pub fn bufs(self: anytype) @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), struct_io_uring_buf) {
        const Intermediate = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), u8);
        const ReturnType = @import("std").zig.c_translation.FlexibleArrayType(@TypeOf(self), struct_io_uring_buf);
        return @as(ReturnType, @ptrCast(@alignCast(@as(Intermediate, @ptrCast(self)) + 0)));
    }
};
const union_unnamed_19 = extern union {
    unnamed_0: struct_unnamed_20,
    unnamed_1: struct_unnamed_21,
};
pub const struct_io_uring_buf_ring = extern struct {
    unnamed_0: union_unnamed_19 = @import("std").mem.zeroes(union_unnamed_19),
};
pub const IOU_PBUF_RING_MMAP: c_int = 1;
const enum_unnamed_23 = c_uint;
pub const struct_io_uring_buf_reg = extern struct {
    ring_addr: __u64 = @import("std").mem.zeroes(__u64),
    ring_entries: __u32 = @import("std").mem.zeroes(__u32),
    bgid: __u16 = @import("std").mem.zeroes(__u16),
    flags: __u16 = @import("std").mem.zeroes(__u16),
    resv: [3]__u64 = @import("std").mem.zeroes([3]__u64),
};
pub const struct_io_uring_buf_status = extern struct {
    buf_group: __u32 = @import("std").mem.zeroes(__u32),
    head: __u32 = @import("std").mem.zeroes(__u32),
    resv: [8]__u32 = @import("std").mem.zeroes([8]__u32),
};
pub const struct_io_uring_napi = extern struct {
    busy_poll_to: __u32 = @import("std").mem.zeroes(__u32),
    prefer_busy_poll: __u8 = @import("std").mem.zeroes(__u8),
    pad: [3]__u8 = @import("std").mem.zeroes([3]__u8),
    resv: __u64 = @import("std").mem.zeroes(__u64),
};
pub const IORING_RESTRICTION_REGISTER_OP: c_int = 0;
pub const IORING_RESTRICTION_SQE_OP: c_int = 1;
pub const IORING_RESTRICTION_SQE_FLAGS_ALLOWED: c_int = 2;
pub const IORING_RESTRICTION_SQE_FLAGS_REQUIRED: c_int = 3;
pub const IORING_RESTRICTION_LAST: c_int = 4;
const enum_unnamed_24 = c_uint;
pub const struct_io_uring_getevents_arg = extern struct {
    sigmask: __u64 = @import("std").mem.zeroes(__u64),
    sigmask_sz: __u32 = @import("std").mem.zeroes(__u32),
    pad: __u32 = @import("std").mem.zeroes(__u32),
    ts: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_io_uring_sync_cancel_reg = extern struct {
    addr: __u64 = @import("std").mem.zeroes(__u64),
    fd: __s32 = @import("std").mem.zeroes(__s32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
    timeout: struct___kernel_timespec = @import("std").mem.zeroes(struct___kernel_timespec),
    opcode: __u8 = @import("std").mem.zeroes(__u8),
    pad: [7]__u8 = @import("std").mem.zeroes([7]__u8),
    pad2: [3]__u64 = @import("std").mem.zeroes([3]__u64),
};
pub const struct_io_uring_file_index_range = extern struct {
    off: __u32 = @import("std").mem.zeroes(__u32),
    len: __u32 = @import("std").mem.zeroes(__u32),
    resv: __u64 = @import("std").mem.zeroes(__u64),
};
pub const struct_io_uring_recvmsg_out = extern struct {
    namelen: __u32 = @import("std").mem.zeroes(__u32),
    controllen: __u32 = @import("std").mem.zeroes(__u32),
    payloadlen: __u32 = @import("std").mem.zeroes(__u32),
    flags: __u32 = @import("std").mem.zeroes(__u32),
};
pub const SOCKET_URING_OP_SIOCINQ: c_int = 0;
pub const SOCKET_URING_OP_SIOCOUTQ: c_int = 1;
pub const SOCKET_URING_OP_GETSOCKOPT: c_int = 2;
pub const SOCKET_URING_OP_SETSOCKOPT: c_int = 3;
const enum_unnamed_25 = c_uint;
pub const __llvm__ = @as(c_int, 1);
pub const __clang__ = @as(c_int, 1);
pub const __clang_major__ = @as(c_int, 18);
pub const __clang_minor__ = @as(c_int, 1);
pub const __clang_patchlevel__ = @as(c_int, 7);
pub const __clang_version__ = "18.1.7 (https://github.com/ziglang/zig-bootstrap e3fe3904af3801454499e3a54c8f8a8c444d3a12)";
pub const __GNUC__ = @as(c_int, 4);
pub const __GNUC_MINOR__ = @as(c_int, 2);
pub const __GNUC_PATCHLEVEL__ = @as(c_int, 1);
pub const __GXX_ABI_VERSION = @as(c_int, 1002);
pub const __ATOMIC_RELAXED = @as(c_int, 0);
pub const __ATOMIC_CONSUME = @as(c_int, 1);
pub const __ATOMIC_ACQUIRE = @as(c_int, 2);
pub const __ATOMIC_RELEASE = @as(c_int, 3);
pub const __ATOMIC_ACQ_REL = @as(c_int, 4);
pub const __ATOMIC_SEQ_CST = @as(c_int, 5);
pub const __MEMORY_SCOPE_SYSTEM = @as(c_int, 0);
pub const __MEMORY_SCOPE_DEVICE = @as(c_int, 1);
pub const __MEMORY_SCOPE_WRKGRP = @as(c_int, 2);
pub const __MEMORY_SCOPE_WVFRNT = @as(c_int, 3);
pub const __MEMORY_SCOPE_SINGLE = @as(c_int, 4);
pub const __OPENCL_MEMORY_SCOPE_WORK_ITEM = @as(c_int, 0);
pub const __OPENCL_MEMORY_SCOPE_WORK_GROUP = @as(c_int, 1);
pub const __OPENCL_MEMORY_SCOPE_DEVICE = @as(c_int, 2);
pub const __OPENCL_MEMORY_SCOPE_ALL_SVM_DEVICES = @as(c_int, 3);
pub const __OPENCL_MEMORY_SCOPE_SUB_GROUP = @as(c_int, 4);
pub const __FPCLASS_SNAN = @as(c_int, 0x0001);
pub const __FPCLASS_QNAN = @as(c_int, 0x0002);
pub const __FPCLASS_NEGINF = @as(c_int, 0x0004);
pub const __FPCLASS_NEGNORMAL = @as(c_int, 0x0008);
pub const __FPCLASS_NEGSUBNORMAL = @as(c_int, 0x0010);
pub const __FPCLASS_NEGZERO = @as(c_int, 0x0020);
pub const __FPCLASS_POSZERO = @as(c_int, 0x0040);
pub const __FPCLASS_POSSUBNORMAL = @as(c_int, 0x0080);
pub const __FPCLASS_POSNORMAL = @as(c_int, 0x0100);
pub const __FPCLASS_POSINF = @as(c_int, 0x0200);
pub const __PRAGMA_REDEFINE_EXTNAME = @as(c_int, 1);
pub const __VERSION__ = "Clang 18.1.7 (https://github.com/ziglang/zig-bootstrap e3fe3904af3801454499e3a54c8f8a8c444d3a12)";
pub const __OBJC_BOOL_IS_BOOL = @as(c_int, 0);
pub const __CONSTANT_CFSTRINGS__ = @as(c_int, 1);
pub const __clang_literal_encoding__ = "UTF-8";
pub const __clang_wide_literal_encoding__ = "UTF-32";
pub const __ORDER_LITTLE_ENDIAN__ = @as(c_int, 1234);
pub const __ORDER_BIG_ENDIAN__ = @as(c_int, 4321);
pub const __ORDER_PDP_ENDIAN__ = @as(c_int, 3412);
pub const __BYTE_ORDER__ = __ORDER_LITTLE_ENDIAN__;
pub const __LITTLE_ENDIAN__ = @as(c_int, 1);
pub const _LP64 = @as(c_int, 1);
pub const __LP64__ = @as(c_int, 1);
pub const __CHAR_BIT__ = @as(c_int, 8);
pub const __BOOL_WIDTH__ = @as(c_int, 8);
pub const __SHRT_WIDTH__ = @as(c_int, 16);
pub const __INT_WIDTH__ = @as(c_int, 32);
pub const __LONG_WIDTH__ = @as(c_int, 64);
pub const __LLONG_WIDTH__ = @as(c_int, 64);
pub const __BITINT_MAXWIDTH__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 8388608, .decimal);
pub const __SCHAR_MAX__ = @as(c_int, 127);
pub const __SHRT_MAX__ = @as(c_int, 32767);
pub const __INT_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __LONG_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __LONG_LONG_MAX__ = @as(c_longlong, 9223372036854775807);
pub const __WCHAR_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __WCHAR_WIDTH__ = @as(c_int, 32);
pub const __WINT_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __WINT_WIDTH__ = @as(c_int, 32);
pub const __INTMAX_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INTMAX_WIDTH__ = @as(c_int, 64);
pub const __SIZE_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __SIZE_WIDTH__ = @as(c_int, 64);
pub const __UINTMAX_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINTMAX_WIDTH__ = @as(c_int, 64);
pub const __PTRDIFF_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __PTRDIFF_WIDTH__ = @as(c_int, 64);
pub const __INTPTR_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INTPTR_WIDTH__ = @as(c_int, 64);
pub const __UINTPTR_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINTPTR_WIDTH__ = @as(c_int, 64);
pub const __SIZEOF_DOUBLE__ = @as(c_int, 8);
pub const __SIZEOF_FLOAT__ = @as(c_int, 4);
pub const __SIZEOF_INT__ = @as(c_int, 4);
pub const __SIZEOF_LONG__ = @as(c_int, 8);
pub const __SIZEOF_LONG_DOUBLE__ = @as(c_int, 16);
pub const __SIZEOF_LONG_LONG__ = @as(c_int, 8);
pub const __SIZEOF_POINTER__ = @as(c_int, 8);
pub const __SIZEOF_SHORT__ = @as(c_int, 2);
pub const __SIZEOF_PTRDIFF_T__ = @as(c_int, 8);
pub const __SIZEOF_SIZE_T__ = @as(c_int, 8);
pub const __SIZEOF_WCHAR_T__ = @as(c_int, 4);
pub const __SIZEOF_WINT_T__ = @as(c_int, 4);
pub const __SIZEOF_INT128__ = @as(c_int, 16);
pub const __INTMAX_TYPE__ = c_long;
pub const __INTMAX_FMTd__ = "ld";
pub const __INTMAX_FMTi__ = "li";
pub const __INTMAX_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `L`");
// (no file):95:9
pub const __UINTMAX_TYPE__ = c_ulong;
pub const __UINTMAX_FMTo__ = "lo";
pub const __UINTMAX_FMTu__ = "lu";
pub const __UINTMAX_FMTx__ = "lx";
pub const __UINTMAX_FMTX__ = "lX";
pub const __UINTMAX_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `UL`");
// (no file):101:9
pub const __PTRDIFF_TYPE__ = c_long;
pub const __PTRDIFF_FMTd__ = "ld";
pub const __PTRDIFF_FMTi__ = "li";
pub const __INTPTR_TYPE__ = c_long;
pub const __INTPTR_FMTd__ = "ld";
pub const __INTPTR_FMTi__ = "li";
pub const __SIZE_TYPE__ = c_ulong;
pub const __SIZE_FMTo__ = "lo";
pub const __SIZE_FMTu__ = "lu";
pub const __SIZE_FMTx__ = "lx";
pub const __SIZE_FMTX__ = "lX";
pub const __WCHAR_TYPE__ = c_int;
pub const __WINT_TYPE__ = c_uint;
pub const __SIG_ATOMIC_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __SIG_ATOMIC_WIDTH__ = @as(c_int, 32);
pub const __CHAR16_TYPE__ = c_ushort;
pub const __CHAR32_TYPE__ = c_uint;
pub const __UINTPTR_TYPE__ = c_ulong;
pub const __UINTPTR_FMTo__ = "lo";
pub const __UINTPTR_FMTu__ = "lu";
pub const __UINTPTR_FMTx__ = "lx";
pub const __UINTPTR_FMTX__ = "lX";
pub const __FLT16_DENORM_MIN__ = @as(f16, 5.9604644775390625e-8);
pub const __FLT16_HAS_DENORM__ = @as(c_int, 1);
pub const __FLT16_DIG__ = @as(c_int, 3);
pub const __FLT16_DECIMAL_DIG__ = @as(c_int, 5);
pub const __FLT16_EPSILON__ = @as(f16, 9.765625e-4);
pub const __FLT16_HAS_INFINITY__ = @as(c_int, 1);
pub const __FLT16_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __FLT16_MANT_DIG__ = @as(c_int, 11);
pub const __FLT16_MAX_10_EXP__ = @as(c_int, 4);
pub const __FLT16_MAX_EXP__ = @as(c_int, 16);
pub const __FLT16_MAX__ = @as(f16, 6.5504e+4);
pub const __FLT16_MIN_10_EXP__ = -@as(c_int, 4);
pub const __FLT16_MIN_EXP__ = -@as(c_int, 13);
pub const __FLT16_MIN__ = @as(f16, 6.103515625e-5);
pub const __FLT_DENORM_MIN__ = @as(f32, 1.40129846e-45);
pub const __FLT_HAS_DENORM__ = @as(c_int, 1);
pub const __FLT_DIG__ = @as(c_int, 6);
pub const __FLT_DECIMAL_DIG__ = @as(c_int, 9);
pub const __FLT_EPSILON__ = @as(f32, 1.19209290e-7);
pub const __FLT_HAS_INFINITY__ = @as(c_int, 1);
pub const __FLT_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __FLT_MANT_DIG__ = @as(c_int, 24);
pub const __FLT_MAX_10_EXP__ = @as(c_int, 38);
pub const __FLT_MAX_EXP__ = @as(c_int, 128);
pub const __FLT_MAX__ = @as(f32, 3.40282347e+38);
pub const __FLT_MIN_10_EXP__ = -@as(c_int, 37);
pub const __FLT_MIN_EXP__ = -@as(c_int, 125);
pub const __FLT_MIN__ = @as(f32, 1.17549435e-38);
pub const __DBL_DENORM_MIN__ = @as(f64, 4.9406564584124654e-324);
pub const __DBL_HAS_DENORM__ = @as(c_int, 1);
pub const __DBL_DIG__ = @as(c_int, 15);
pub const __DBL_DECIMAL_DIG__ = @as(c_int, 17);
pub const __DBL_EPSILON__ = @as(f64, 2.2204460492503131e-16);
pub const __DBL_HAS_INFINITY__ = @as(c_int, 1);
pub const __DBL_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __DBL_MANT_DIG__ = @as(c_int, 53);
pub const __DBL_MAX_10_EXP__ = @as(c_int, 308);
pub const __DBL_MAX_EXP__ = @as(c_int, 1024);
pub const __DBL_MAX__ = @as(f64, 1.7976931348623157e+308);
pub const __DBL_MIN_10_EXP__ = -@as(c_int, 307);
pub const __DBL_MIN_EXP__ = -@as(c_int, 1021);
pub const __DBL_MIN__ = @as(f64, 2.2250738585072014e-308);
pub const __LDBL_DENORM_MIN__ = @as(c_longdouble, 3.64519953188247460253e-4951);
pub const __LDBL_HAS_DENORM__ = @as(c_int, 1);
pub const __LDBL_DIG__ = @as(c_int, 18);
pub const __LDBL_DECIMAL_DIG__ = @as(c_int, 21);
pub const __LDBL_EPSILON__ = @as(c_longdouble, 1.08420217248550443401e-19);
pub const __LDBL_HAS_INFINITY__ = @as(c_int, 1);
pub const __LDBL_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __LDBL_MANT_DIG__ = @as(c_int, 64);
pub const __LDBL_MAX_10_EXP__ = @as(c_int, 4932);
pub const __LDBL_MAX_EXP__ = @as(c_int, 16384);
pub const __LDBL_MAX__ = @as(c_longdouble, 1.18973149535723176502e+4932);
pub const __LDBL_MIN_10_EXP__ = -@as(c_int, 4931);
pub const __LDBL_MIN_EXP__ = -@as(c_int, 16381);
pub const __LDBL_MIN__ = @as(c_longdouble, 3.36210314311209350626e-4932);
pub const __POINTER_WIDTH__ = @as(c_int, 64);
pub const __BIGGEST_ALIGNMENT__ = @as(c_int, 16);
pub const __WINT_UNSIGNED__ = @as(c_int, 1);
pub const __INT8_TYPE__ = i8;
pub const __INT8_FMTd__ = "hhd";
pub const __INT8_FMTi__ = "hhi";
pub const __INT8_C_SUFFIX__ = "";
pub const __INT16_TYPE__ = c_short;
pub const __INT16_FMTd__ = "hd";
pub const __INT16_FMTi__ = "hi";
pub const __INT16_C_SUFFIX__ = "";
pub const __INT32_TYPE__ = c_int;
pub const __INT32_FMTd__ = "d";
pub const __INT32_FMTi__ = "i";
pub const __INT32_C_SUFFIX__ = "";
pub const __INT64_TYPE__ = c_long;
pub const __INT64_FMTd__ = "ld";
pub const __INT64_FMTi__ = "li";
pub const __INT64_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `L`");
// (no file):198:9
pub const __UINT8_TYPE__ = u8;
pub const __UINT8_FMTo__ = "hho";
pub const __UINT8_FMTu__ = "hhu";
pub const __UINT8_FMTx__ = "hhx";
pub const __UINT8_FMTX__ = "hhX";
pub const __UINT8_C_SUFFIX__ = "";
pub const __UINT8_MAX__ = @as(c_int, 255);
pub const __INT8_MAX__ = @as(c_int, 127);
pub const __UINT16_TYPE__ = c_ushort;
pub const __UINT16_FMTo__ = "ho";
pub const __UINT16_FMTu__ = "hu";
pub const __UINT16_FMTx__ = "hx";
pub const __UINT16_FMTX__ = "hX";
pub const __UINT16_C_SUFFIX__ = "";
pub const __UINT16_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65535, .decimal);
pub const __INT16_MAX__ = @as(c_int, 32767);
pub const __UINT32_TYPE__ = c_uint;
pub const __UINT32_FMTo__ = "o";
pub const __UINT32_FMTu__ = "u";
pub const __UINT32_FMTx__ = "x";
pub const __UINT32_FMTX__ = "X";
pub const __UINT32_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `U`");
// (no file):220:9
pub const __UINT32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __INT32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __UINT64_TYPE__ = c_ulong;
pub const __UINT64_FMTo__ = "lo";
pub const __UINT64_FMTu__ = "lu";
pub const __UINT64_FMTx__ = "lx";
pub const __UINT64_FMTX__ = "lX";
pub const __UINT64_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `UL`");
// (no file):228:9
pub const __UINT64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __INT64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INT_LEAST8_TYPE__ = i8;
pub const __INT_LEAST8_MAX__ = @as(c_int, 127);
pub const __INT_LEAST8_WIDTH__ = @as(c_int, 8);
pub const __INT_LEAST8_FMTd__ = "hhd";
pub const __INT_LEAST8_FMTi__ = "hhi";
pub const __UINT_LEAST8_TYPE__ = u8;
pub const __UINT_LEAST8_MAX__ = @as(c_int, 255);
pub const __UINT_LEAST8_FMTo__ = "hho";
pub const __UINT_LEAST8_FMTu__ = "hhu";
pub const __UINT_LEAST8_FMTx__ = "hhx";
pub const __UINT_LEAST8_FMTX__ = "hhX";
pub const __INT_LEAST16_TYPE__ = c_short;
pub const __INT_LEAST16_MAX__ = @as(c_int, 32767);
pub const __INT_LEAST16_WIDTH__ = @as(c_int, 16);
pub const __INT_LEAST16_FMTd__ = "hd";
pub const __INT_LEAST16_FMTi__ = "hi";
pub const __UINT_LEAST16_TYPE__ = c_ushort;
pub const __UINT_LEAST16_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65535, .decimal);
pub const __UINT_LEAST16_FMTo__ = "ho";
pub const __UINT_LEAST16_FMTu__ = "hu";
pub const __UINT_LEAST16_FMTx__ = "hx";
pub const __UINT_LEAST16_FMTX__ = "hX";
pub const __INT_LEAST32_TYPE__ = c_int;
pub const __INT_LEAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __INT_LEAST32_WIDTH__ = @as(c_int, 32);
pub const __INT_LEAST32_FMTd__ = "d";
pub const __INT_LEAST32_FMTi__ = "i";
pub const __UINT_LEAST32_TYPE__ = c_uint;
pub const __UINT_LEAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __UINT_LEAST32_FMTo__ = "o";
pub const __UINT_LEAST32_FMTu__ = "u";
pub const __UINT_LEAST32_FMTx__ = "x";
pub const __UINT_LEAST32_FMTX__ = "X";
pub const __INT_LEAST64_TYPE__ = c_long;
pub const __INT_LEAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INT_LEAST64_WIDTH__ = @as(c_int, 64);
pub const __INT_LEAST64_FMTd__ = "ld";
pub const __INT_LEAST64_FMTi__ = "li";
pub const __UINT_LEAST64_TYPE__ = c_ulong;
pub const __UINT_LEAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINT_LEAST64_FMTo__ = "lo";
pub const __UINT_LEAST64_FMTu__ = "lu";
pub const __UINT_LEAST64_FMTx__ = "lx";
pub const __UINT_LEAST64_FMTX__ = "lX";
pub const __INT_FAST8_TYPE__ = i8;
pub const __INT_FAST8_MAX__ = @as(c_int, 127);
pub const __INT_FAST8_WIDTH__ = @as(c_int, 8);
pub const __INT_FAST8_FMTd__ = "hhd";
pub const __INT_FAST8_FMTi__ = "hhi";
pub const __UINT_FAST8_TYPE__ = u8;
pub const __UINT_FAST8_MAX__ = @as(c_int, 255);
pub const __UINT_FAST8_FMTo__ = "hho";
pub const __UINT_FAST8_FMTu__ = "hhu";
pub const __UINT_FAST8_FMTx__ = "hhx";
pub const __UINT_FAST8_FMTX__ = "hhX";
pub const __INT_FAST16_TYPE__ = c_short;
pub const __INT_FAST16_MAX__ = @as(c_int, 32767);
pub const __INT_FAST16_WIDTH__ = @as(c_int, 16);
pub const __INT_FAST16_FMTd__ = "hd";
pub const __INT_FAST16_FMTi__ = "hi";
pub const __UINT_FAST16_TYPE__ = c_ushort;
pub const __UINT_FAST16_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65535, .decimal);
pub const __UINT_FAST16_FMTo__ = "ho";
pub const __UINT_FAST16_FMTu__ = "hu";
pub const __UINT_FAST16_FMTx__ = "hx";
pub const __UINT_FAST16_FMTX__ = "hX";
pub const __INT_FAST32_TYPE__ = c_int;
pub const __INT_FAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __INT_FAST32_WIDTH__ = @as(c_int, 32);
pub const __INT_FAST32_FMTd__ = "d";
pub const __INT_FAST32_FMTi__ = "i";
pub const __UINT_FAST32_TYPE__ = c_uint;
pub const __UINT_FAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __UINT_FAST32_FMTo__ = "o";
pub const __UINT_FAST32_FMTu__ = "u";
pub const __UINT_FAST32_FMTx__ = "x";
pub const __UINT_FAST32_FMTX__ = "X";
pub const __INT_FAST64_TYPE__ = c_long;
pub const __INT_FAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INT_FAST64_WIDTH__ = @as(c_int, 64);
pub const __INT_FAST64_FMTd__ = "ld";
pub const __INT_FAST64_FMTi__ = "li";
pub const __UINT_FAST64_TYPE__ = c_ulong;
pub const __UINT_FAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINT_FAST64_FMTo__ = "lo";
pub const __UINT_FAST64_FMTu__ = "lu";
pub const __UINT_FAST64_FMTx__ = "lx";
pub const __UINT_FAST64_FMTX__ = "lX";
pub const __USER_LABEL_PREFIX__ = "";
pub const __FINITE_MATH_ONLY__ = @as(c_int, 0);
pub const __GNUC_STDC_INLINE__ = @as(c_int, 1);
pub const __GCC_ATOMIC_TEST_AND_SET_TRUEVAL = @as(c_int, 1);
pub const __CLANG_ATOMIC_BOOL_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_CHAR_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_CHAR16_T_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_CHAR32_T_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_WCHAR_T_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_SHORT_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_INT_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_LONG_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_LLONG_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_POINTER_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_BOOL_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_CHAR_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_CHAR16_T_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_CHAR32_T_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_WCHAR_T_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_SHORT_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_INT_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_LONG_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_LLONG_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_POINTER_LOCK_FREE = @as(c_int, 2);
pub const __NO_INLINE__ = @as(c_int, 1);
pub const __PIC__ = @as(c_int, 2);
pub const __pic__ = @as(c_int, 2);
pub const __PIE__ = @as(c_int, 2);
pub const __pie__ = @as(c_int, 2);
pub const __FLT_RADIX__ = @as(c_int, 2);
pub const __DECIMAL_DIG__ = __LDBL_DECIMAL_DIG__;
pub const __ELF__ = @as(c_int, 1);
pub const __GCC_ASM_FLAG_OUTPUTS__ = @as(c_int, 1);
pub const __code_model_small__ = @as(c_int, 1);
pub const __amd64__ = @as(c_int, 1);
pub const __amd64 = @as(c_int, 1);
pub const __x86_64 = @as(c_int, 1);
pub const __x86_64__ = @as(c_int, 1);
pub const __SEG_GS = @as(c_int, 1);
pub const __SEG_FS = @as(c_int, 1);
pub const __seg_gs = @compileError("unable to translate macro: undefined identifier `address_space`");
// (no file):359:9
pub const __seg_fs = @compileError("unable to translate macro: undefined identifier `address_space`");
// (no file):360:9
pub const __k8 = @as(c_int, 1);
pub const __k8__ = @as(c_int, 1);
pub const __tune_k8__ = @as(c_int, 1);
pub const __REGISTER_PREFIX__ = "";
pub const __NO_MATH_INLINES = @as(c_int, 1);
pub const __AES__ = @as(c_int, 1);
pub const __VAES__ = @as(c_int, 1);
pub const __PCLMUL__ = @as(c_int, 1);
pub const __VPCLMULQDQ__ = @as(c_int, 1);
pub const __LAHF_SAHF__ = @as(c_int, 1);
pub const __LZCNT__ = @as(c_int, 1);
pub const __RDRND__ = @as(c_int, 1);
pub const __FSGSBASE__ = @as(c_int, 1);
pub const __BMI__ = @as(c_int, 1);
pub const __BMI2__ = @as(c_int, 1);
pub const __POPCNT__ = @as(c_int, 1);
pub const __PRFCHW__ = @as(c_int, 1);
pub const __RDSEED__ = @as(c_int, 1);
pub const __ADX__ = @as(c_int, 1);
pub const __MOVBE__ = @as(c_int, 1);
pub const __FMA__ = @as(c_int, 1);
pub const __F16C__ = @as(c_int, 1);
pub const __GFNI__ = @as(c_int, 1);
pub const __SHA__ = @as(c_int, 1);
pub const __FXSR__ = @as(c_int, 1);
pub const __XSAVE__ = @as(c_int, 1);
pub const __XSAVEOPT__ = @as(c_int, 1);
pub const __XSAVEC__ = @as(c_int, 1);
pub const __XSAVES__ = @as(c_int, 1);
pub const __PKU__ = @as(c_int, 1);
pub const __CLFLUSHOPT__ = @as(c_int, 1);
pub const __CLWB__ = @as(c_int, 1);
pub const __SHSTK__ = @as(c_int, 1);
pub const __RDPID__ = @as(c_int, 1);
pub const __WAITPKG__ = @as(c_int, 1);
pub const __MOVDIRI__ = @as(c_int, 1);
pub const __MOVDIR64B__ = @as(c_int, 1);
pub const __PTWRITE__ = @as(c_int, 1);
pub const __INVPCID__ = @as(c_int, 1);
pub const __AVX2__ = @as(c_int, 1);
pub const __AVX__ = @as(c_int, 1);
pub const __SSE4_2__ = @as(c_int, 1);
pub const __SSE4_1__ = @as(c_int, 1);
pub const __SSSE3__ = @as(c_int, 1);
pub const __SSE3__ = @as(c_int, 1);
pub const __SSE2__ = @as(c_int, 1);
pub const __SSE2_MATH__ = @as(c_int, 1);
pub const __SSE__ = @as(c_int, 1);
pub const __SSE_MATH__ = @as(c_int, 1);
pub const __MMX__ = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 = @as(c_int, 1);
pub const __SIZEOF_FLOAT128__ = @as(c_int, 16);
pub const unix = @as(c_int, 1);
pub const __unix = @as(c_int, 1);
pub const __unix__ = @as(c_int, 1);
pub const linux = @as(c_int, 1);
pub const __linux = @as(c_int, 1);
pub const __linux__ = @as(c_int, 1);
pub const __gnu_linux__ = @as(c_int, 1);
pub const __FLOAT128__ = @as(c_int, 1);
pub const __STDC__ = @as(c_int, 1);
pub const __STDC_HOSTED__ = @as(c_int, 1);
pub const __STDC_VERSION__ = @as(c_long, 201710);
pub const __STDC_UTF_16__ = @as(c_int, 1);
pub const __STDC_UTF_32__ = @as(c_int, 1);
pub const _DEBUG = @as(c_int, 1);
pub const __GCC_HAVE_DWARF2_CFI_ASM = @as(c_int, 1);
pub const LINUX_IO_URING_H = "";
pub const _LINUX_FS_H = "";
pub const _LINUX_FSCRYPT_H = "";
pub const _LINUX_IOCTL_H = "";
pub const _ASM_GENERIC_IOCTL_H = "";
pub const _IOC_NRBITS = @as(c_int, 8);
pub const _IOC_TYPEBITS = @as(c_int, 8);
pub const _IOC_SIZEBITS = @as(c_int, 14);
pub const _IOC_DIRBITS = @as(c_int, 2);
pub const _IOC_NRMASK = (@as(c_int, 1) << _IOC_NRBITS) - @as(c_int, 1);
pub const _IOC_TYPEMASK = (@as(c_int, 1) << _IOC_TYPEBITS) - @as(c_int, 1);
pub const _IOC_SIZEMASK = (@as(c_int, 1) << _IOC_SIZEBITS) - @as(c_int, 1);
pub const _IOC_DIRMASK = (@as(c_int, 1) << _IOC_DIRBITS) - @as(c_int, 1);
pub const _IOC_NRSHIFT = @as(c_int, 0);
pub const _IOC_TYPESHIFT = _IOC_NRSHIFT + _IOC_NRBITS;
pub const _IOC_SIZESHIFT = _IOC_TYPESHIFT + _IOC_TYPEBITS;
pub const _IOC_DIRSHIFT = _IOC_SIZESHIFT + _IOC_SIZEBITS;
pub const _IOC_NONE = @as(c_uint, 0);
pub const _IOC_WRITE = @as(c_uint, 1);
pub const _IOC_READ = @as(c_uint, 2);
pub inline fn _IOC(dir: anytype, @"type": anytype, nr: anytype, size: anytype) @TypeOf((((dir << _IOC_DIRSHIFT) | (@"type" << _IOC_TYPESHIFT)) | (nr << _IOC_NRSHIFT)) | (size << _IOC_SIZESHIFT)) {
    _ = &dir;
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return (((dir << _IOC_DIRSHIFT) | (@"type" << _IOC_TYPESHIFT)) | (nr << _IOC_NRSHIFT)) | (size << _IOC_SIZESHIFT);
}
pub inline fn _IOC_TYPECHECK(t: anytype) @TypeOf(@import("std").zig.c_translation.sizeof(t)) {
    _ = &t;
    return @import("std").zig.c_translation.sizeof(t);
}
pub inline fn _IO(@"type": anytype, nr: anytype) @TypeOf(_IOC(_IOC_NONE, @"type", nr, @as(c_int, 0))) {
    _ = &@"type";
    _ = &nr;
    return _IOC(_IOC_NONE, @"type", nr, @as(c_int, 0));
}
pub inline fn _IOR(@"type": anytype, nr: anytype, size: anytype) @TypeOf(_IOC(_IOC_READ, @"type", nr, _IOC_TYPECHECK(size))) {
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return _IOC(_IOC_READ, @"type", nr, _IOC_TYPECHECK(size));
}
pub inline fn _IOW(@"type": anytype, nr: anytype, size: anytype) @TypeOf(_IOC(_IOC_WRITE, @"type", nr, _IOC_TYPECHECK(size))) {
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return _IOC(_IOC_WRITE, @"type", nr, _IOC_TYPECHECK(size));
}
pub inline fn _IOWR(@"type": anytype, nr: anytype, size: anytype) @TypeOf(_IOC(_IOC_READ | _IOC_WRITE, @"type", nr, _IOC_TYPECHECK(size))) {
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return _IOC(_IOC_READ | _IOC_WRITE, @"type", nr, _IOC_TYPECHECK(size));
}
pub inline fn _IOR_BAD(@"type": anytype, nr: anytype, size: anytype) @TypeOf(_IOC(_IOC_READ, @"type", nr, @import("std").zig.c_translation.sizeof(size))) {
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return _IOC(_IOC_READ, @"type", nr, @import("std").zig.c_translation.sizeof(size));
}
pub inline fn _IOW_BAD(@"type": anytype, nr: anytype, size: anytype) @TypeOf(_IOC(_IOC_WRITE, @"type", nr, @import("std").zig.c_translation.sizeof(size))) {
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return _IOC(_IOC_WRITE, @"type", nr, @import("std").zig.c_translation.sizeof(size));
}
pub inline fn _IOWR_BAD(@"type": anytype, nr: anytype, size: anytype) @TypeOf(_IOC(_IOC_READ | _IOC_WRITE, @"type", nr, @import("std").zig.c_translation.sizeof(size))) {
    _ = &@"type";
    _ = &nr;
    _ = &size;
    return _IOC(_IOC_READ | _IOC_WRITE, @"type", nr, @import("std").zig.c_translation.sizeof(size));
}
pub inline fn _IOC_DIR(nr: anytype) @TypeOf((nr >> _IOC_DIRSHIFT) & _IOC_DIRMASK) {
    _ = &nr;
    return (nr >> _IOC_DIRSHIFT) & _IOC_DIRMASK;
}
pub inline fn _IOC_TYPE(nr: anytype) @TypeOf((nr >> _IOC_TYPESHIFT) & _IOC_TYPEMASK) {
    _ = &nr;
    return (nr >> _IOC_TYPESHIFT) & _IOC_TYPEMASK;
}
pub inline fn _IOC_NR(nr: anytype) @TypeOf((nr >> _IOC_NRSHIFT) & _IOC_NRMASK) {
    _ = &nr;
    return (nr >> _IOC_NRSHIFT) & _IOC_NRMASK;
}
pub inline fn _IOC_SIZE(nr: anytype) @TypeOf((nr >> _IOC_SIZESHIFT) & _IOC_SIZEMASK) {
    _ = &nr;
    return (nr >> _IOC_SIZESHIFT) & _IOC_SIZEMASK;
}
pub const IOC_IN = _IOC_WRITE << _IOC_DIRSHIFT;
pub const IOC_OUT = _IOC_READ << _IOC_DIRSHIFT;
pub const IOC_INOUT = (_IOC_WRITE | _IOC_READ) << _IOC_DIRSHIFT;
pub const IOCSIZE_MASK = _IOC_SIZEMASK << _IOC_SIZESHIFT;
pub const IOCSIZE_SHIFT = _IOC_SIZESHIFT;
pub const _LINUX_TYPES_H = "";
pub const _ASM_GENERIC_TYPES_H = "";
pub const _ASM_GENERIC_INT_LL64_H = "";
pub const __ASM_X86_BITSPERLONG_H = "";
pub const __BITS_PER_LONG = @as(c_int, 64);
pub const __ASM_GENERIC_BITS_PER_LONG = "";
pub const __BITS_PER_LONG_LONG = @as(c_int, 64);
pub const _LINUX_POSIX_TYPES_H = "";
pub const _LINUX_STDDEF_H = "";
pub const __always_inline = @compileError("unable to translate C expr: unexpected token '__inline__'");
// ./linux/stddef.h:6:13
pub const __struct_group = @compileError("unable to translate C expr: expected ')' instead got '...'");
// ./linux/stddef.h:24:9
pub const __DECLARE_FLEX_ARRAY = @compileError("unable to translate macro: undefined identifier `__empty_`");
// ./linux/stddef.h:48:13
pub const __counted_by = @compileError("unable to translate C expr: unexpected token ''");
// ./linux/stddef.h:57:13
pub const __FD_SETSIZE = @as(c_int, 1024);
pub const _ASM_X86_POSIX_TYPES_64_H = "";
pub const __ASM_GENERIC_POSIX_TYPES_H = "";
pub const __bitwise = "";
pub const __bitwise__ = "";
pub const __aligned_u64 = @compileError("unable to translate macro: undefined identifier `aligned`");
// ./linux/types.h:50:13
pub const __aligned_be64 = @compileError("unable to translate macro: undefined identifier `aligned`");
// ./linux/types.h:51:13
pub const __aligned_le64 = @compileError("unable to translate macro: undefined identifier `aligned`");
// ./linux/types.h:52:13
pub const FSCRYPT_POLICY_FLAGS_PAD_4 = @as(c_int, 0x00);
pub const FSCRYPT_POLICY_FLAGS_PAD_8 = @as(c_int, 0x01);
pub const FSCRYPT_POLICY_FLAGS_PAD_16 = @as(c_int, 0x02);
pub const FSCRYPT_POLICY_FLAGS_PAD_32 = @as(c_int, 0x03);
pub const FSCRYPT_POLICY_FLAGS_PAD_MASK = @as(c_int, 0x03);
pub const FSCRYPT_POLICY_FLAG_DIRECT_KEY = @as(c_int, 0x04);
pub const FSCRYPT_POLICY_FLAG_IV_INO_LBLK_64 = @as(c_int, 0x08);
pub const FSCRYPT_POLICY_FLAG_IV_INO_LBLK_32 = @as(c_int, 0x10);
pub const FSCRYPT_MODE_AES_256_XTS = @as(c_int, 1);
pub const FSCRYPT_MODE_AES_256_CTS = @as(c_int, 4);
pub const FSCRYPT_MODE_AES_128_CBC = @as(c_int, 5);
pub const FSCRYPT_MODE_AES_128_CTS = @as(c_int, 6);
pub const FSCRYPT_MODE_SM4_XTS = @as(c_int, 7);
pub const FSCRYPT_MODE_SM4_CTS = @as(c_int, 8);
pub const FSCRYPT_MODE_ADIANTUM = @as(c_int, 9);
pub const FSCRYPT_MODE_AES_256_HCTR2 = @as(c_int, 10);
pub const FSCRYPT_POLICY_V1 = @as(c_int, 0);
pub const FSCRYPT_KEY_DESCRIPTOR_SIZE = @as(c_int, 8);
pub const FSCRYPT_KEY_DESC_PREFIX = "fscrypt:";
pub const FSCRYPT_KEY_DESC_PREFIX_SIZE = @as(c_int, 8);
pub const FSCRYPT_MAX_KEY_SIZE = @as(c_int, 64);
pub const FSCRYPT_POLICY_V2 = @as(c_int, 2);
pub const FSCRYPT_KEY_IDENTIFIER_SIZE = @as(c_int, 16);
pub const FSCRYPT_KEY_SPEC_TYPE_DESCRIPTOR = @as(c_int, 1);
pub const FSCRYPT_KEY_SPEC_TYPE_IDENTIFIER = @as(c_int, 2);
pub const FSCRYPT_KEY_REMOVAL_STATUS_FLAG_FILES_BUSY = @as(c_int, 0x00000001);
pub const FSCRYPT_KEY_REMOVAL_STATUS_FLAG_OTHER_USERS = @as(c_int, 0x00000002);
pub const FSCRYPT_KEY_STATUS_ABSENT = @as(c_int, 1);
pub const FSCRYPT_KEY_STATUS_PRESENT = @as(c_int, 2);
pub const FSCRYPT_KEY_STATUS_INCOMPLETELY_REMOVED = @as(c_int, 3);
pub const FSCRYPT_KEY_STATUS_FLAG_ADDED_BY_SELF = @as(c_int, 0x00000001);
pub const FS_IOC_SET_ENCRYPTION_POLICY = _IOR('f', @as(c_int, 19), struct_fscrypt_policy_v1);
pub const FS_IOC_GET_ENCRYPTION_PWSALT = _IOW('f', @as(c_int, 20), __u8[@as(usize, @intCast(@as(c_int, 16)))]);
pub const FS_IOC_GET_ENCRYPTION_POLICY = _IOW('f', @as(c_int, 21), struct_fscrypt_policy_v1);
pub const FS_IOC_GET_ENCRYPTION_POLICY_EX = _IOWR('f', @as(c_int, 22), __u8[@as(usize, @intCast(@as(c_int, 9)))]);
pub const FS_IOC_ADD_ENCRYPTION_KEY = _IOWR('f', @as(c_int, 23), struct_fscrypt_add_key_arg);
pub const FS_IOC_REMOVE_ENCRYPTION_KEY = _IOWR('f', @as(c_int, 24), struct_fscrypt_remove_key_arg);
pub const FS_IOC_REMOVE_ENCRYPTION_KEY_ALL_USERS = _IOWR('f', @as(c_int, 25), struct_fscrypt_remove_key_arg);
pub const FS_IOC_GET_ENCRYPTION_KEY_STATUS = _IOWR('f', @as(c_int, 26), struct_fscrypt_get_key_status_arg);
pub const FS_IOC_GET_ENCRYPTION_NONCE = _IOR('f', @as(c_int, 27), __u8[@as(usize, @intCast(@as(c_int, 16)))]);
pub const fscrypt_policy = fscrypt_policy_v1;
pub const FS_KEY_DESCRIPTOR_SIZE = FSCRYPT_KEY_DESCRIPTOR_SIZE;
pub const FS_POLICY_FLAGS_PAD_4 = FSCRYPT_POLICY_FLAGS_PAD_4;
pub const FS_POLICY_FLAGS_PAD_8 = FSCRYPT_POLICY_FLAGS_PAD_8;
pub const FS_POLICY_FLAGS_PAD_16 = FSCRYPT_POLICY_FLAGS_PAD_16;
pub const FS_POLICY_FLAGS_PAD_32 = FSCRYPT_POLICY_FLAGS_PAD_32;
pub const FS_POLICY_FLAGS_PAD_MASK = FSCRYPT_POLICY_FLAGS_PAD_MASK;
pub const FS_POLICY_FLAG_DIRECT_KEY = FSCRYPT_POLICY_FLAG_DIRECT_KEY;
pub const FS_POLICY_FLAGS_VALID = @as(c_int, 0x07);
pub const FS_ENCRYPTION_MODE_INVALID = @as(c_int, 0);
pub const FS_ENCRYPTION_MODE_AES_256_XTS = FSCRYPT_MODE_AES_256_XTS;
pub const FS_ENCRYPTION_MODE_AES_256_GCM = @as(c_int, 2);
pub const FS_ENCRYPTION_MODE_AES_256_CBC = @as(c_int, 3);
pub const FS_ENCRYPTION_MODE_AES_256_CTS = FSCRYPT_MODE_AES_256_CTS;
pub const FS_ENCRYPTION_MODE_AES_128_CBC = FSCRYPT_MODE_AES_128_CBC;
pub const FS_ENCRYPTION_MODE_AES_128_CTS = FSCRYPT_MODE_AES_128_CTS;
pub const FS_ENCRYPTION_MODE_ADIANTUM = FSCRYPT_MODE_ADIANTUM;
pub const FS_KEY_DESC_PREFIX = FSCRYPT_KEY_DESC_PREFIX;
pub const FS_KEY_DESC_PREFIX_SIZE = FSCRYPT_KEY_DESC_PREFIX_SIZE;
pub const FS_MAX_KEY_SIZE = FSCRYPT_MAX_KEY_SIZE;
pub const _LINUX_LIMITS_H = "";
pub const NR_OPEN = @as(c_int, 1024);
pub const NGROUPS_MAX = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65536, .decimal);
pub const ARG_MAX = @import("std").zig.c_translation.promoteIntLiteral(c_int, 131072, .decimal);
pub const LINK_MAX = @as(c_int, 127);
pub const MAX_CANON = @as(c_int, 255);
pub const MAX_INPUT = @as(c_int, 255);
pub const NAME_MAX = @as(c_int, 255);
pub const PATH_MAX = @as(c_int, 4096);
pub const PIPE_BUF = @as(c_int, 4096);
pub const XATTR_NAME_MAX = @as(c_int, 255);
pub const XATTR_SIZE_MAX = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65536, .decimal);
pub const XATTR_LIST_MAX = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65536, .decimal);
pub const RTSIG_MAX = @as(c_int, 32);
pub const _LINUX_MOUNT_H = "";
pub const MS_RDONLY = @as(c_int, 1);
pub const MS_NOSUID = @as(c_int, 2);
pub const MS_NODEV = @as(c_int, 4);
pub const MS_NOEXEC = @as(c_int, 8);
pub const MS_SYNCHRONOUS = @as(c_int, 16);
pub const MS_REMOUNT = @as(c_int, 32);
pub const MS_MANDLOCK = @as(c_int, 64);
pub const MS_DIRSYNC = @as(c_int, 128);
pub const MS_NOSYMFOLLOW = @as(c_int, 256);
pub const MS_NOATIME = @as(c_int, 1024);
pub const MS_NODIRATIME = @as(c_int, 2048);
pub const MS_BIND = @as(c_int, 4096);
pub const MS_MOVE = @as(c_int, 8192);
pub const MS_REC = @as(c_int, 16384);
pub const MS_VERBOSE = @import("std").zig.c_translation.promoteIntLiteral(c_int, 32768, .decimal);
pub const MS_SILENT = @import("std").zig.c_translation.promoteIntLiteral(c_int, 32768, .decimal);
pub const MS_POSIXACL = @as(c_int, 1) << @as(c_int, 16);
pub const MS_UNBINDABLE = @as(c_int, 1) << @as(c_int, 17);
pub const MS_PRIVATE = @as(c_int, 1) << @as(c_int, 18);
pub const MS_SLAVE = @as(c_int, 1) << @as(c_int, 19);
pub const MS_SHARED = @as(c_int, 1) << @as(c_int, 20);
pub const MS_RELATIME = @as(c_int, 1) << @as(c_int, 21);
pub const MS_KERNMOUNT = @as(c_int, 1) << @as(c_int, 22);
pub const MS_I_VERSION = @as(c_int, 1) << @as(c_int, 23);
pub const MS_STRICTATIME = @as(c_int, 1) << @as(c_int, 24);
pub const MS_LAZYTIME = @as(c_int, 1) << @as(c_int, 25);
pub const MS_SUBMOUNT = @as(c_int, 1) << @as(c_int, 26);
pub const MS_NOREMOTELOCK = @as(c_int, 1) << @as(c_int, 27);
pub const MS_NOSEC = @as(c_int, 1) << @as(c_int, 28);
pub const MS_BORN = @as(c_int, 1) << @as(c_int, 29);
pub const MS_ACTIVE = @as(c_int, 1) << @as(c_int, 30);
pub const MS_NOUSER = @as(c_int, 1) << @as(c_int, 31);
pub const MS_RMT_MASK = (((MS_RDONLY | MS_SYNCHRONOUS) | MS_MANDLOCK) | MS_I_VERSION) | MS_LAZYTIME;
pub const MS_MGC_VAL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0xC0ED0000, .hex);
pub const MS_MGC_MSK = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0xffff0000, .hex);
pub const OPEN_TREE_CLONE = @as(c_int, 1);
pub const OPEN_TREE_CLOEXEC = @compileError("unable to translate macro: undefined identifier `O_CLOEXEC`");
// ./linux/mount.h:65:9
pub const MOVE_MOUNT_F_SYMLINKS = @as(c_int, 0x00000001);
pub const MOVE_MOUNT_F_AUTOMOUNTS = @as(c_int, 0x00000002);
pub const MOVE_MOUNT_F_EMPTY_PATH = @as(c_int, 0x00000004);
pub const MOVE_MOUNT_T_SYMLINKS = @as(c_int, 0x00000010);
pub const MOVE_MOUNT_T_AUTOMOUNTS = @as(c_int, 0x00000020);
pub const MOVE_MOUNT_T_EMPTY_PATH = @as(c_int, 0x00000040);
pub const MOVE_MOUNT_SET_GROUP = @as(c_int, 0x00000100);
pub const MOVE_MOUNT_BENEATH = @as(c_int, 0x00000200);
pub const MOVE_MOUNT__MASK = @as(c_int, 0x00000377);
pub const FSOPEN_CLOEXEC = @as(c_int, 0x00000001);
pub const FSPICK_CLOEXEC = @as(c_int, 0x00000001);
pub const FSPICK_SYMLINK_NOFOLLOW = @as(c_int, 0x00000002);
pub const FSPICK_NO_AUTOMOUNT = @as(c_int, 0x00000004);
pub const FSPICK_EMPTY_PATH = @as(c_int, 0x00000008);
pub const FSMOUNT_CLOEXEC = @as(c_int, 0x00000001);
pub const MOUNT_ATTR_RDONLY = @as(c_int, 0x00000001);
pub const MOUNT_ATTR_NOSUID = @as(c_int, 0x00000002);
pub const MOUNT_ATTR_NODEV = @as(c_int, 0x00000004);
pub const MOUNT_ATTR_NOEXEC = @as(c_int, 0x00000008);
pub const MOUNT_ATTR__ATIME = @as(c_int, 0x00000070);
pub const MOUNT_ATTR_RELATIME = @as(c_int, 0x00000000);
pub const MOUNT_ATTR_NOATIME = @as(c_int, 0x00000010);
pub const MOUNT_ATTR_STRICTATIME = @as(c_int, 0x00000020);
pub const MOUNT_ATTR_NODIRATIME = @as(c_int, 0x00000080);
pub const MOUNT_ATTR_IDMAP = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00100000, .hex);
pub const MOUNT_ATTR_NOSYMFOLLOW = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00200000, .hex);
pub const MOUNT_ATTR_SIZE_VER0 = @as(c_int, 32);
pub const MNT_ID_REQ_SIZE_VER0 = @as(c_int, 24);
pub const STATMOUNT_SB_BASIC = @as(c_uint, 0x00000001);
pub const STATMOUNT_MNT_BASIC = @as(c_uint, 0x00000002);
pub const STATMOUNT_PROPAGATE_FROM = @as(c_uint, 0x00000004);
pub const STATMOUNT_MNT_ROOT = @as(c_uint, 0x00000008);
pub const STATMOUNT_MNT_POINT = @as(c_uint, 0x00000010);
pub const STATMOUNT_FS_TYPE = @as(c_uint, 0x00000020);
pub const LSMT_ROOT = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0xffffffffffffffff, .hex);
pub const INR_OPEN_CUR = @as(c_int, 1024);
pub const INR_OPEN_MAX = @as(c_int, 4096);
pub const BLOCK_SIZE_BITS = @as(c_int, 10);
pub const BLOCK_SIZE = @as(c_int, 1) << BLOCK_SIZE_BITS;
pub const SEEK_SET = @as(c_int, 0);
pub const SEEK_CUR = @as(c_int, 1);
pub const SEEK_END = @as(c_int, 2);
pub const SEEK_DATA = @as(c_int, 3);
pub const SEEK_HOLE = @as(c_int, 4);
pub const SEEK_MAX = SEEK_HOLE;
pub const RENAME_NOREPLACE = @as(c_int, 1) << @as(c_int, 0);
pub const RENAME_EXCHANGE = @as(c_int, 1) << @as(c_int, 1);
pub const RENAME_WHITEOUT = @as(c_int, 1) << @as(c_int, 2);
pub const FILE_DEDUPE_RANGE_SAME = @as(c_int, 0);
pub const FILE_DEDUPE_RANGE_DIFFERS = @as(c_int, 1);
pub const NR_FILE = @as(c_int, 8192);
pub const FS_XFLAG_REALTIME = @as(c_int, 0x00000001);
pub const FS_XFLAG_PREALLOC = @as(c_int, 0x00000002);
pub const FS_XFLAG_IMMUTABLE = @as(c_int, 0x00000008);
pub const FS_XFLAG_APPEND = @as(c_int, 0x00000010);
pub const FS_XFLAG_SYNC = @as(c_int, 0x00000020);
pub const FS_XFLAG_NOATIME = @as(c_int, 0x00000040);
pub const FS_XFLAG_NODUMP = @as(c_int, 0x00000080);
pub const FS_XFLAG_RTINHERIT = @as(c_int, 0x00000100);
pub const FS_XFLAG_PROJINHERIT = @as(c_int, 0x00000200);
pub const FS_XFLAG_NOSYMLINKS = @as(c_int, 0x00000400);
pub const FS_XFLAG_EXTSIZE = @as(c_int, 0x00000800);
pub const FS_XFLAG_EXTSZINHERIT = @as(c_int, 0x00001000);
pub const FS_XFLAG_NODEFRAG = @as(c_int, 0x00002000);
pub const FS_XFLAG_FILESTREAM = @as(c_int, 0x00004000);
pub const FS_XFLAG_DAX = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00008000, .hex);
pub const FS_XFLAG_COWEXTSIZE = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00010000, .hex);
pub const FS_XFLAG_HASATTR = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x80000000, .hex);
pub const BLKROSET = _IO(@as(c_int, 0x12), @as(c_int, 93));
pub const BLKROGET = _IO(@as(c_int, 0x12), @as(c_int, 94));
pub const BLKRRPART = _IO(@as(c_int, 0x12), @as(c_int, 95));
pub const BLKGETSIZE = _IO(@as(c_int, 0x12), @as(c_int, 96));
pub const BLKFLSBUF = _IO(@as(c_int, 0x12), @as(c_int, 97));
pub const BLKRASET = _IO(@as(c_int, 0x12), @as(c_int, 98));
pub const BLKRAGET = _IO(@as(c_int, 0x12), @as(c_int, 99));
pub const BLKFRASET = _IO(@as(c_int, 0x12), @as(c_int, 100));
pub const BLKFRAGET = _IO(@as(c_int, 0x12), @as(c_int, 101));
pub const BLKSECTSET = _IO(@as(c_int, 0x12), @as(c_int, 102));
pub const BLKSECTGET = _IO(@as(c_int, 0x12), @as(c_int, 103));
pub const BLKSSZGET = _IO(@as(c_int, 0x12), @as(c_int, 104));
pub const BLKBSZGET = @compileError("unable to translate macro: undefined identifier `size_t`");
// ./linux/fs.h:184:9
pub const BLKBSZSET = @compileError("unable to translate macro: undefined identifier `size_t`");
// ./linux/fs.h:185:9
pub const BLKGETSIZE64 = @compileError("unable to translate macro: undefined identifier `size_t`");
// ./linux/fs.h:186:9
pub const BLKTRACESETUP = @compileError("unable to translate macro: undefined identifier `blk_user_trace_setup`");
// ./linux/fs.h:187:9
pub const BLKTRACESTART = _IO(@as(c_int, 0x12), @as(c_int, 116));
pub const BLKTRACESTOP = _IO(@as(c_int, 0x12), @as(c_int, 117));
pub const BLKTRACETEARDOWN = _IO(@as(c_int, 0x12), @as(c_int, 118));
pub const BLKDISCARD = _IO(@as(c_int, 0x12), @as(c_int, 119));
pub const BLKIOMIN = _IO(@as(c_int, 0x12), @as(c_int, 120));
pub const BLKIOOPT = _IO(@as(c_int, 0x12), @as(c_int, 121));
pub const BLKALIGNOFF = _IO(@as(c_int, 0x12), @as(c_int, 122));
pub const BLKPBSZGET = _IO(@as(c_int, 0x12), @as(c_int, 123));
pub const BLKDISCARDZEROES = _IO(@as(c_int, 0x12), @as(c_int, 124));
pub const BLKSECDISCARD = _IO(@as(c_int, 0x12), @as(c_int, 125));
pub const BLKROTATIONAL = _IO(@as(c_int, 0x12), @as(c_int, 126));
pub const BLKZEROOUT = _IO(@as(c_int, 0x12), @as(c_int, 127));
pub const BLKGETDISKSEQ = _IOR(@as(c_int, 0x12), @as(c_int, 128), __u64);
pub const BMAP_IOCTL = @as(c_int, 1);
pub const FIBMAP = _IO(@as(c_int, 0x00), @as(c_int, 1));
pub const FIGETBSZ = _IO(@as(c_int, 0x00), @as(c_int, 2));
pub const FIFREEZE = _IOWR('X', @as(c_int, 119), c_int);
pub const FITHAW = _IOWR('X', @as(c_int, 120), c_int);
pub const FITRIM = _IOWR('X', @as(c_int, 121), struct_fstrim_range);
pub const FICLONE = _IOW(@as(c_int, 0x94), @as(c_int, 9), c_int);
pub const FICLONERANGE = _IOW(@as(c_int, 0x94), @as(c_int, 13), struct_file_clone_range);
pub const FIDEDUPERANGE = _IOWR(@as(c_int, 0x94), @as(c_int, 54), struct_file_dedupe_range);
pub const FSLABEL_MAX = @as(c_int, 256);
pub const FS_IOC_GETFLAGS = _IOR('f', @as(c_int, 1), c_long);
pub const FS_IOC_SETFLAGS = _IOW('f', @as(c_int, 2), c_long);
pub const FS_IOC_GETVERSION = _IOR('v', @as(c_int, 1), c_long);
pub const FS_IOC_SETVERSION = _IOW('v', @as(c_int, 2), c_long);
pub const FS_IOC_FIEMAP = @compileError("unable to translate macro: undefined identifier `fiemap`");
// ./linux/fs.h:222:9
pub const FS_IOC32_GETFLAGS = _IOR('f', @as(c_int, 1), c_int);
pub const FS_IOC32_SETFLAGS = _IOW('f', @as(c_int, 2), c_int);
pub const FS_IOC32_GETVERSION = _IOR('v', @as(c_int, 1), c_int);
pub const FS_IOC32_SETVERSION = _IOW('v', @as(c_int, 2), c_int);
pub const FS_IOC_FSGETXATTR = _IOR('X', @as(c_int, 31), struct_fsxattr);
pub const FS_IOC_FSSETXATTR = _IOW('X', @as(c_int, 32), struct_fsxattr);
pub const FS_IOC_GETFSLABEL = _IOR(@as(c_int, 0x94), @as(c_int, 49), u8[@as(usize, @intCast(FSLABEL_MAX))]);
pub const FS_IOC_SETFSLABEL = _IOW(@as(c_int, 0x94), @as(c_int, 50), u8[@as(usize, @intCast(FSLABEL_MAX))]);
pub const FS_IOC_GETFSUUID = _IOR(@as(c_int, 0x15), @as(c_int, 0), struct_fsuuid2);
pub const FS_IOC_GETFSSYSFSPATH = _IOR(@as(c_int, 0x15), @as(c_int, 1), struct_fs_sysfs_path);
pub const FS_SECRM_FL = @as(c_int, 0x00000001);
pub const FS_UNRM_FL = @as(c_int, 0x00000002);
pub const FS_COMPR_FL = @as(c_int, 0x00000004);
pub const FS_SYNC_FL = @as(c_int, 0x00000008);
pub const FS_IMMUTABLE_FL = @as(c_int, 0x00000010);
pub const FS_APPEND_FL = @as(c_int, 0x00000020);
pub const FS_NODUMP_FL = @as(c_int, 0x00000040);
pub const FS_NOATIME_FL = @as(c_int, 0x00000080);
pub const FS_DIRTY_FL = @as(c_int, 0x00000100);
pub const FS_COMPRBLK_FL = @as(c_int, 0x00000200);
pub const FS_NOCOMP_FL = @as(c_int, 0x00000400);
pub const FS_ENCRYPT_FL = @as(c_int, 0x00000800);
pub const FS_BTREE_FL = @as(c_int, 0x00001000);
pub const FS_INDEX_FL = @as(c_int, 0x00001000);
pub const FS_IMAGIC_FL = @as(c_int, 0x00002000);
pub const FS_JOURNAL_DATA_FL = @as(c_int, 0x00004000);
pub const FS_NOTAIL_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00008000, .hex);
pub const FS_DIRSYNC_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00010000, .hex);
pub const FS_TOPDIR_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00020000, .hex);
pub const FS_HUGE_FILE_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00040000, .hex);
pub const FS_EXTENT_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00080000, .hex);
pub const FS_VERITY_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00100000, .hex);
pub const FS_EA_INODE_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00200000, .hex);
pub const FS_EOFBLOCKS_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00400000, .hex);
pub const FS_NOCOW_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x00800000, .hex);
pub const FS_DAX_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x02000000, .hex);
pub const FS_INLINE_DATA_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x10000000, .hex);
pub const FS_PROJINHERIT_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x20000000, .hex);
pub const FS_CASEFOLD_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x40000000, .hex);
pub const FS_RESERVED_FL = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x80000000, .hex);
pub const FS_FL_USER_VISIBLE = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x0003DFFF, .hex);
pub const FS_FL_USER_MODIFIABLE = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0x000380FF, .hex);
pub const SYNC_FILE_RANGE_WAIT_BEFORE = @as(c_int, 1);
pub const SYNC_FILE_RANGE_WRITE = @as(c_int, 2);
pub const SYNC_FILE_RANGE_WAIT_AFTER = @as(c_int, 4);
pub const SYNC_FILE_RANGE_WRITE_AND_WAIT = (SYNC_FILE_RANGE_WRITE | SYNC_FILE_RANGE_WAIT_BEFORE) | SYNC_FILE_RANGE_WAIT_AFTER;
pub const RWF_HIPRI = @import("std").zig.c_translation.cast(__kernel_rwf_t, @as(c_int, 0x00000001));
pub const RWF_DSYNC = @import("std").zig.c_translation.cast(__kernel_rwf_t, @as(c_int, 0x00000002));
pub const RWF_SYNC = @import("std").zig.c_translation.cast(__kernel_rwf_t, @as(c_int, 0x00000004));
pub const RWF_NOWAIT = @import("std").zig.c_translation.cast(__kernel_rwf_t, @as(c_int, 0x00000008));
pub const RWF_APPEND = @import("std").zig.c_translation.cast(__kernel_rwf_t, @as(c_int, 0x00000010));
pub const RWF_NOAPPEND = @import("std").zig.c_translation.cast(__kernel_rwf_t, @as(c_int, 0x00000020));
pub const RWF_SUPPORTED = ((((RWF_HIPRI | RWF_DSYNC) | RWF_SYNC) | RWF_NOWAIT) | RWF_APPEND) | RWF_NOAPPEND;
pub const PAGEMAP_SCAN = _IOWR('f', @as(c_int, 16), struct_pm_scan_arg);
pub const PAGE_IS_WPALLOWED = @as(c_int, 1) << @as(c_int, 0);
pub const PAGE_IS_WRITTEN = @as(c_int, 1) << @as(c_int, 1);
pub const PAGE_IS_FILE = @as(c_int, 1) << @as(c_int, 2);
pub const PAGE_IS_PRESENT = @as(c_int, 1) << @as(c_int, 3);
pub const PAGE_IS_SWAPPED = @as(c_int, 1) << @as(c_int, 4);
pub const PAGE_IS_PFNZERO = @as(c_int, 1) << @as(c_int, 5);
pub const PAGE_IS_HUGE = @as(c_int, 1) << @as(c_int, 6);
pub const PAGE_IS_SOFT_DIRTY = @as(c_int, 1) << @as(c_int, 7);
pub const PM_SCAN_WP_MATCHING = @as(c_int, 1) << @as(c_int, 0);
pub const PM_SCAN_CHECK_WPASYNC = @as(c_int, 1) << @as(c_int, 1);
pub const _LINUX_TIME_TYPES_H = "";
pub const IORING_FILE_INDEX_ALLOC = ~@as(c_uint, 0);
pub const IOSQE_FIXED_FILE = @as(c_uint, 1) << IOSQE_FIXED_FILE_BIT;
pub const IOSQE_IO_DRAIN = @as(c_uint, 1) << IOSQE_IO_DRAIN_BIT;
pub const IOSQE_IO_LINK = @as(c_uint, 1) << IOSQE_IO_LINK_BIT;
pub const IOSQE_IO_HARDLINK = @as(c_uint, 1) << IOSQE_IO_HARDLINK_BIT;
pub const IOSQE_ASYNC = @as(c_uint, 1) << IOSQE_ASYNC_BIT;
pub const IOSQE_BUFFER_SELECT = @as(c_uint, 1) << IOSQE_BUFFER_SELECT_BIT;
pub const IOSQE_CQE_SKIP_SUCCESS = @as(c_uint, 1) << IOSQE_CQE_SKIP_SUCCESS_BIT;
pub const IORING_SETUP_IOPOLL = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_SETUP_SQPOLL = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_SETUP_SQ_AFF = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_SETUP_CQSIZE = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_SETUP_CLAMP = @as(c_uint, 1) << @as(c_int, 4);
pub const IORING_SETUP_ATTACH_WQ = @as(c_uint, 1) << @as(c_int, 5);
pub const IORING_SETUP_R_DISABLED = @as(c_uint, 1) << @as(c_int, 6);
pub const IORING_SETUP_SUBMIT_ALL = @as(c_uint, 1) << @as(c_int, 7);
pub const IORING_SETUP_COOP_TASKRUN = @as(c_uint, 1) << @as(c_int, 8);
pub const IORING_SETUP_TASKRUN_FLAG = @as(c_uint, 1) << @as(c_int, 9);
pub const IORING_SETUP_SQE128 = @as(c_uint, 1) << @as(c_int, 10);
pub const IORING_SETUP_CQE32 = @as(c_uint, 1) << @as(c_int, 11);
pub const IORING_SETUP_SINGLE_ISSUER = @as(c_uint, 1) << @as(c_int, 12);
pub const IORING_SETUP_DEFER_TASKRUN = @as(c_uint, 1) << @as(c_int, 13);
pub const IORING_SETUP_NO_MMAP = @as(c_uint, 1) << @as(c_int, 14);
pub const IORING_SETUP_REGISTERED_FD_ONLY = @as(c_uint, 1) << @as(c_int, 15);
pub const IORING_SETUP_NO_SQARRAY = @as(c_uint, 1) << @as(c_int, 16);
pub const IORING_URING_CMD_FIXED = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_URING_CMD_MASK = IORING_URING_CMD_FIXED;
pub const IORING_FSYNC_DATASYNC = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_TIMEOUT_ABS = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_TIMEOUT_UPDATE = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_TIMEOUT_BOOTTIME = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_TIMEOUT_REALTIME = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_LINK_TIMEOUT_UPDATE = @as(c_uint, 1) << @as(c_int, 4);
pub const IORING_TIMEOUT_ETIME_SUCCESS = @as(c_uint, 1) << @as(c_int, 5);
pub const IORING_TIMEOUT_MULTISHOT = @as(c_uint, 1) << @as(c_int, 6);
pub const IORING_TIMEOUT_CLOCK_MASK = IORING_TIMEOUT_BOOTTIME | IORING_TIMEOUT_REALTIME;
pub const IORING_TIMEOUT_UPDATE_MASK = IORING_TIMEOUT_UPDATE | IORING_LINK_TIMEOUT_UPDATE;
pub const SPLICE_F_FD_IN_FIXED = @as(c_uint, 1) << @as(c_int, 31);
pub const IORING_POLL_ADD_MULTI = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_POLL_UPDATE_EVENTS = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_POLL_UPDATE_USER_DATA = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_POLL_ADD_LEVEL = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_ASYNC_CANCEL_ALL = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_ASYNC_CANCEL_FD = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_ASYNC_CANCEL_ANY = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_ASYNC_CANCEL_FD_FIXED = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_ASYNC_CANCEL_USERDATA = @as(c_uint, 1) << @as(c_int, 4);
pub const IORING_ASYNC_CANCEL_OP = @as(c_uint, 1) << @as(c_int, 5);
pub const IORING_RECVSEND_POLL_FIRST = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_RECV_MULTISHOT = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_RECVSEND_FIXED_BUF = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_SEND_ZC_REPORT_USAGE = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_NOTIF_USAGE_ZC_COPIED = @as(c_uint, 1) << @as(c_int, 31);
pub const IORING_ACCEPT_MULTISHOT = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_MSG_RING_CQE_SKIP = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_MSG_RING_FLAGS_PASS = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_FIXED_FD_NO_CLOEXEC = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_CQE_F_BUFFER = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_CQE_F_MORE = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_CQE_F_SOCK_NONEMPTY = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_CQE_F_NOTIF = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_OFF_SQ_RING = @as(c_ulonglong, 0);
pub const IORING_OFF_CQ_RING = @as(c_ulonglong, 0x8000000);
pub const IORING_OFF_SQES = @as(c_ulonglong, 0x10000000);
pub const IORING_OFF_PBUF_RING = @as(c_ulonglong, 0x80000000);
pub const IORING_OFF_PBUF_SHIFT = @as(c_int, 16);
pub const IORING_OFF_MMAP_MASK = @as(c_ulonglong, 0xf8000000);
pub const IORING_SQ_NEED_WAKEUP = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_SQ_CQ_OVERFLOW = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_SQ_TASKRUN = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_CQ_EVENTFD_DISABLED = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_ENTER_GETEVENTS = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_ENTER_SQ_WAKEUP = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_ENTER_SQ_WAIT = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_ENTER_EXT_ARG = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_ENTER_REGISTERED_RING = @as(c_uint, 1) << @as(c_int, 4);
pub const IORING_FEAT_SINGLE_MMAP = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_FEAT_NODROP = @as(c_uint, 1) << @as(c_int, 1);
pub const IORING_FEAT_SUBMIT_STABLE = @as(c_uint, 1) << @as(c_int, 2);
pub const IORING_FEAT_RW_CUR_POS = @as(c_uint, 1) << @as(c_int, 3);
pub const IORING_FEAT_CUR_PERSONALITY = @as(c_uint, 1) << @as(c_int, 4);
pub const IORING_FEAT_FAST_POLL = @as(c_uint, 1) << @as(c_int, 5);
pub const IORING_FEAT_POLL_32BITS = @as(c_uint, 1) << @as(c_int, 6);
pub const IORING_FEAT_SQPOLL_NONFIXED = @as(c_uint, 1) << @as(c_int, 7);
pub const IORING_FEAT_EXT_ARG = @as(c_uint, 1) << @as(c_int, 8);
pub const IORING_FEAT_NATIVE_WORKERS = @as(c_uint, 1) << @as(c_int, 9);
pub const IORING_FEAT_RSRC_TAGS = @as(c_uint, 1) << @as(c_int, 10);
pub const IORING_FEAT_CQE_SKIP = @as(c_uint, 1) << @as(c_int, 11);
pub const IORING_FEAT_LINKED_FILE = @as(c_uint, 1) << @as(c_int, 12);
pub const IORING_FEAT_REG_REG_RING = @as(c_uint, 1) << @as(c_int, 13);
pub const IORING_RSRC_REGISTER_SPARSE = @as(c_uint, 1) << @as(c_int, 0);
pub const IORING_REGISTER_FILES_SKIP = -@as(c_int, 2);
pub const IO_URING_OP_SUPPORTED = @as(c_uint, 1) << @as(c_int, 0);
pub const fscrypt_policy_v1 = struct_fscrypt_policy_v1;
pub const fscrypt_key = struct_fscrypt_key;
pub const fscrypt_policy_v2 = struct_fscrypt_policy_v2;
pub const fscrypt_get_policy_ex_arg = struct_fscrypt_get_policy_ex_arg;
pub const fscrypt_key_specifier = struct_fscrypt_key_specifier;
pub const fscrypt_provisioning_key_payload = struct_fscrypt_provisioning_key_payload;
pub const fscrypt_add_key_arg = struct_fscrypt_add_key_arg;
pub const fscrypt_remove_key_arg = struct_fscrypt_remove_key_arg;
pub const fscrypt_get_key_status_arg = struct_fscrypt_get_key_status_arg;
pub const fsconfig_command = enum_fsconfig_command;
pub const mount_attr = struct_mount_attr;
pub const statmount = struct_statmount;
pub const mnt_id_req = struct_mnt_id_req;
pub const file_clone_range = struct_file_clone_range;
pub const fstrim_range = struct_fstrim_range;
pub const fsuuid2 = struct_fsuuid2;
pub const fs_sysfs_path = struct_fs_sysfs_path;
pub const file_dedupe_range_info = struct_file_dedupe_range_info;
pub const file_dedupe_range = struct_file_dedupe_range;
pub const files_stat_struct = struct_files_stat_struct;
pub const inodes_stat_t = struct_inodes_stat_t;
pub const fsxattr = struct_fsxattr;
pub const page_region = struct_page_region;
pub const pm_scan_arg = struct_pm_scan_arg;
pub const __kernel_timespec = struct___kernel_timespec;
pub const __kernel_itimerspec = struct___kernel_itimerspec;
pub const __kernel_old_timeval = struct___kernel_old_timeval;
pub const __kernel_old_timespec = struct___kernel_old_timespec;
pub const __kernel_old_itimerval = struct___kernel_old_itimerval;
pub const __kernel_sock_timeval = struct___kernel_sock_timeval;
pub const io_uring_sqe = struct_io_uring_sqe;
pub const io_uring_op = enum_io_uring_op;
pub const io_uring_cqe = struct_io_uring_cqe;
pub const io_sqring_offsets = struct_io_sqring_offsets;
pub const io_cqring_offsets = struct_io_cqring_offsets;
pub const io_uring_params = struct_io_uring_params;
pub const io_uring_files_update = struct_io_uring_files_update;
pub const io_uring_rsrc_register = struct_io_uring_rsrc_register;
pub const io_uring_rsrc_update = struct_io_uring_rsrc_update;
pub const io_uring_rsrc_update2 = struct_io_uring_rsrc_update2;
pub const io_uring_probe_op = struct_io_uring_probe_op;
pub const io_uring_probe = struct_io_uring_probe;
pub const io_uring_restriction = struct_io_uring_restriction;
pub const io_uring_buf = struct_io_uring_buf;
pub const io_uring_buf_ring = struct_io_uring_buf_ring;
pub const io_uring_buf_reg = struct_io_uring_buf_reg;
pub const io_uring_buf_status = struct_io_uring_buf_status;
pub const io_uring_napi = struct_io_uring_napi;
pub const io_uring_getevents_arg = struct_io_uring_getevents_arg;
pub const io_uring_sync_cancel_reg = struct_io_uring_sync_cancel_reg;
pub const io_uring_file_index_range = struct_io_uring_file_index_range;
pub const io_uring_recvmsg_out = struct_io_uring_recvmsg_out;
