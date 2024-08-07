const vortex = @import("vortex");
const assert = @import("common.zig").assert;

const IoUring = vortex.os.linux.IoUring;

test "AsyncCancel" {
    try assert(IoUring.AsyncCancel, .{
        .all = 1 << 0,
        .fd = 1 << 1,
        .any = 1 << 2,
        .fd_fixed = 1 << 3,
        .userdata = 1 << 4,
        .op = 1 << 5,
    });
}

test "SqringOffsetsFlags" {
    try assert(IoUring.SqringOffsetsFlags, .{
        .need_wakeup = 1 << 0,
        .cq_overflow = 1 << 1,
        .taskrun = 1 << 2,
    });
}

test "CqringOffsetsFlags" {
    try assert(IoUring.CqringOffsetsFlags, .{
        .eventfd_disabled = 1 << 0,
    });
}

test "ParamsFeatures" {
    try assert(IoUring.ParamsFeatures, .{
        .single_mmap = 1 << 0,
        .nodrop = 1 << 1,
        .submit_stable = 1 << 2,
        .rw_cur_pos = 1 << 3,
        .cur_personality = 1 << 4,
        .fast_poll = 1 << 5,
        .poll_32bits = 1 << 6,
        .sqpoll_nonfixed = 1 << 7,
        .ext_arg = 1 << 8,
        .native_workers = 1 << 9,
        .rsrc_tags = 1 << 10,
        .cqe_skip = 1 << 11,
        .linked_file = 1 << 12,
        .reg_reg_ring = 1 << 13,
    });
}

test "EnterFlags" {
    try assert(IoUring.EnterFlags, .{
        .getevents = 1 << 0,
        .sq_wakeup = 1 << 1,
        .sq_wait = 1 << 2,
        .ext_arg = 1 << 3,
        .registered_ring = 1 << 4,
    });
}

test "SqeFlags" {
    try assert(IoUring.SqeFlags, .{
        .fixed_file = 1 << 0,
        .io_drain = 1 << 1,
        .io_link = 1 << 2,
        .io_hardlink = 1 << 3,
        .io_async = 1 << 4,
        .buffer_select = 1 << 5,
        .cqe_skip_sucess = 1 << 6,
    });
}

test "SqeIoprio" {
    try assert(IoUring.SqeIoprio, .{
        .recvsend_poll_first = 1 << 0,
        .rescv_mutlishot = 1 << 1,
        .recvsend_fixed_buf = 1 << 2,
        .send_zc_report_usage = 1 << 3,
    });
}

test "SqeFsyncFlags" {
    try assert(IoUring.SqeFsyncFlags, .{
        .datasync = 1 << 0,
    });
}

test "SqePollEvents" {
    try assert(IoUring.SqePollEvents, .{
        .add_mutli = 1 << 0,
        .update_events = 1 << 1,
        .update_user_data = 1 << 2,
        .add_level = 1 << 3,
    });
}

test "SqeTimeoutFlags" {
    try assert(IoUring.SqeTimeoutFlags, .{
        .abs = 1 << 0,
        .update = 1 << 1,
        .boottime = 1 << 2,
        .realtime = 1 << 3,
        .link_update = 1 << 4,
        .etime_success = 1 << 5,
        .multishot = 1 << 6,
    });
}

test "SqeSpliceFlags" {
    try assert(IoUring.SqeSpliceFlags, .{
        .fd_in_fixed = 1 << 31,
    });
}

test "SqeMsgRingFlags" {
    try assert(IoUring.SqeMsgRingFlags, .{
        .cqe_skip = 1 << 0,
        .flags_pass = 1 << 1,
    });
}

test "SqeUringCmdFlags" {
    try assert(IoUring.SqeUringCmdFlags, .{
        .fixed = 1 << 0,
    });
}

test "SqeInstallFdFlags" {
    try assert(IoUring.SqeInstallFdFlags, .{
        .no_cloexec = 1 << 0,
    });
}

test "NotifUsageZcCopied" {
    try assert(IoUring.NotifUsageZcCopied, .{
        .usage_zc_copied = 1 << 31,
    });
}

test "CqeFlags" {
    try assert(IoUring.CqeFlags, .{
        .buffer = 1 << 0,
        .more = 1 << 1,
        .sock_nonempty = 1 << 2,
        .notif = 1 << 3,
    });
}

test "SetupFlags" {
    try assert(IoUring.SetupFlags, .{
        .iopoll = 1 << 0,
        .sqpoll = 1 << 1,
        .sq_aff = 1 << 2,
        .cqsize = 1 << 3,
        .clamp = 1 << 4,
        .attach_wq = 1 << 5,
        .r_disabled = 1 << 6,
        .submit_all = 1 << 7,
        .coop_taskrun = 1 << 8,
        .taskrun_flag = 1 << 9,
        .sqe128 = 1 << 10,
        .cqe32 = 1 << 11,
        .single_issuer = 1 << 12,
        .defer_taskrun = 1 << 13,
        .no_mmap = 1 << 14,
        .registered_fd_only = 1 << 15,
        .no_sqarray = 1 << 16,
    });
}
