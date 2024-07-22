/// if sqe->file_index is set to this for opcodes that instantiate a new
/// direct descriptor (like openat/openat2/accept), then io_uring will allocate
/// an available direct descriptor instead of having the application pass one
/// in. the picked direct descriptor will be returned in cqe->res, or -enfile
/// if the space is full.
pub const file_index_alloc: c_int = ~@as(c_uint, 0);

pub const iosqe_fixed_file_bit: c_int = 0;
pub const iosqe_io_drain_bit: c_int = 1;
pub const iosqe_io_link_bit: c_int = 2;
pub const iosqe_io_hardlink_bit: c_int = 3;
pub const iosqe_async_bit: c_int = 4;
pub const iosqe_buffer_select_bit: c_int = 5;
pub const iosqe_cqe_skip_success_bit: c_int = 6;

pub const ioring_cqe_buffer_shift = 16;

/// magic offsets for the application to mmap the data it needs
pub const ioring_off_sq_ring: c_ulonglong = 0;
pub const ioring_off_cq_ring: c_ulonglong = 0x8000000;
pub const ioring_off_sqes: c_ulonglong = 0x10000000;
pub const ioring_off_pbuf_ring: c_ulonglong = 0x80000000;
pub const ioring_off_pbuf_shift: c_int = 16;
pub const ioring_off_mmap_mask: c_ulonglong = 0xf8000000;

/// io-wq worker categories
pub const constio_wq_bound: c_int = 0;
pub const io_wq_unbound: c_int = 1;

/// skip updating fd indexes set to this value in the fd table
pub const ioring_register_files_skip: c_int = -2;

pub const io_uring_op_supported: c_int = 1;

/// io_uring_restriction->opcode values.
/// allow an io_uring_register(2) opcode
pub const ioring_restriction_register_op: c_int = 0;
/// allow an sqe opcode
pub const ioring_restriction_sqe_op: c_int = 1;
/// allow sqe flags
pub const ioring_restriction_sqe_flags_allowed: c_int = 2;
/// require sqe flags (these flags must be set on each submission)
pub const ioring_restriction_sqe_flags_required: c_int = 3;
pub const ioring_restriction_last: c_int = 4;

/// argument for ioring_op_uring_cmd when file is a socket
pub const socket_uring_op_siocinq: c_int = 0;
pub const socket_uring_op_siocoutq: c_int = 1;
pub const socket_uring_op_getsockopt: c_int = 2;
pub const socket_uring_op_setsockopt: c_int = 3;

/// flags for ioring_register_pbuf_ring.
///
/// iou_pbuf_ring_mmap:	if set, kernel will allocate the memory for the ring.
///			the application must not set a ring_addr in extern struct
///			io_uring_buf_reg, instead it must subsequently call
///			mmap(2) with the offset set as:
///			ioring_off_pbuf_ring | (bgid << ioring_off_pbuf_shift)
///			to get a virtual mapping for the ring.
pub const iou_pbuf_ring_mmap: c_int = 1;

pub const ioring_rsrc_register = packed struct {
    sparse: bool = false,
    _: u63 = 0,
};

pub const io_uring_rsrc_register = extern struct {
    nr: u32,
    flags: u32,
    resv2: u64,
    data: u64 align(8),
    tags: u64 align(8),
};

pub const io_uring_rsrc_update = extern struct {
    offset: u32,
    resv: u32,
    data: u64 align(8),
};

pub const io_uring_rsrc_update2 = extern struct {
    offset: u32,
    resv: u32,
    data: u64 align(8),
    tags: u64 align(8),
    nr: u32,
    resv2: u32,
};

pub const io_uring_probe_op = extern struct {
    op: u8,
    resv: u8,
    flags: u16, // io_uring_op_* flags
    resv2: u32,
};

pub const io_uring_probe = extern struct {
    last_op: u8, // last opcode supported
    ops_len: u8, // length of ops[] array below
    resv: u16,
    resv2: [3]u32,
    ops: []io_uring_probe_op,
};

pub const io_uring_restriction = extern struct {
    opcode: u16,
    _: extern union {
        register_op: u8, // ioring_restriction_register_op
        sqe_op: u8, // ioring_restriction_sqe_op
        sqe_flags: u8, // ioring_restriction_sqe_flags_*
    },
    resv: u8,
    resv2: [3]u32,
};

pub const io_uring_buf = extern struct {
    addr: u64,
    len: u32,
    bid: u16,
    resv: u16,
};

pub const io_uring_getevents_arg = extern struct {
    sigmask: u64,
    sigmask_sz: u32,
    pad: u32,
    ts: u64,
};

pub const __kernel_timespec = extern struct {
    tv_sec: c_longlong, // seconds
    tv_nsec: c_longlong, // nanoseconds
};

/// argument for ioring_register_sync_cancel
pub const io_uring_sync_cancel_reg = extern struct {
    addr: u64,
    fd: i32,
    flags: u32,
    timeout: __kernel_timespec,
    opcode: u8,
    pad: [7]u8,
    pad2: [3]u64,
};

/// argument for ioring_register_file_alloc_range
/// the range is specified as [off, off + len)
pub const io_uring_file_index_range = extern struct {
    off: u32,
    len: u32,
    resv: u64,
};

pub const io_uring_recvmsg_out = extern struct {
    namelen: u32,
    controllen: u32,
    payloadlen: u32,
    flags: u32,
};

pub const io_uring_buf_ring = extern struct {
    _: extern union {
        /// to avoid spilling into more pages than we need to, the
        /// ring tail is overlaid with the io_uring_buf->resv field.
        __: extern struct {
            resv1: u64,
            resv2: u32,
            resv3: u16,
            tail: u16,
        },
        bufs: [0]io_uring_buf,
    },
};

/// argument for ioring_(un)register_pbuf_ring
pub const io_uring_buf_reg = extern struct {
    ring_addr: u64,
    ring_entries: 32,
    bgid: u16,
    flags: 16,
    resv: [3]u64,
};

/// argument for ioring_register_pbuf_status
pub const io_uring_buf_status = extern struct {
    buf_group: u32, // input
    head: u32, // output
    resv: [8]u32,
};

/// argument for ioring_(un)register_napi
pub const io_uring_napi = extern struct {
    busy_poll_to: u32,
    prefer_busy_poll: u8,
    pad: [3]u8,
    resv: u64,
};

/// async_cancel flags.
///
/// ioring_async_cancel_all	cancel all requests that match the given key
/// ioring_async_cancel_fd	key off 'fd' for cancelation rather than the
///				request 'user_data'
/// ioring_async_cancel_any	match any request
/// ioring_async_cancel_fd_fixed	'fd' passed in is a fixed descriptor
/// ioring_async_cancel_userdata	match on user_data, default for no other key
/// ioring_async_cancel_op	match request based on opcode
pub const io_uring_async_cancel = packed struct {
    all: bool = false,
    fd: bool = false,
    any: bool = false,
    fd_fixed: bool = false,
    userdata: bool = false,
    op: bool = false,
    _: u58,
};

/// filled with the offset for mmap(2)
pub const io_sqring_offsets = extern struct {
    head: u32,
    tail: u32,
    ring_mask: u32,
    ring_entries: u32,
    flags: packed struct {
        need_wakeup: bool = false, // needs io_uring_enter wakeup
        cq_overflow: bool = false, // cq ring is overflown
        taskrun: bool = false, // task should enter the kernel
        _: u29 = 0,
    },
    dropped: u32,
    array: u32,
    resv1: u32,
    user_addr: u64,
};

pub const io_cqring_offsets = extern struct {
    head: u32,
    tail: u32,
    ring_mask: u32,
    ring_entries: u32,
    overflow: u32,
    cqes: u32,
    flags: packed struct {
        /// disable eventfd notifications
        eventfd_disabled: bool = false,
        _: u31 = 0,
    },
    resv1: u32,
    user_addr: u64,
};

/// passed in for io_uring_setup(2). copied back with updated info on success
pub const io_uring_params = extern struct {
    sq_entries: u32,
    cq_entries: u32,
    flags: io_uring_setup,
    sq_thread_cpu: u32,
    sq_thread_idle: u32,
    features: packed struct {
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
    },
    wq_fd: u32,
    resv: [3]u32,
    sq_off: io_sqring_offsets,
    cq_off: io_cqring_offsets,
};

/// io_uring_enter(2) flags
pub const io_uring_enter_flags = extern struct {
    getevents: bool = false,
    sq_wakeup: bool = false,
    sq_wait: bool = false,
    ext_arg: bool = false,
    registered_ring: bool = false,
    _: u27 = 0,
};

/// io submission data extern structure (submission queue entry)
pub const io_uring_sqe = extern struct {
    opcode: u8, // type of operation for this sqe
    flags: packed struct {
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
        _: bool = false,
    }, // iosqe_ flags
    /// send/sendmsg and recv/recvmsg flags (sqe->ioprio)
    ///
    /// ioring_recvsend_poll_first	if set, instead of first attempting to send
    ///				or receive and arm poll if that yields an
    ///				-eagain result, arm poll upfront and skip
    ///				the initial transfer attempt.
    ///
    /// ioring_recv_multishot	multishot recv. sets ioring_cqe_f_more if
    ///				the handler will continue to report
    ///				cqes on behalf of the same sqe.
    ///
    /// ioring_recvsend_fixed_buf	use registered buffers, the index is stored in
    ///				the buf_index field.
    ///
    /// ioring_send_zc_report_usage
    ///				if set, send[msg]_zc should report
    ///				the zerocopy usage in cqe.res
    ///				for the ioring_cqe_f_notif cqe.
    ///				0 is reported if zerocopy was actually possible.
    ///				ioring_notif_usage_zc_copied if data was copied
    ///				(at least partially).
    ioprio: packed struct {
        /// also known as accept_multishot
        recvsend_poll_first: bool = false,
        rescv_mutlishot: bool = false,
        recvsend_fixed_buf: bool = false,
        send_zc_report_usage: bool = false,
        _: u60 = 0,
    }, // ioprio for the request
    fd: i32, // file descriptor to do io on
    _: extern union {
        off: u64, // offset into file
        addr2: u64,
        __: extern struct {
            cmd_op: u32,
            __pad1: u32,
        },
    },
    _1: extern union {
        /// ioring_op_msg_ring command types
        addr: enum(u64) {
            msg_data, // pass sqe->len as 'res' and off as user_data
            msg_send_fd, // send a registered fd to another ring
        }, // pointer to buffer or iovecs
        splice_off_in: u64,
        __1: extern struct {
            level: u32,
            optname: u32,
        },
    },
    len: u32, // buffer size or number of iovecs
    _2: extern union {
        rw_flags: c_int,
        fsync_flags: packed struct {
            datasync: bool = false,
            _: u31 = 0,
        },
        /// poll_add flags. the command flags for poll_add are stored in sqe->len.
        ///
        /// ioring_poll_add_multi	multishot poll. sets ioring_cqe_f_more if
        ///				the poll handler will continue to report
        ///				cqes on behalf of the same sqe.
        ///
        /// ioring_poll_update		update existing poll request, matching
        ///				sqe->addr as the old user_data field.
        ///
        /// ioring_poll_level		level triggered poll.
        poll_events: packed struct {
            add_mutli: bool = false,
            update_events: bool = false,
            update_user_data: bool = false,
            add_level: bool = false,
            _: u12 = 0,
        }, // compatibility
        poll32_events: u32, // word-reversed for be
        sync_range_flags: u32,
        msg_flags: u32,
        /// ioring_timeout_clock_mask = ioring_timeout_boottime | ioring_timeout_realtime
        /// ioring_timeout_update_mask = ioring_timeout_update | ioring_link_timeout_update
        timeout_flags: packed struct {
            abs: bool = false,
            update: bool = false,
            boottime: bool = false,
            realtime: bool = false,
            link_update: bool = false,
            etime_success: bool = false,
            multishot: bool = false,
            _: u25 = 0,
        },
        accept_flags: u32,
        cancel_flags: u32,
        open_flags: u32,
        statx_flags: u32,
        fadvise_advice: u32,
        /// extends splice(2) flags
        splice_flags: packed struct {
            _: u31 = 0,
            fd_in_fixed: bool = false,
        },
        rename_flags: u32,
        unlink_flags: u32,
        hardlink_flags: u32,
        xattr_flags: u32,
        /// ioring_op_msg_ring flags (sqe->msg_ring_flags)
        ///
        /// ioring_msg_ring_cqe_skip	don't post a cqe to the target ring. not
        ///				applicable for ioring_msg_data, obviously.
        msg_ring_flags: packed struct {
            cqe_skip: bool = false,
            /// pass through the flags from sqe->file_index to cqe->flags
            flags_pass: bool = false,
            _: u30 = 0,
        },
        /// top 8bits aren't available for userspace
        /// ioring_uring_cmd_fixed	use registered buffer; pass this flag
        /// along with setting sqe->buf_index.
        uring_cmd_flags: packed struct {
            /// ioring_uring_cmd_fixed = ioring_uring_cmd_mask
            fixed: bool = false,
        },
        waitid_flags: u32,
        futex_flags: u32,
        /// ioring_op_fixed_fd_install flags (sqe->install_fd_flags)
        ///
        /// ioring_fixed_fd_no_cloexec	don't mark the fd as o_cloexec
        install_fd_flags: packed struct {
            no_cloexec: bool = false,
            _: u31 = 0,
        },
        nop_flags: u32,
    },
    user_data: u64, // data to be passed back at completion time
    /// pack this to avoid bogus arm oabi complaints
    _3: packed union {
        /// index into fixed buffers, if used
        buf_index: u16,
        /// for grouped buffer selection
        buf_group: u16,
    },
    /// personality to use, if used
    personality: u16,
    _4: extern union {
        splice_fd_in: i32,
        file_index: u32,
        optlen: u32,
        __2: extern struct {
            addr_len: u16,
            __pad3: [1]u16,
        },
    },
    _5: extern union {
        __3: extern struct {
            addr3: u64,
            __pad2: [1]u64,
        },
        optval: u64,
        /// if the ring is initialized with ioring_setup_sqe128, then
        /// this field is used for 80 bytes of arbitrary command data
        cmd: [0]u8,
    },
};

/// cqe.res for ioring_cqe_f_notif if
/// ioring_send_zc_report_usage was requested
///
/// it should be treated as a flag, all other
/// bits of cqe.res should be treated as reserved!
pub const ioring_notif_usage_zc_copied = packed struct {
    _: i31 = 0,
    usage_zc_copied: bool = false,
};

/// io completion data extern structure (completion queue entry)
pub const io_uring_cqe = extern struct {
    user_data: u64, // sqe->data submission passed back
    res: i32, // result code for this event
    /// ioring_cqe_f_buffer	if set, the upper 16 bits are the buffer id
    /// ioring_cqe_f_more	if set, parent sqe will generate more cqe entries
    /// ioring_cqe_f_sock_nonempty	if set, more data to read after socket recv
    /// ioring_cqe_f_notif	set for notification cqes. can be used to distinct
    /// 			them from sends.
    flags: packed struct {
        buffer: bool = false,
        more: bool = false,
        sock_nonempty: bool = false,
        notif: bool = false,
        _: u28 = 0,
    },
    /// if the ring is initialized with ioring_setup_cqe32, then this field
    /// contains 16-bytes of padding, doubling the size of the cqe.
    big_cqe: []u64,
};

/// io_uring_register(2) opcodes and arguments
pub const io_uring_reg = enum(c_uint) {
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

pub const io_uring_op = enum(c_int) {
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
pub const io_uring_setup = packed struct {
    iopoll: bool = false, // 1u << 0
    sqpoll: bool = false, // 1u << 1
    sq_aff: bool = false, // 1u << 2
    cqsize: bool = false, // 1u << 3
    clamp: bool = false, // 1u << 4
    attach_wq: bool = false, // 1u << 5
    r_disabled: bool = false, // 1u << 6
    submit_all: bool = false, // 1u << 7
    /// cooperative task running. when requests complete, they often require
    /// forcing the submitter to transition to the kernel to complete. if this
    /// flag is set, work will be done when the task transitions anyway, rather
    /// than force an inter-processor interrupt reschedule. this avoids interrupting
    /// a task running in userspace, and saves an ipi.
    coop_taskrun: bool = false, // 1u << 8
    /// if coop_taskrun is set, get notified if task work is available for
    /// running and a kernel transition would be needed to run it. this sets
    /// ioring_sq_taskrun in the sq ring flags. not valid with coop_taskrun.
    taskrun_flag: bool = false, // 1u << 9
    sqe128: bool = false, // 1u << 10
    cqe32: bool = false, // 1u << 11
    /// only one task is allowed to submit requests
    single_issuer: bool = false, // 1u << 12
    /// defer running task work to get events.
    /// rather than running bits of task work whenever the task transitions
    /// try to do it just before it is needed.
    defer_taskrun: bool = false, // 1u << 13
    /// application provides the memory for the rings
    no_mmap: bool = false, // 1u << 14
    /// register the ring fd in itself for use with
    /// ioring_register_use_registered_ring; return a registered fd index rather
    /// than an fd.
    registered_fd_only: bool = false, // 1u << 15
    /// removes indirection through the sq index array
    no_sqarray: bool = false, // 1u << 16,
    _: u15 = 0,
};
