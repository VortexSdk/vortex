#pragma once

#include "../diagnostics.hpp"
#include "../math.hpp"
#include "../mem/utils.hpp"
#include "../metap/metap.hpp"
#include "../numbers.hpp"
#include "linux/io_uring.h"
#include "syscall/syscall.hpp"
#include "syscall/wrapperHelper.hpp"
#include "vortex/mem/Allocator.hpp"
#include "vortex/Vec.hpp"
#include <asm-generic/mman-common.h>

inline u32* off_cast(u8* mem, u32 idx) {
    return reinterpret_cast<u32*>(&mem [idx]);
}

DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast", "-Wcast-align")

#define IO_TRY_ADD(expr, t) ({ TRY(expr, t, SysResKind::NOMEM); })
#define __IO_TRY_ADD_AND_GET_INNER(r, expr, t, ctr)                                                \
    ({                                                                                             \
        IO_TRY_ADD(expr, t);                                                                       \
        auto UNIQUE_NAME(io_uring_submitted, ctr) = TRY(r->submit_and_get_one(), t);               \
        ::vortex::move(TRY(::vortex::IoUringRes(move(UNIQUE_NAME(io_uring_submitted, ctr))), t));  \
    })
#define __IO_TRY_ALL_AND_SUBMIT_INNER(r, count, t, ctr)                                            \
    ({                                                                                             \
        auto UNIQUE_NAME(io_uring_submitted, ctr) =                                                \
            TRY(r->submit_and_wait(static_cast<u32>(count)), t);                                   \
        IoUringRes UNIQUE_NAME(io_uring_arr, ctr) [count];                                         \
        for (usize UNIQUE_NAME(io_uring_i, ctr) = 0;                                               \
             UNIQUE_NAME(io_uring_i, ctr) < UNIQUE_NAME(io_uring_submitted, ctr);                  \
             UNIQUE_NAME(io_uring_i, ctr)++) {                                                     \
            UNIQUE_NAME(io_uring_arr, ctr)                                                         \
            [UNIQUE_NAME(io_uring_i, ctr)] =                                                       \
                ::vortex::IoUringRes(*r->cq.cqes [(*r->cq.head) + UNIQUE_NAME(io_uring_i, ctr)]);  \
            if (UNIQUE_NAME(io_uring_arr, ctr) [UNIQUE_NAME(io_uring_i, ctr)].is_err()) {          \
                r->cq_advance(static_cast<u32>(UNIQUE_NAME(io_uring_i, ctr)));                     \
                return UNIQUE_NAME(io_uring_arr, ctr) [UNIQUE_NAME(io_uring_i, ctr)]               \
                    .return_err<t>();                                                              \
            }                                                                                      \
        }                                                                                          \
        r->cq_advance(static_cast<u32>(UNIQUE_NAME(io_uring_submitted, ctr)));                     \
        UNIQUE_NAME(io_uring_arr, ctr);                                                            \
    })
#define IO_TRY_ADD_AND_GET(r, expr, t)     __IO_TRY_ADD_AND_GET_INNER(r, expr, t, __COUNTER__)
#define IO_TRY_ALL_AND_SUBMIT(r, count, t) __IO_TRY_ALL_AND_SUBMIT_INNER(r, count, t, __COUNTER__)

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
        : mask(exchange(s.mask, 0_u32)), array(move(s.array)), head(exchange(s.head, null<u32>())),
          tail(exchange(s.tail, null<u32>())), flags(exchange(s.flags, null<u32>())),
          dropped(exchange(s.dropped, null<u32>())), sqe_head(exchange(s.sqe_head, 0_u32)),
          sqe_tail(exchange(s.sqe_tail, 0_u32)), mem(move(s.mem)), sqes(move(s.sqes)) {}
    SubmissionQueue& operator=(SubmissionQueue&& other) noexcept {
        if (this != &other) {
            mask     = exchange(other.mask, 0_u32);
            array    = move(other.array);
            head     = exchange(other.head, null<u32>());
            tail     = exchange(other.tail, null<u32>());
            flags    = exchange(other.flags, null<u32>());
            dropped  = exchange(other.dropped, null<u32>());
            sqe_head = exchange(other.sqe_head, 0_u32);
            sqe_tail = exchange(other.sqe_tail, 0_u32);
            mem      = move(other.mem);
            sqes     = move(other.sqes);
        }
        return *this;
    }

    static SysRes<SubmissionQueue> init(FdUL fd, const io_uring_params& p) {
        const auto& off       = p.sq_off;
        const usize sqes_size = p.sq_entries;
        const usize mem_size =
            max(off.array + p.sq_entries * sizeof(u32),
                p.cq_off.cqes + p.cq_entries * sizeof(io_uring_sqe));

        auto m =
            TRY(mmap(
                    NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd,
                    IORING_OFF_SQ_RING
                ),
                SubmissionQueue);
        auto sqes =
            TRY(mmap(
                    NULL, sqes_size * sizeof(io_uring_sqe), PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_POPULATE, fd, IORING_OFF_SQES
                )
                    .unsafe_cast<io_uring_sqe*>(),
                SubmissionQueue);

        return SysRes<SubmissionQueue>::from_successful(SubmissionQueue(
            *off_cast(m, off.ring_mask), Slice<u32>::init(p.sq_entries, off_cast(m, off.array)),
            off_cast(m, off.head), off_cast(m, off.tail), off_cast(m, off.dropped),
            off_cast(m, off.flags), 0, 0, Slice<u8>::init(mem_size, m),
            Slice<io_uring_sqe>::init(sqes_size, sqes)
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
        : mask(exchange(s.mask, 0_u32)), head(exchange(s.head, null<u32>())),
          tail(exchange(s.tail, null<u32>())), overflow(exchange(s.overflow, null<u32>())),
          cqes(move(s.cqes)) {}
    CompletionQueue& operator=(CompletionQueue&& other) noexcept {
        if (this != &other) {
            mask     = exchange(other.mask, 0_u32);
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
        : io_fd(exchange(i.io_fd, static_cast<FdL>(-1))), features(exchange(i.features, 0_u32)),
          setup_flags(exchange(i.setup_flags, 0_u32)), sq(move(i.sq)), cq(move(i.cq)) {}
    IoUring& operator=(IoUring&& other) noexcept {
        if (this != &other) {
            io_fd       = exchange(other.io_fd, static_cast<FdL>(-1));
            features    = exchange(other.features, 0_u32);
            setup_flags = exchange(other.setup_flags, 0_u32);
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

        self.io_fd        = static_cast<FdL>(TRY(io_uring_setup(entries, &p), IoUring));

        if (!is_supported(p.features)) {
            self.close_fd();
            return SysRes<IoUring>::from_err(SysResKind::NOSYS);
        }

        auto sq_r = SubmissionQueue::init(static_cast<FdUL>(self.io_fd), p);
        if (sq_r.is_err()) {
            self.close_fd();
            return sq_r.return_err<IoUring>();
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

        if (next - head > self.sq.sqes.len) [[unlikely]] {
            return null<io_uring_sqe>();
        }

        auto* sqe        = &self.sq.sqes.ptr [static_cast<usize>(self.sq.sqe_tail & self.sq.mask)];
        self.sq.sqe_tail = next;

        sqe->opcode      = op;
        sqe->fd          = fd;
        sqe->user_data   = user_data;
        return sqe;
    }

    SysRes<IoUringRes> get_cqe(this IoUring& self) {
        if (self.cq_ready() == 0) [[unlikely]]
            return SysRes<IoUringRes>::from_err(SysResKind::INPROGRESS);

        const auto cqe = IoUringRes(*self.cq.cqes [*self.cq.head]);
        self.cq_advance(1);

        return SysRes<IoUringRes>::from_successful(cqe);
    }

    template <AllocatorStrategy U>
    SysRes<Vec<IoUringRes>> submit_and_get(this IoUring& self, Allocator<U>* a, u32 wait_nr) {
        auto submitted = TRY(self.submit_and_wait(wait_nr), Vec<IoUringRes>);
        auto v         = TRY(Vec<IoUringRes>::init(a, submitted), Vec<IoUringRes>);

        for (usize i = 0; i < submitted; i++) {
            const auto push_r = v.push(a, IoUringRes(*self.cq.cqes [(*self.cq.head) + i]));
            if (push_r) {
                self.cq_advance(static_cast<u32>(i));
                return SysRes<Vec<IoUringRes>>::from_err(SysResKind::NOMEM);
            }
        }
        self.cq_advance(static_cast<u32>(submitted));

        return SysRes<Vec<io_uring_cqe>>::from_successful(v);
    }

    template <usize wait_nr> SysRes<IoUringRes*> submit_and_get(this IoUring& self) {
        IoUringRes arr [wait_nr];
        auto submitted = TRY(self.submit_and_wait(wait_nr), IoUringRes*);

        for (usize i = 0; i < submitted; i++)
            arr [i] = IoUringRes(*self.cq.cqes [(*self.cq.head) + i]);
        self.cq_advance(static_cast<u32>(submitted));

        return SysRes<IoUringRes*>::from_successful(arr);
    }

    SysRes<IoUringRes> submit_and_get_one(this IoUring& self) {
        auto r = self.submit_and_get<1>();
        if (r.is_err()) return r.return_err<IoUringRes>();

        return SysRes<IoUringRes>::from_successful(r.unwrap() [0]);
    }

    io_uring_sqe* nop(this IoUring& self, u64 user_data) {
        return self.get_sqe(IORING_OP_NOP, user_data);
    }

    io_uring_sqe*
    pread(this IoUring& self, u64 user_data, FdI fd, void* buf, usize count, off_t offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_READ, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = (u64)buf;
        sqe->len  = (u32)count;
        sqe->off  = (u64)offset;
        return sqe;
    }

    io_uring_sqe* read(this IoUring& self, u64 user_data, FdI fd, void* buf, usize count) {
        return self.pread(user_data, fd, buf, count, 0);
    }

    io_uring_sqe*
    pwrite(this IoUring& self, u64 user_data, FdI fd, const void* buf, usize count, off_t offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WRITE, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = (u64)buf;
        sqe->len  = (u32)count;
        sqe->off  = (u64)offset;
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
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->rw_flags = (__kernel_rwf_t)flags;
        sqe->len      = iovcnt;
        sqe->addr     = (u64)iov;
        sqe->off      = (u64)offset;
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
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->rw_flags = (__kernel_rwf_t)flags;
        sqe->len      = iovcnt;
        sqe->addr     = (u64)iov;
        sqe->off      = (u64)offset;
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

        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_ADD, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->poll32_events = poll_mask;
        return sqe;
    }

    io_uring_sqe* poll_add_multishot(this IoUring& self, u64 user_data, FdI fd, u32 poll_mask) {
        io_uring_sqe* sqe = self.poll_add(user_data, fd, poll_mask);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len = IORING_POLL_ADD_MULTI;
        return sqe;
    }

    io_uring_sqe* poll_remove(this IoUring& self, u64 user_data) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_REMOVE, user_data, -1);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = user_data;
        sqe->len  = IORING_POLL_ADD_MULTI;
        return sqe;
    }

    io_uring_sqe* poll_update(
        this IoUring& self, u64 old_user_data, u64 new_user_data, u32 poll_mask, u32 flags
    ) {
        static_assert(__BYTE_ORDER == __LITTLE_ENDIAN, "Unsupported endianness!");

        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_REMOVE, new_user_data, -1);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len           = flags;
        sqe->poll32_events = poll_mask;
        sqe->addr          = old_user_data;
        return sqe;
    }

    io_uring_sqe* poll_update(
        this IoUring& self, u64 user_data, u64 old_user_data, u64 new_user_data, u32 poll_mask,
        u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_REMOVE, user_data, -1);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr          = old_user_data;
        sqe->len           = flags;
        sqe->off           = new_user_data;
        sqe->poll32_events = poll_mask;
        return sqe;
    }

    io_uring_sqe*
    epoll_ctl(this IoUring& self, u64 user_data, i32 epfd, FdI fd, i32 op, struct epoll_event* ev) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_EPOLL_CTL, user_data, epfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->off  = (u64)fd;
        sqe->len  = (u32)op;
        sqe->addr = (u64)ev;
        return sqe;
    }

    io_uring_sqe*
    sync_file_range(this IoUring& self, u64 user_data, FdI fd, usize len, off_t offset, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SYNC_FILE_RANGE, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->sync_range_flags = flags;
        sqe->len              = (u32)len;
        sqe->off              = (u64)offset;
        return sqe;
    }

    io_uring_sqe* sendmsg(this IoUring& self, u64 user_data, FdI fd, const msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SENDMSG, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len       = 1;
        sqe->msg_flags = flags;
        sqe->addr      = (u64)msg;
        return sqe;
    }

    io_uring_sqe* accept(
        this IoUring& self, u64 user_data, FdI fd, sockaddr* addr, socklen_t* addrlen, int flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ACCEPT, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr         = (u64)addr;
        sqe->addr2        = (u64)addrlen;
        sqe->accept_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* connect(
        this IoUring& self, u64 user_data, FdI sockfd, const sockaddr* addr, socklen_t addrlen
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_CONNECT, user_data, sockfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = reinterpret_cast<u64>(addr);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->off = addrlen;
        return sqe;
    }

    io_uring_sqe* shutdown(this IoUring& self, u64 user_data, FdI fd, int how) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SHUTDOWN, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len = (u32)how;
        return sqe;
    }

    io_uring_sqe* renameat(
        this IoUring& self, u64 user_data, int olddfd, const char* oldpath, int newdfd,
        const char* newpath, int flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RENAMEAT, user_data, olddfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr         = (u64)oldpath;
        sqe->len          = (u32)newdfd;
        sqe->addr2        = (u64)newpath;
        sqe->rename_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* unlink(this IoUring& self, u64 user_data, const char* path, int flags) {
        return self.unlinkat(user_data, AT_FDCWD, path, flags);
    }

    io_uring_sqe*
    unlinkat(this IoUring& self, u64 user_data, int dfd, const char* path, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_UNLINKAT, user_data, dfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr         = (u64)path;
        sqe->unlink_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    openat(this IoUring& self, u64 user_data, int dfd, const char* path, int flags, mode_t mode) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_OPENAT, user_data, dfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr       = (u64)path;
        sqe->len        = (u32)mode;
        sqe->open_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* statx(
        this IoUring& self, u64 user_data, int dfd, const char* path, int flags, unsigned mask,
        statx_t* statxbuf
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_STATX, user_data, dfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)path;
        sqe->len         = mask;
        sqe->addr2       = (u64)statxbuf;
        sqe->statx_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* splice(
        this IoUring& self, u64 user_data, FdI fd_in, i64 off_in, FdI fd_out, i64 off_out,
        u32 nbytes, unsigned int splice_flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SPLICE, user_data, fd_out);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr          = 0;
        sqe->len           = nbytes;
        sqe->off           = (u64)off_out;
        sqe->splice_off_in = (u64)off_in;
        sqe->splice_fd_in  = fd_in;
        sqe->splice_flags  = splice_flags;
        return sqe;
    }

    io_uring_sqe*
    tee(this IoUring& self, u64 user_data, FdI fd_in, FdI fd_out, u32 nbytes,
        unsigned int splice_flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TEE, user_data, fd_out);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr         = 0;
        sqe->len          = nbytes;
        sqe->off          = 0;
        sqe->splice_fd_in = fd_in;
        sqe->splice_flags = splice_flags;
        return sqe;
    }

    io_uring_sqe*
    timeout(this IoUring& self, u64 user_data, __kernel_timespec* ts, u32 count, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TIMEOUT, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr          = (u64)ts;
        sqe->len           = 1;
        sqe->off           = count;
        sqe->timeout_flags = flags;
        return sqe;
    }

    io_uring_sqe*
    timeout_remove(this IoUring& self, u64 user_data, u64 timeout_user_data, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TIMEOUT_REMOVE, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr          = timeout_user_data;
        sqe->timeout_flags = flags;
        return sqe;
    }

    io_uring_sqe* timeout_update(
        this IoUring& self, u64 user_data, __kernel_timespec* ts, u64 user_data_to_update, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TIMEOUT_REMOVE, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr          = user_data_to_update;
        sqe->off           = (u64)ts;
        sqe->timeout_flags = flags | IORING_TIMEOUT_UPDATE;
        return sqe;
    }

    io_uring_sqe*
    link_timeout(this IoUring& self, u64 user_data, __kernel_timespec* ts, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_LINK_TIMEOUT, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr          = (u64)ts;
        sqe->len           = 1;
        sqe->timeout_flags = flags;
        return sqe;
    }

    io_uring_sqe*
    files_update(this IoUring& self, u64 user_data, int* fds, u32 nr_fds, int offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FILES_UPDATE, user_data, -1);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = (u64)fds;
        sqe->len  = nr_fds;
        sqe->off  = (u64)offset;
        return sqe;
    }

    io_uring_sqe*
    fallocate(this IoUring& self, u64 user_data, FdI fd, int mode, i64 offset, u64 len) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FALLOCATE, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len  = (u32)mode;
        sqe->off  = (u64)offset;
        sqe->addr = len;
        return sqe;
    }

    io_uring_sqe*
    bind(this IoUring& self, u64 user_data, FdI fd, sockaddr* addr, socklen_t addrlen) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_BIND, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr  = (u64)addr;
        sqe->addr2 = 0;
        sqe->off   = addrlen;
        return sqe;
    }

    io_uring_sqe* listen(this IoUring& self, u64 user_data, FdI fd, int backlog) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_LISTEN, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = 0;
        sqe->len  = (u32)backlog;
        return sqe;
    }

    io_uring_sqe*
    recv(this IoUring& self, u64 user_data, FdI fd, void* buf, usize count, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RECV, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr      = (u64)buf;
        sqe->len       = (u32)count;
        sqe->msg_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* recvmsg(this IoUring& self, u64 user_data, FdI fd, msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RECVMSG, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr      = (u64)msg;
        sqe->len       = 1;
        sqe->msg_flags = flags;
        return sqe;
    }

    io_uring_sqe* mkdir(this IoUring& self, u64 user_data, const char* path, mode_t mode) {
        return self.mkdirat(user_data, AT_FDCWD, path, mode);
    }

    io_uring_sqe*
    mkdirat(this IoUring& self, u64 user_data, int dfd, const char* path, mode_t mode) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MKDIRAT, user_data, dfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr = (u64)path;
        sqe->len  = (u32)mode;
        return sqe;
    }

    io_uring_sqe*
    symlink(this IoUring& self, u64 user_data, const char* target, const char* linkpath) {
        return self.symlinkat(user_data, target, AT_FDCWD, linkpath);
    }

    io_uring_sqe* symlinkat(
        this IoUring& self, u64 user_data, const char* target, int newdfd, const char* linkpath
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SYMLINKAT, user_data, newdfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr  = (u64)target;
        sqe->addr2 = (u64)linkpath;
        return sqe;
    }

    io_uring_sqe*
    link(this IoUring& self, u64 user_data, const char* oldpath, const char* newpath, int flags) {
        return self.linkat(user_data, AT_FDCWD, oldpath, AT_FDCWD, newpath, flags);
    }

    io_uring_sqe* linkat(
        this IoUring& self, u64 user_data, int olddfd, const char* oldpath, int newdfd,
        const char* newpath, int flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_LINKAT, user_data, olddfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr           = (u64)oldpath;
        sqe->len            = (u32)newdfd;
        sqe->addr2          = (u64)newpath;
        sqe->hardlink_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* provide_buffers(
        this IoUring& self, u64 user_data, void* addr, i32 len, i32 nr, i32 bgid, i32 bid
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_PROVIDE_BUFFERS, user_data, nr);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr      = (u64)addr;
        sqe->len       = (u32)len;
        sqe->off       = (u64)bid;
        sqe->buf_group = (u16)bgid;
        return sqe;
    }

    io_uring_sqe* remove_buffers(this IoUring& self, u64 user_data, i32 nr, i32 bgid) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_REMOVE_BUFFERS, user_data, nr);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->buf_group = (u16)bgid;
        return sqe;
    }

    io_uring_sqe*
    sync_file_range(this IoUring& self, u64 user_data, FdI fd, u32 len, u64 offset, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SYNC_FILE_RANGE, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len              = len;
        sqe->off              = offset;
        sqe->sync_range_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    socket(this IoUring& self, u64 user_data, int domain, int type, int protocol, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SOCKET, user_data, domain);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->off      = (u64)type;
        sqe->len      = (u32)protocol;
        sqe->rw_flags = (__kernel_rwf_t)flags;
        return sqe;
    }

    io_uring_sqe* getxattr(
        this IoUring& self, u64 user_data, const char* name, char* value, const char* path, u32 len
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_GETXATTR, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->addr3       = (u64)path;
        sqe->len         = len;
        sqe->xattr_flags = 0;
        return sqe;
    }

    io_uring_sqe* setxattr(
        this IoUring& self, u64 user_data, const char* name, const char* value, const char* path,
        int flags, u32 len
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SETXATTR, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->addr3       = (u64)path;
        sqe->len         = len;
        sqe->xattr_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    fgetxattr(this IoUring& self, u64 user_data, FdI fd, const char* name, char* value, u32 len) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FGETXATTR, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->len         = len;
        sqe->xattr_flags = 0;
        return sqe;
    }

    io_uring_sqe* fsetxattr(
        this IoUring& self, u64 user_data, FdI fd, const char* name, const char* value, int flags,
        u32 len
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FSETXATTR, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->len         = len;
        sqe->xattr_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    msg_ring(this IoUring& self, u64 user_data, int fd, u32 len, u64 data, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MSG_RING, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->len            = len;
        sqe->off            = data;
        sqe->msg_ring_flags = flags;
        return sqe;
    }

    io_uring_sqe* msg_ring_fd(
        this IoUring& self, u64 user_data, int fd, int source_fd, int target_fd, u64 data, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MSG_RING, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr           = (u64)IORING_MSG_SEND_FD;
        sqe->addr3          = (u64)source_fd;
        sqe->len            = 0;
        sqe->off            = data;
        sqe->msg_ring_flags = flags;

        if (target_fd == static_cast<int>(IORING_FILE_INDEX_ALLOC)) target_fd--;

        sqe->file_index = (u32)(target_fd + 1);
        return sqe;
    }

    io_uring_sqe* cancel(this IoUring& self, u64 user_data, u64 cancel_user_data, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ASYNC_CANCEL, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr         = cancel_user_data;
        sqe->cancel_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* cancel_fd(this IoUring& self, u64 user_data, FdI fd, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ASYNC_CANCEL, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->cancel_flags = flags | IORING_ASYNC_CANCEL_FD;
        return sqe;
    }

    io_uring_sqe* fixed_fd_install(this IoUring& self, u64 user_data, FdI fd, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FIXED_FD_INSTALL, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->flags            = IOSQE_FIXED_FILE;
        sqe->install_fd_flags = flags;
        return sqe;
    }

    io_uring_sqe* ftruncate(this IoUring& self, u64 user_data, FdI fd, i64 offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FTRUNCATE, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->off = (u64)offset;
        return sqe;
    }

    io_uring_sqe* madvise(this IoUring& self, u64 user_data, void* addr, u32 len, int advice) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MADVISE, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr           = (u64)addr;
        sqe->len            = len;
        sqe->fadvise_advice = (u32)advice;
        return sqe;
    }

    io_uring_sqe* send_zc(
        this IoUring& self, u64 user_data, FdI sockfd, const void* buf, usize len, int flags,
        u16 zc_flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SEND_ZC, user_data, sockfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr      = (u64)buf;
        sqe->len       = (u32)len;
        sqe->msg_flags = (u32)flags;
        sqe->ioprio    = zc_flags;
        return sqe;
    }

    io_uring_sqe*
    sendmsg_zc(this IoUring& self, u64 user_data, FdI fd, const msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SENDMSG_ZC, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr      = (u64)msg;
        sqe->len       = 1;
        sqe->msg_flags = flags;
        return sqe;
    }

    io_uring_sqe*
    recv_multishot(this IoUring& self, u64 user_data, FdI sockfd, void* buf, usize len, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RECV, user_data, sockfd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr      = (u64)buf;
        sqe->len       = (u32)len;
        sqe->msg_flags = (u32)flags;
        sqe->ioprio |= IORING_RECV_MULTISHOT;
        return sqe;
    }

    io_uring_sqe* accept_multishot(
        this IoUring& self, u64 user_data, FdI fd, sockaddr* addr, socklen_t* addrlen, int flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ACCEPT, user_data, fd);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr         = (u64)addr;
        sqe->addr2        = (u64)addrlen;
        sqe->accept_flags = (u32)flags;
        sqe->ioprio |= IORING_ACCEPT_MULTISHOT;
        return sqe;
    }

    io_uring_sqe* waitid(
        this IoUring& self, u64 user_data, int idtype, FdI id, siginfo_t* infop, int options,
        u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WAITID, user_data, id);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr2        = (u64)infop;
        sqe->len          = (u32)idtype;
        sqe->waitid_flags = flags;
        sqe->file_index   = (u32)options;
        return sqe;
    }

    io_uring_sqe* futex_wait(
        this IoUring& self, u64 user_data, u32* futex, u64 val, u64 mask, int futex_flags, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FUTEX_WAIT, user_data, futex_flags);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)futex;
        sqe->len         = 0;
        sqe->off         = val;
        sqe->addr3       = mask;
        sqe->futex_flags = flags;
        return sqe;
    }

    io_uring_sqe* futex_wake(
        this IoUring& self, u64 user_data, u32* futex, u64 val, u64 mask, int futex_flags, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FUTEX_WAKE, user_data, futex_flags);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)futex;
        sqe->len         = 0;
        sqe->off         = val;
        sqe->addr3       = mask;
        sqe->futex_flags = flags;
        return sqe;
    }

    io_uring_sqe* futex_waitv(
        this IoUring& self, u64 user_data, struct futex_waitv* futex, u32 nr_futex, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FUTEX_WAITV, user_data);
        if (sqe == null<io_uring_sqe>()) return sqe;
        sqe->addr        = (u64)futex;
        sqe->len         = nr_futex;
        sqe->futex_flags = flags;
        return sqe;
    }

    io_uring_sqe* close(this IoUring& self, u64 user_data, FdI fd) {
        return self.get_sqe(IORING_OP_CLOSE, user_data, fd);
    }

    io_uring_sqe* close_direct(this IoUring& self, u64 user_data, u32 file_index) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_CLOSE, user_data, 0);
        if (sqe == null<io_uring_sqe>()) return sqe;
        if (file_index == static_cast<u32>(IORING_FILE_INDEX_ALLOC)) file_index--;
        sqe->file_index = file_index + 1;
        return sqe;
    }

    io_uring_sqe* link(this IoUring&, io_uring_sqe* sqe) {
        if (sqe != null<io_uring_sqe>()) [[likely]]
            sqe->flags |= IOSQE_IO_LINK;
        return sqe;
    }
};

DIAG_IGNORE_CLANG_POP
