// Manually translated uapi's io_uring header from Linux 6.9.
// Reference: https://github.com/torvalds/linux/blob/a38297e3fb012ddfa7ce0321a7e5a8daeb1872b6/include/uapi/linux/io_uring.h

const mem = @import("../../../mem/mem.zig");

/// if sqe->file_index is set to this for opcodes that instantiate a new
/// direct descriptor (like openat/openat2/accept), then io_uring will allocate
/// an available direct descriptor instead of having the application pass one
/// in. the picked direct descriptor will be returned in cqe->res, or -enfile
/// if the sp
pub const file_index_alloc: c_uint = ~@as(c_uint, 0);

/// IOSQE_BUFFER_SELECT
///   Used in conjunction with the IORING_OP_PROVIDE_BUFFERS
///   command, which registers a pool of buffers to be used by
///   commands that read or receive data. When buffers are
///   registered for this use case, and this flag is set in the
///   command, io_uring will grab a buffer from this pool when
///   the request is ready to receive or read data. If
///   successful, the resulting CQE will have
///   IORING_CQE_F_BUFFER set in the flags part of the struct,
///   and the upper IORING_CQE_BUFFER_SHIFT bits will contain
///   the ID of the selected buffers. This allows the
///   application to know exactly which buffer was selected for
///   the operation.
pub const cqe_buffer_shift = 16;

/// skip updating fd indexes set to this value in the fd table
pub const register_files_skip: c_int = -2;

/// IORING_REGISTER_PROBE
///   This operation returns a structure, io_uring_probe, which
///   contains information about the opcodes supported by
///   io_uring on the running kernel.  arg must contain a
///   pointer to a struct io_uring_probe, and nr_args must
///   contain the size of the ops array in that probe struct.
///   The ops array is of the type io_uring_probe_op, which
///   holds the value of the opcode and a flags field. If the
///   flags field has IO_URING_OP_SUPPORTED set, then this
///   opcode is supported on the running kernel.
pub const op_supported: c_int = 1;

/// Register a fully sparse file space, rather than pass in an array of all -1 file descriptors.
pub const rsrc_register_sparse: c_int = 1;

/// magic offsets for the application to mmap the data it needs
pub const mmap_offsets = struct {
    pub const sq_ring: c_ulonglong = 0;
    pub const cq_ring: c_ulonglong = 0x8000000;
    pub const sqes: c_ulonglong = 0x10000000;
    pub const pbuf_ring: c_ulonglong = 0x80000000;
    pub const pbuf_shift: c_int = 16;
    pub const mmap_mask: c_ulonglong = 0xf8000000;
};

/// flags for register_pbuf_ring.
///
/// pbuf_ring_mmap: if set, kernel will allocate the memory for the ring.
///   the application must not set a ring_addr in extern struct
///   io_uring_buf_reg, instead it must subsequently call
///   mmap(2) with the offset set as:
///   off_pbuf_ring | (bgid << off_pbuf_shift)
///   to get a virtual mapping for the ring.
pub const PbufRingFlags = enum(c_int) {
    ring_mmap = 1,
};

/// io-wq worker categories
pub const IoWq = enum(c_int) {
    bound,
    unbound,
};

/// argument for op_uring_cmd when file is a socket
pub const socket_uring = enum(c_int) {
    op_siocinq = 0,
    op_siocoutq = 1,
    op_getsockopt = 2,
    op_setsockopt = 3,
};

pub const iosqe = enum(c_int) {
    fixed_file_bit = 0,
    io_drain_bit = 1,
    io_link_bit = 2,
    io_hardlink_bit = 3,
    async_bit = 4,
    buffer_select_bit = 5,
    cqe_skip_success_bit = 6,
};

pub const RsrcRegister = extern struct {
    nr: u32 = 0,
    flags: u32 = 0,
    resv2: u64 = 0,
    data: u64 align(8) = 0,
    tags: u64 align(8) = 0,
};

pub const RsrcUpdate = extern struct {
    offset: u32 = 0,
    resv: u32 = 0,
    data: u64 align(8) = 0,
};

pub const RsrcUpdate2 = extern struct {
    offset: u32 = 0,
    resv: u32 = 0,
    data: u64 align(8) = 0,
    tags: u64 align(8) = 0,
    nr: u32 = 0,
    resv2: u32 = 0,
};

pub const ProbeOp = extern struct {
    op: u8 = 0,
    resv: u8 = 0,
    flags: u16 = 0, // Op* flags
    resv2: u32 = 0,
};

pub const Probe = extern struct {
    last_op: u8 align(4) = 0, // last opcode supported
    ops_len: u8 = 0, // length of ops[] array below
    resv: u16 = 0,
    resv2: [3]u32 = .{ 0, 0, 0 },
    // ops: [256]ProbeOp = .{.{}} ** 256,
};

/// Restriction.opcode
pub const RestrictionOpcode = enum(u16) {
    /// allow an io_uring_register(2) opcode
    register_op = 0,
    /// allow an sqe opcode
    sqe_op = 1,
    /// allow sqe flags
    sqe_flags_allowed = 2,
    /// require sqe flags (these flags must be set on each submission)
    sqe_flags_required = 3,
    last = 4,
};

/// Restriction.flags
pub const RestrictionFlags = extern union {
    register_op: u8, // RestrictionRegisterOp
    sqe_op: u8, // RestrictionSqeOp
    sqe_flags: u8, // RestrictionSqeFlags*
};

pub const Restriction = extern struct {
    opcode: RestrictionOpcode = mem.zeroes(RestrictionOpcode),
    flags: RestrictionFlags = mem.zeroes(RestrictionFlags),
    resv: u8 = 0,
    resv2: [3]u32 = .{ 0, 0, 0 },
};

pub const Buf = extern struct {
    addr: u64 = 0,
    len: u32 = 0,
    bid: u16 = 0,
    resv: u16 = 0,
};

pub const GeteventsArg = extern struct {
    sigmask: u64 = 0,
    sigmask_sz: u32 = 0,
    pad: u32 = 0,
    ts: u64 = 0,
};

pub const __kernel_timespec = extern struct {
    tv_sec: c_longlong = 0, // seconds
    tv_nsec: c_longlong = 0, // nanoseconds
};

/// argument for register_sync_cancel
pub const SyncCancelReg = extern struct {
    addr: u64 = 0,
    fd: i32 = 0,
    flags: u32 = 0,
    timeout: __kernel_timespec = .{},
    opcode: u8 = 0,
    pad: [7]u8 = .{ 0, 0, 0, 0, 0, 0, 0 },
    pad2: [3]u64 = .{ 0, 0, 0 },
};

/// argument for register_file_alloc_range
/// the range is specified as [off, off + len)
pub const FileIndexRange = extern struct {
    off: u32 = 0,
    len: u32 = 0,
    resv: u64 = 0,
};

pub const RecvmsgOut = extern struct {
    namelen: u32 = 0,
    controllen: u32 = 0,
    payloadlen: u32 = 0,
    flags: u32 = 0,
};

pub const BufRingUnion = extern union {
    /// to avoid spilling into more pages than we need to, the
    /// ring tail is overlaid with the io_uring_buf->resv field.
    __: extern struct {
        resv1: u64,
        resv2: u32,
        resv3: u16,
        tail: u16,
    },
    bufs: [0]Buf,
};
pub const BufRing = extern struct {
    _: BufRingUnion = mem.zeroes(BufRingUnion),
};

/// argument for (un)register_pbuf_ring
pub const BufReg = extern struct {
    ring_addr: u64 = 0,
    ring_entries: u32 = 0,
    bgid: u16 = 0,
    flags: u16 = 0,
    resv: [3]u64 = .{ 0, 0, 0 },
};

/// argument for register_pbuf_status
pub const BufStatus = extern struct {
    buf_group: u32 = 0, // input
    head: u32 = 0, // output
    resv: [8]u32 = .{ 0, 0, 0, 0, 0, 0, 0, 0 },
};

/// argument for (un)register_napi
pub const Napi = extern struct {
    busy_poll_to: u32 = 0,
    prefer_busy_poll: u8 = 0,
    pad: [3]u8 = .{ 0, 0, 0 },
    resv: u64 = 0,
};

/// async_cancel flags.
///
/// all      cancel all requests that match the given key
/// fd      key off 'fd' for cancelation rather than the request 'user_data'
/// any      match any request
/// fd_fixed 'fd' passed in is a fixed descriptor
/// userdata match on user_data, default for no other key
/// op      match  request based on opcode
pub const AsyncCancel = packed struct {
    all: bool = false,
    fd: bool = false,
    any: bool = false,
    fd_fixed: bool = false,
    userdata: bool = false,
    op: bool = false,
    _: u58 = 0,
};

/// SqringOffsets.flags
pub const SqringOffsetsFlags = packed struct {
    need_wakeup: bool = false, // needs io_uring_enter wakeup
    cq_overflow: bool = false, // cq ring is overflown
    taskrun: bool = false, // task should enter the kernel
    _: u29 = 0,
};

/// filled with the offset for mmap(2)
pub const SqringOffsets = extern struct {
    head: u32 = 0,
    tail: u32 = 0,
    ring_mask: u32 = 0,
    ring_entries: u32 = 0,
    flags: SqringOffsetsFlags = .{},
    dropped: u32 = 0,
    array: u32 = 0,
    resv1: u32 = 0,
    user_addr: u64 = 0,
};

/// CqringOffsets.flags
pub const CqringOffsetsFlags = packed struct {
    /// disable eventfd notifications
    eventfd_disabled: bool = false,
    _: u31 = 0,
};

/// filled with the offset for mmap(2)
pub const CqringOffsets = extern struct {
    head: u32 = 0,
    tail: u32 = 0,
    ring_mask: u32 = 0,
    ring_entries: u32 = 0,
    overflow: u32 = 0,
    cqes: u32 = 0,
    flags: CqringOffsetsFlags = .{},
    resv1: u32 = 0,
    user_addr: u64 = 0,
};

/// Params.features
pub const ParamsFeatures = packed struct {
    single_mmap: bool = false,
    nodrop: bool = false,
    submit_stable: bool = false,
    rw_cur_pos: bool = false,
    cur_personality: bool = false,
    fast_poll: bool = false,
    poll_32bits: bool = false,
    sqpoll_nonfixed: bool = false,
    ext_arg: bool = false,
    native_workers: bool = false,
    rsrc_tags: bool = false,
    cqe_skip: bool = false,
    linked_file: bool = false,
    reg_reg_ring: bool = false,
    _: u18 = 0,
};

/// passed in for io_uring_setup(2). copied back with updated info on success
pub const Params = extern struct {
    sq_entries: u32 = 0,
    cq_entries: u32 = 0,
    flags: SetupFlags = .{},
    sq_thread_cpu: u32 = 0,
    sq_thread_idle: u32 = 0,
    features: ParamsFeatures = .{},
    wq_fd: u32 = 0,
    resv: [3]u32 = .{ 0, 0, 0 },
    sq_off: SqringOffsets = .{},
    cq_off: CqringOffsets = .{},
};

/// io_uring_enter(2) flags
pub const EnterFlags = packed struct {
    getevents: bool = false,
    sq_wakeup: bool = false,
    sq_wait: bool = false,
    ext_arg: bool = false,
    registered_ring: bool = false,
    _: u27 = 0,
};

/// Sqe.flags
pub const SqeFlags = packed struct {
    /// use fixed fileset
    fixed_file: bool = false,
    /// issue after inflight io
    io_drain: bool = false,
    /// links next sqe
    io_link: bool = false,
    /// like link, but stronger
    io_hardlink: bool = false,
    /// always go async
    io_async: bool = false,
    /// select buffer from sqe->buf_group
    buffer_select: bool = false,
    /// don't post cqe if request succeeded
    cqe_skip_sucess: bool = false,
    _: u1 = 0,
};

/// send/sendmsg and recv/recvmsg flags (sqe->ioprio)
///
/// recvsend_poll_first if set, instead of first attempting to send
///    or receive and arm poll if that yields an
///    -eagain result, arm poll upfront and skip
///    the initial transfer attempt.
///
/// recv_multishot multishot recv. sets cqe_f_more if
///    the handler will continue to report
///    cqes on behalf of the same sqe.
///
/// recvsend_fixed_buf use registered buffers, the index is stored in
///    the buf_index field.
///
/// send_zc_report_usage
///    if set, send[msg]_zc should report
///    the zerocopy usage in cqe.res
///    for the cqe_f_notif cqe.
///    0 is reported if zerocopy was actually possible.
///    notif_usage_zc_copied if data was copied
///    (at least partially).
/// Sqe.ioprio
pub const SqeIoprio = packed struct {
    /// also known as accept_multishot
    recvsend_poll_first: bool = false,
    rescv_mutlishot: bool = false,
    recvsend_fixed_buf: bool = false,
    send_zc_report_usage: bool = false,
    _: u12 = 0,
};

/// Sqe.off.op
pub const SqeOffAddrCmd = extern struct {
    cmd_op: u32 = 0,
    _pad: u32 = 0,
};

/// Sqe.off
pub const SqeOff = extern union {
    off: u64, // offset into file
    addr2: u64,
    op: SqeOffAddrCmd,
};

/// op_msg_ring command types
/// Sqe.addr.addr
pub const SqeAddrEnum = enum(u64) {
    msg_data, // pass sqe->len as 'res' and off as user_data
    msg_send_fd, // send a registered fd to another ring
};

/// Sqe.addr.opt
pub const SqeAddrOpt = extern struct {
    level: u32 = 0,
    optname: u32 = 0,
};

/// Sqe.addr
pub const SqeAddr = extern union {
    addr: u64, // pointer to buffer or iovecs
    splice_off_in: u64,
    opt: SqeAddrOpt,
};

/// Sqe.op_flags.fsync_flags
pub const SqeFsyncFlags = packed struct {
    datasync: bool = false,
    _: u31 = 0,
};

/// poll_add flags. the command flags for poll_add are stored in sqe->len.
///
/// add_multi multishot poll. sets cqe_f_more if
///    the poll handler will continue to report
///    cqes on behalf of the same sqe.
///
/// update  update existing poll request, matching
///    sqe->addr as the old user_data field.
///
/// level  level triggered poll.
/// Sqe.op_flags.poll_events
pub const SqePollEvents = packed struct {
    add_mutli: bool = false,
    update_events: bool = false,
    update_user_data: bool = false,
    add_level: bool = false,
    _: u12 = 0,
};

/// timeout_clock_mask  = timeout_boottime | timeout_realtime.
/// timeout_update_mask = timeout_update | link_timeout_update.
/// Sqe.op_flags.timeout_flags
pub const SqeTimeoutFlags = packed struct {
    abs: bool = false,
    update: bool = false,
    boottime: bool = false,
    realtime: bool = false,
    link_update: bool = false,
    etime_success: bool = false,
    multishot: bool = false,
    _: u25 = 0,
};

/// extends splice(2) flags.
/// Sqe.op_flags.splice_flags
pub const SqeSpliceFlags = packed struct {
    _: u31 = 0,
    fd_in_fixed: bool = false,
};

/// op_msg_ring flags (sqe->msg_ring_flags)
///
/// msg_ring_cqe_skip don't post a cqe to the target ring. not applicable for msg_data, obviously.
/// Sqe.op_flags.msg_ring_flags
pub const SqeMsgRingFlags = packed struct {
    cqe_skip: bool = false,
    /// pass through the flags from sqe->file_index to cqe->flags
    flags_pass: bool = false,
    _: u30 = 0,
};

/// top 8bits aren't available for userspace
/// uring_cmd_fixed use registered buffer; pass this flag
/// along with setting sqe->buf_index.
pub const SqeUringCmdFlags = packed struct {
    /// uring_cmd_fixed = uring_cmd_mask
    fixed: bool = false,
    _: u31 = 0,
};

/// op_fixed_fd_install flags (sqe->install_fd_flags)
///
/// fixed_fd_no_cloexec don't mark the fd as o_cloexec.
/// Sqe.op_flags.uring_cmd_flags
pub const SqeInstallFdFlags = packed struct {
    no_cloexec: bool = false,
    _: u31 = 0,
};

/// Sqe.op_flags
pub const SqeOpFlags = extern union {
    rw_flags: c_int,
    fsync_flags: SqeFsyncFlags,
    poll_events: SqePollEvents, // compatibility
    poll32_events: u32, // word-reversed for be
    sync_range_flags: u32,
    msg_flags: u32,
    timeout_flags: SqeTimeoutFlags,
    accept_flags: u32,
    cancel_flags: u32,
    open_flags: u32,
    statx_flags: u32,
    fadvise_advice: u32,
    splice_flags: SqeSpliceFlags,
    rename_flags: u32,
    unlink_flags: u32,
    hardlink_flags: u32,
    xattr_flags: u32,
    msg_ring_flags: SqeMsgRingFlags,
    uring_cmd_flags: SqeUringCmdFlags,
    waitid_flags: u32,
    futex_flags: u32,
    install_fd_flags: SqeInstallFdFlags,
    nop_flags: u32,
};

/// pack this to avoid bogus arm oabi complaints. Sqe.buf_group
/// Sqe.buf
pub const SqeBuf = packed union {
    /// index into fixed buffers, if used
    buf_index: u16 align(1),
    /// for grouped buffer selection
    buf_group: u16 align(1),
};

/// Sqe.add.addr_len
pub const SqeAdditionalAddrLen = extern struct {
    addr_len: u16 = 0,
    __pad: [1]u16 = .{0},
};

/// Sqe.add
pub const SqeAdd = extern union {
    splice_fd_in: i32,
    file_index: u32,
    optlen: u32,
    addr_len: SqeAdditionalAddrLen,
};

/// Sqe.extra.addr
pub const SqeOptvalAddr = extern struct {
    addr3: u64 = 0,
    __pad: [1]u64 = .{0},
};

/// Sqe.extra
pub const SqeExtra = extern union {
    addr: SqeOptvalAddr,
    optval: u64,
    /// if the ring is initialized with setup_sqe128, then
    /// this field is used for 80 bytes of arbitrary command data
    cmd: [0]u8,
};

/// io submission data extern structure (submission queue entry)
pub const Sqe = extern struct {
    opcode: Op = mem.zeroes(Op), // type of operation for this sqe
    flags: SqeFlags = .{}, // iosqe_ flags
    ioprio: SqeIoprio = .{}, // ioprio for the request
    fd: i32 = 0, // file descriptor to do io on
    off: SqeOff = mem.zeroes(SqeOff),
    addr: SqeAddr = mem.zeroes(SqeAddr),
    len: u32 = 0, // buffer size or number of iovecs
    op_flags: SqeOpFlags = mem.zeroes(SqeOpFlags),
    user_data: u64 = 0, // data to be passed back at completion time
    /// pack this to avoid bogus arm oabi complaints
    buf: SqeBuf = mem.zeroes(SqeBuf),
    /// personality to use, if used
    personality: u16 = 0,
    add: SqeAdd = mem.zeroes(SqeAdd),
    extra: SqeExtra = mem.zeroes(SqeExtra),
};

/// cqe.res for cqe_f_notif if
/// send_zc_report_usage was requested
///
/// it should be treated as a flag, all other
/// bits of cqe.res should be treated as reserved!
pub const NotifUsageZcCopied = packed struct {
    _: i31 = 0,
    usage_zc_copied: bool = false,
};

/// Cqe.flags
pub const CqeFlags = packed struct {
    buffer: bool = false,
    more: bool = false,
    sock_nonempty: bool = false,
    notif: bool = false,
    _: u28 = 0,
};

/// io completion data extern structure (completion queue entry)
pub const Cqe = extern struct {
    user_data: u64 align(8) = 0, // sqe->data submission passed back
    res: i32 = 0, // result code for this event
    /// cqe_f_buffer     if set, the upper 16 bits are the buffer id
    /// cqe_f_more         if set, parent sqe will generate more cqe entries
    /// cqe_f_sock_nonempty if set, more data to read after socket recv
    /// cqe_f_notif         set for notification cqes. can be used to distinct them from sends.
    flags: CqeFlags = .{},
};

/// io completion data extern structure (completion queue entry).
/// Use this structure instead of Cqe if you have initialized the
/// ring with IORING_SETUP_CQE32.
pub const Cqe32 = extern struct {
    user_data: u64 align(8) = 0, // sqe->data submission passed back
    res: i32 = 0, // result code for this event
    /// cqe_f_buffer     if set, the upper 16 bits are the buffer id
    /// cqe_f_more         if set, parent sqe will generate more cqe entries
    /// cqe_f_sock_nonempty if set, more data to read after socket recv
    /// cqe_f_notif         set for notification cqes. can be used to distinct them from sends.
    flags: CqeFlags = .{},
    padding: [2]u64 = .{ 0, 0 },
};

/// io_uring_register(2) opcodes and arguments
pub const Reg = enum(c_uint) {
    register_buffers = 0,
    unregister_buffers = 1,
    register_files = 2,
    unregister_files = 3,
    register_eventfd = 4,
    unregister_eventfd = 5,
    register_files_update = 6,
    register_eventfd_async = 7,
    register_probe = 8,
    register_personality = 9,
    unregister_personality = 10,
    register_restrictions = 11,
    register_enable_rings = 12,

    /// extended with tagging
    register_files2 = 13,
    register_files_update2 = 14,
    register_buffers2 = 15,
    register_buffers_update = 16,

    /// set/clear io-wq thread affinities
    register_iowq_aff = 17,
    unregister_iowq_aff = 18,

    /// set/get max number of io-wq workers
    register_iowq_max_workers = 19,

    /// register/unregister io_uring fd with the ring
    register_ring_fds = 20,
    unregister_ring_fds = 21,

    /// register ring based provide buffer group
    register_pbuf_ring = 22,
    unregister_pbuf_ring = 23,

    /// sync cancelation api
    register_sync_cancel = 24,

    /// register a range of fixed file slots for automatic slot allocation
    register_file_alloc_range = 25,

    /// return status information for a buffer group
    register_pbuf_status = 26,

    /// set/clear busy poll settings
    register_napi = 27,
    unregister_napi = 28,

    /// this goes last
    register_last,

    /// flag added to the opcode to use a registered ring fd
    register_use_registered_ring = @as(c_uint, 1) << 31,
};

pub const Op = enum(u8) {
    nop,
    readv,
    writev,
    fsync,
    read_fixed,
    write_fixed,
    poll_add,
    poll_remove,
    sync_file_range,
    sendmsg,
    recvmsg,
    timeout,
    timeout_remove,
    accept,
    async_cancel,
    link_timeout,
    connect,
    fallocate,
    openat,
    close,
    files_update,
    statx,
    read,
    write,
    fadvise,
    madvise,
    send,
    recv,
    openat2,
    epoll_ctl,
    splice,
    provide_buffers,
    remove_buffers,
    tee,
    shutdown,
    renameat,
    unlinkat,
    mkdirat,
    symlinkat,
    linkat,
    msg_ring,
    fsetxattr,
    setxattr,
    fgetxattr,
    getxattr,
    socket,
    uring_cmd,
    send_zc,
    sendmsg_zc,
    read_multishot,
    waitid,
    futex_wait,
    futex_wake,
    futex_waitv,
    fixed_fd_install,
    ftruncate,

    /// this goes last, obviously
    last,
};

/// io_uring_setup() flags
pub const SetupFlags = packed struct {
    iopoll: bool = false,
    sqpoll: bool = false,
    sq_aff: bool = false,
    cqsize: bool = false,
    clamp: bool = false,
    attach_wq: bool = false,
    r_disabled: bool = false,
    submit_all: bool = false,
    /// cooperative task running. when requests complete, they often require
    /// forcing the submitter to transition to the kernel to complete. if this
    /// flag is set, work will be done when the task transitions anyway, rather
    /// than force an inter-processor interrupt reschedule. this avoids interrupting
    /// a task running in userspace, and saves an ipi.
    coop_taskrun: bool = false,
    /// if coop_taskrun is set, get notified if task work is available for
    /// running and a kernel transition would be needed to run it. this sets
    /// sq_taskrun in the sq ring flags. not valid with coop_taskrun.
    taskrun_flag: bool = false,
    sqe128: bool = false,
    cqe32: bool = false,
    /// only one task is allowed to submit requests
    single_issuer: bool = false,
    /// defer running task work to get events.
    /// rather than running bits of task work whenever the task transitions
    /// try to do it just before it is needed.
    defer_taskrun: bool = false,
    /// application provides the memory for the rings
    no_mmap: bool = false,
    /// register the ring fd in itself for use with
    /// register_use_registered_ring; return a registered fd index rather
    /// than an fd.
    registered_fd_only: bool = false,
    /// removes indirection through the sq index array
    no_sqarray: bool = false,
    _: u15 = 0,
};
