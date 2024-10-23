#pragma once

#include "../diagnostics.hpp"
#include "../math.hpp"
#include "../mem/utils.hpp"
#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "syscall/syscall.hpp"
#include <asm-generic/mman-common.h>
#include <asm/unistd_64.h>
#include <linux/io_uring.h>
#include <linux/mman.h>
#include <linux/signal.h>

inline u32* off_cast(u8* mem, u32 idx) {
    return reinterpret_cast<u32*>(&mem [idx]);
}

DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast", "-Wcast-align")

struct SubmissionQueue {
    u32 mask{0};
    Slice<u32> array{};
    u32* head{null<u32>()};
    u32* tail{null<u32>()};
    u32* flags{null<u32>()};
    u32* dropped{null<u32>()};

    u32 sqe_head = 0;
    u32 sqe_tail = 0;

    Slice<u8> mem{};
    Slice<io_uring_sqe> sqes{};

    SubmissionQueue() {}
    SubmissionQueue(
        u32 _mask, Slice<u32> _array, u32* _head, u32* _tail, u32* _flags, u32* _dropped,
        u32 _sqe_head, u32 _sqe_tail, Slice<u8> _mem, Slice<io_uring_sqe> _sqes
    )
        : mask(_mask), array(move(_array)), head(_head), tail(_tail), flags(_flags),
          dropped(_dropped), sqe_head(_sqe_head), sqe_tail(_sqe_tail), mem(move(_mem)),
          sqes(move(_sqes)) {}

    SubmissionQueue(const SubmissionQueue& t)            = delete;
    SubmissionQueue& operator=(const SubmissionQueue& t) = delete;
    SubmissionQueue(SubmissionQueue&& s) noexcept
        : mask(exchange(s.mask, U32_0)), array(move(s.array)), head(exchange(s.head, null<u32>())),
          tail(exchange(s.tail, null<u32>())), flags(exchange(s.flags, null<u32>())),
          dropped(exchange(s.dropped, null<u32>())), sqe_head(exchange(s.sqe_head, U32_0)),
          sqe_tail(exchange(s.sqe_tail, U32_0)), mem(move(s.mem)), sqes(move(s.sqes)) {}
    SubmissionQueue& operator=(SubmissionQueue&& other) noexcept {
        if (this != &other) {
            mask     = exchange(other.mask, U32_0);
            array    = move(other.array);
            head     = exchange(other.head, null<u32>());
            tail     = exchange(other.tail, null<u32>());
            flags    = exchange(other.flags, null<u32>());
            dropped  = exchange(other.dropped, null<u32>());
            sqe_head = exchange(other.sqe_head, U32_0);
            sqe_tail = exchange(other.sqe_tail, U32_0);
            mem      = move(other.mem);
            sqes     = move(other.sqes);
        }
        return *this;
    }

    static usize max(usize a, usize b) {
        return (a > b) ? a : b;
    }

    static SysRes<SubmissionQueue> init(FdUL fd, const io_uring_params& p) {
        const auto& off       = p.sq_off;
        const usize sqes_size = p.sq_entries;
        const usize mem_size =
            max(off.array + p.sq_entries * sizeof(u32),
                p.cq_off.cqes + p.cq_entries * sizeof(io_uring_sqe));

        auto m_r = mmap(
            NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd,
            IORING_OFF_SQ_RING
        );
        if (m_r.is_err()) return m_r.return_err<SubmissionQueue>();
        auto m    = m_r.unwrap();

        auto sqes = mmap(
                        NULL, sqes_size * sizeof(io_uring_sqe), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_POPULATE, fd, IORING_OFF_SQES
        )
                        .unsafe_cast<io_uring_sqe*>();
        if (sqes.is_err()) return sqes.return_err<SubmissionQueue>();

        return SysRes<SubmissionQueue>::from_successful(SubmissionQueue(
            *off_cast(m, off.ring_mask), Slice<u32>::init(p.sq_entries, off_cast(m, off.array)),
            off_cast(m, off.head), off_cast(m, off.tail), off_cast(m, off.dropped),
            off_cast(m, off.flags), 0, 0, Slice<u8>::init(mem_size, m),
            Slice<io_uring_sqe>::init(sqes_size, sqes.unwrap())
        ));
    }

    void deinit(this SubmissionQueue& self) {
        munmap(self.mem.ptr, self.mem.len);
        munmap(self.sqes.ptr, self.sqes.len * sizeof(io_uring_sqe));
    }
};

struct CompletionQueue {
    u32 mask{0};
    u32* head{null<u32>()};
    u32* tail{null<u32>()};
    u32* overflow{null<u32>()};
    Slice<io_uring_cqe> cqes{};

    CompletionQueue() {}
    CompletionQueue(u32 _mask, u32* _head, u32* _tail, u32* _overflow, Slice<io_uring_cqe> _cqes)
        : mask(_mask), head(_head), tail(_tail), overflow(_overflow), cqes(move(_cqes)) {}

    CompletionQueue(const CompletionQueue& t)            = delete;
    CompletionQueue& operator=(const CompletionQueue& t) = delete;
    CompletionQueue(CompletionQueue&& s) noexcept
        : mask(exchange(s.mask, U32_0)), head(exchange(s.head, null<u32>())),
          tail(exchange(s.tail, null<u32>())), overflow(exchange(s.overflow, null<u32>())),
          cqes(move(s.cqes)) {}
    CompletionQueue& operator=(CompletionQueue&& other) noexcept {
        if (this != &other) {
            mask     = exchange(other.mask, U32_0);
            head     = exchange(other.head, null<u32>());
            tail     = exchange(other.tail, null<u32>());
            overflow = exchange(other.overflow, null<u32>());
            cqes     = move(other.cqes);
        }
        return *this;
    }

    static CompletionQueue init(const io_uring_params& p, const SubmissionQueue& sq) {
        const auto& off = p.cq_off;
        u8* m           = sq.mem.ptr;

        return CompletionQueue(
            *off_cast(m, off.ring_mask), off_cast(m, off.head), off_cast(m, off.tail),
            off_cast(m, off.overflow),
            Slice<io_uring_cqe>::init(
                p.cq_entries, reinterpret_cast<io_uring_cqe*>(off_cast(m, off.cqes))
            )
        );
    }

    void deinit() {}
};

struct IoUring {
    FdL io_fd{-1};
    u32 features{0};
    u32 setup_flags{0};
    SubmissionQueue sq{};
    CompletionQueue cq{};

    IoUring() {}
    IoUring(const IoUring& t)            = delete;
    IoUring& operator=(const IoUring& t) = delete;
    IoUring(IoUring&& i) noexcept
        : io_fd(exchange(i.io_fd, static_cast<FdL>(-1))), features(exchange(i.features, U32_0)),
          setup_flags(exchange(i.setup_flags, U32_0)), sq(move(i.sq)), cq(move(i.cq)) {}
    IoUring& operator=(IoUring&& other) noexcept {
        if (this != &other) {
            io_fd       = exchange(other.io_fd, static_cast<FdL>(-1));
            features    = exchange(other.features, U32_0);
            setup_flags = exchange(other.setup_flags, U32_0);
            sq          = move(other.sq);
            cq          = move(other.cq);
        }
        return *this;
    }

    void close_fd(this IoUring& self) {
        syscall(__NR_close, self.io_fd);
        self.io_fd = -1;
    }

    static bool is_supported(u32 features) {
        // since kernel 5.19
        return features & IORING_FEAT_NODROP &&
               // since kernel 5.17
               features & IORING_FEAT_CQE_SKIP &&
               // since kernel 5.11
               features & IORING_FEAT_SQPOLL_NONFIXED &&
               // since kernel 5.7
               features & IORING_FEAT_FAST_POLL &&
               // since kernel 5.4
               features & IORING_FEAT_SINGLE_MMAP;
    }

    static SysRes<IoUring> init(u32 entries, u32 flags, u32 thread_idle, u32 thread_cpu) {
        IoUring self;
        entries           = ceilPowerOfTwo(entries);

        io_uring_params p = zeroed<io_uring_params>();
        p.flags           = flags;
        p.sq_thread_cpu   = thread_cpu;
        p.sq_thread_idle  = thread_idle;

        auto r            = io_uring_setup(entries, &p);
        if (r.is_err()) return r.return_err<IoUring>();
        self.io_fd = static_cast<FdL>(r.unwrap());

        if (!is_supported(p.features)) {
            self.close_fd();
            return SysRes<IoUring>::from_err(SysResKind::NOSYS);
        }

        auto sq_r = SubmissionQueue::init(static_cast<FdUL>(self.io_fd), p);
        if (sq_r.is_err()) {
            self.close_fd();
            return r.return_err<IoUring>();
        }

        self.features = p.features;
        self.sq       = sq_r.unwrap();
        self.cq       = CompletionQueue::init(p, self.sq);

        return SysRes<IoUring>::from_successful(move(self));
    }

    void deinit(this IoUring& self) {
        self.cq.deinit();
        self.sq.deinit();
        self.close_fd();
    }

    u32 sq_ready(this IoUring& self) {
        return self.sq.sqe_tail - __atomic_load_n(self.sq.head, __ATOMIC_ACQUIRE);
    }
    u32 flush_sq(this IoUring& self) {
        if (self.sq.sqe_head != self.sq.sqe_tail) {
            u32 to_submit = self.sq.sqe_tail - self.sq.sqe_head;
            u32 tail      = *self.sq.tail;

            for (usize i = 0; i < to_submit; i++) {
                *self.sq.array.get(tail & self.sq.mask) = self.sq.sqe_head & self.sq.mask;
                tail++;
                self.sq.sqe_head++;
            }

            __atomic_store_n(self.sq.tail, tail, __ATOMIC_RELEASE);
        }

        return self.sq_ready();
    }

    inline u32 load_sq_flags(this IoUring& self) {
        return __atomic_load_n(self.sq.flags, __ATOMIC_RELAXED);
    }

    bool cq_ring_needs_flush(this IoUring& self) {
        const u32 f = self.load_sq_flags();
        return (f & IORING_SQ_CQ_OVERFLOW) || (f & IORING_SQ_TASKRUN);
    }
    bool cq_ring_needs_enter(this IoUring& self) {
        return (self.setup_flags & IORING_SETUP_IOPOLL) || self.cq_ring_needs_flush();
    }
    bool sq_ring_needs_enter(this IoUring& self, u32& flags) {
        if (!(self.setup_flags & IORING_SETUP_SQPOLL)) return true;

        if (*self.sq.flags & IORING_SQ_NEED_WAKEUP) {
            flags |= IORING_ENTER_SQ_WAKEUP;
            return true;
        }

        return false;
    }

    SysRes<usize> submit_and_wait(this IoUring& self, u32 wait_nr) {
        u32 submitted = self.flush_sq();
        u32 flags     = 0;

        if (self.sq_ring_needs_enter(flags) || wait_nr > 0) {
            if (wait_nr > 0 || self.cq_ring_needs_enter()) {
                flags |= IORING_ENTER_GETEVENTS;
            }

            return io_uring_enter(static_cast<FdU>(self.io_fd), submitted, wait_nr, flags, NULL, 0);
        }

        return SysRes<usize>::from_successful(submitted);
    }
    SysRes<usize> submit(this IoUring& self) {
        return self.submit_and_wait(0);
    }

    u32 cq_ready(this IoUring& self) {
        return *__atomic_load_n(&self.cq.tail, __ATOMIC_ACQUIRE) - *self.cq.head;
    }
    void cq_advance(this IoUring& self, u32 count) {
        if (count > 0) {
            __atomic_store_n(&self.cq.head, self.cq.head + count, __ATOMIC_RELEASE);
        }
    }

    io_uring_sqe* get_sqe(this IoUring& self, u8 op, u64 user_data, FdI fd = 0) {
        u32 head = __atomic_load_n(self.sq.head, __ATOMIC_ACQUIRE);
        u32 next = self.sq.sqe_tail + 1;

        if (next - head > self.sq.sqes.len) {
            return reinterpret_cast<io_uring_sqe*>(NULL);
        }

        auto* sqe        = &self.sq.sqes.ptr [static_cast<usize>(self.sq.sqe_tail & self.sq.mask)];
        self.sq.sqe_tail = next;

        sqe->opcode      = op;
        sqe->fd          = fd;
        sqe->user_data   = user_data;
        return sqe;
    }

    io_uring_sqe* nop(this IoUring& self, u64 user_data) {
        return self.get_sqe(IORING_OP_NOP, user_data);
    }

    io_uring_sqe*
    pread(this IoUring& self, u64 user_data, FdI fd, void* buf, usize count, off_t offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_READ, user_data, fd);
        sqe->addr         = (u64)buf;
        sqe->len          = (u32)count;
        sqe->off          = (u64)offset;
        return sqe;
    }
    io_uring_sqe* read(this IoUring& self, u64 user_data, FdI fd, void* buf, usize count) {
        return self.pread(user_data, fd, buf, count, 0);
    }
    io_uring_sqe*
    pwrite(this IoUring& self, u64 user_data, FdI fd, const void* buf, usize count, off_t offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WRITE, user_data, fd);
        sqe->addr         = (u64)buf;
        sqe->len          = (u32)count;
        sqe->off          = (u64)offset;
        return sqe;
    }
    io_uring_sqe* write(this IoUring& self, u64 user_data, FdI fd, const void* buf, usize count) {
        return self.pwrite(user_data, fd, buf, count, 0);
    }

    io_uring_sqe* preadv2(
        this IoUring& self, u64 user_data, FdI fd, const struct iovec* iov, u32 iovcnt,
        off_t offset, rwf_t flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_READV, user_data, fd);
        sqe->rw_flags     = flags;
        sqe->len          = iovcnt;
        sqe->addr         = (u64)iov;
        sqe->off          = (u64)offset;
        return sqe;
    }
    io_uring_sqe* preadv(
        this IoUring& self, u64 user_data, FdI fd, const struct iovec* iov, u32 iovcnt, off_t offset
    ) {
        return self.preadv2(user_data, fd, iov, iovcnt, offset, 0);
    }
    io_uring_sqe*
    readv(this IoUring& self, u64 user_data, FdI fd, const struct iovec* iov, u32 iovcnt) {
        return self.preadv2(user_data, fd, iov, iovcnt, 0, 0);
    }
    io_uring_sqe* pwritev2(
        this IoUring& self, u64 user_data, FdI fd, const struct iovec* iov, u32 iovcnt,
        off_t offset, rwf_t flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WRITEV, user_data, fd);
        sqe->rw_flags     = flags;
        sqe->len          = iovcnt;
        sqe->addr         = (u64)iov;
        sqe->off          = (u64)offset;
        return sqe;
    }
    io_uring_sqe* pwritev(
        this IoUring& self, u64 user_data, FdI fd, const struct iovec* iov, u32 iovcnt, off_t offset
    ) {
        return self.pwritev2(user_data, fd, iov, iovcnt, offset, 0);
    }
    io_uring_sqe*
    writev(this IoUring& self, u64 user_data, FdI fd, const struct iovec* iov, u32 iovcnt) {
        return self.pwritev2(user_data, fd, iov, iovcnt, 0, 0);
    }

    io_uring_sqe* fsync(this IoUring& self, u64 user_data, FdI fd) {
        return self.get_sqe(IORING_OP_FSYNC, user_data, fd);
    }

    io_uring_sqe* poll_add(this IoUring& self, u64 user_data, FdI fd, u32 poll_mask) {
        static_assert(__BYTE_ORDER == __LITTLE_ENDIAN, "Unsupported endianness!");

        io_uring_sqe* sqe  = self.get_sqe(IORING_OP_POLL_ADD, user_data, fd);
        sqe->poll32_events = poll_mask;
        return sqe;
    }
    io_uring_sqe* poll_add_multishot(this IoUring& self, u64 user_data, FdI fd, u32 poll_mask) {
        io_uring_sqe* sqe = self.poll_add(user_data, fd, poll_mask);
        sqe->len          = IORING_POLL_ADD_MULTI;
        return sqe;
    }
    io_uring_sqe* poll_remove(this IoUring& self, u64 user_data) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_REMOVE, user_data, -1);
        sqe->addr         = user_data;
        sqe->len          = IORING_POLL_ADD_MULTI;
        return sqe;
    }
    io_uring_sqe* poll_update(
        this IoUring& self, u64 old_user_data, u64 new_user_data, u32 poll_mask, u32 flags
    ) {
        static_assert(__BYTE_ORDER == __LITTLE_ENDIAN, "Unsupported endianness!");

        io_uring_sqe* sqe  = self.get_sqe(IORING_OP_POLL_REMOVE, new_user_data, -1);
        sqe->len           = flags;
        sqe->poll32_events = poll_mask;
        sqe->addr          = old_user_data;
        return sqe;
    }

    io_uring_sqe*
    epoll_ctl(this IoUring& self, u64 user_data, i32 epfd, FdI fd, i32 op, struct epoll_event* ev) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_EPOLL_CTL, user_data, epfd);
        sqe->off          = (u64)fd;
        sqe->len          = (u32)op;
        sqe->addr         = (u64)ev;
        return sqe;
    }

    io_uring_sqe*
    sync_file_range(this IoUring& self, u64 user_data, FdI fd, usize len, off_t offset, u32 flags) {
        io_uring_sqe* sqe     = self.get_sqe(IORING_OP_SYNC_FILE_RANGE, user_data, fd);
        sqe->sync_range_flags = flags;
        sqe->len              = (u32)len;
        sqe->off              = (u64)offset;
        return sqe;
    }

    io_uring_sqe*
    sendmsg(this IoUring& self, u64 user_data, FdI fd, const struct msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SENDMSG, user_data, fd);
        sqe->len          = 1;
        sqe->msg_flags    = flags;
        sqe->addr         = (u64)msg;
        return sqe;
    }
};

DIAG_IGNORE_CLANG_POP
