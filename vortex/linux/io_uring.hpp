#pragma once

#include "../math.hpp"
#include "../mem/utils.hpp"
#include "../metap/diagnostics.hpp"
#include "../metap/metap.hpp"
#include "../metap/structs.hpp"
#include "../numbers.hpp"
#include "linux/io_uring.h"
#include "syscall/SysRes.hpp"
#include "syscall/wrapper.hpp"

inline u32* off_cast(u8* mem, u32 idx) {
    return reinterpret_cast<u32*>(&mem [idx]);
}

DIAG_IGNORE_CLANG_PUSH("-Wold-style-cast", "-Wcast-align")

#define __IO_TRY_ADD(expr, sqe)                                                                    \
    ({                                                                                             \
        auto sqe = VORTEX_NS(move(expr));                                                          \
        if (sqe == reinterpret_cast<decltype(sqe)>(0)) [[unlikely]]                                \
            return VORTEX_NS(ErrSysRes(SysResKind::NOMEM, __FILE__, __LINE__));                    \
        sqe;                                                                                       \
    })
#define IO_TRY_ADD(expr) __IO_TRY_ADD(expr, UNIQUE_NAME(sqe))
#define __IO_TRY_SUBMIT(r, expr, sqe, submitted_r)                                                 \
    ({                                                                                             \
        auto sqe = VORTEX_NS(move(expr));                                                          \
        if (sqe == reinterpret_cast<decltype(sqe)>(0)) [[unlikely]]                                \
            return VORTEX_NS(ErrSysRes(SysResKind::NOMEM, __FILE__, __LINE__));                    \
                                                                                                   \
        auto submitted_r = r.submit();                                                             \
        if (submitted_r.is_err()) [[unlikely]]                                                     \
            return VORTEX_NS(ErrSysRes(submitted_r.kind, __FILE__, __LINE__));                     \
    })
#define IO_TRY_SUBMIT(r, expr) __IO_TRY_SUBMIT(r, expr, UNIQUE_NAME(sqe), UNIQUE_NAME(sqe))
// TODO: HANDLE THE ERROR FOR SUBMIT!
#define __IO_TRY_ADD_AND_GET_INNER(expr, sqe, submitted_r, copy_r, submitted)                      \
    ({                                                                                             \
        auto sqe = VORTEX_NS(move((expr)));                                                        \
        if (sqe == reinterpret_cast<decltype(sqe)>(0)) [[unlikely]]                                \
            return VORTEX_NS(ErrSysRes(VORTEX_NS(SysResKind::NOMEM), __FILE__, __LINE__));         \
                                                                                                   \
        auto submitted_r = r->submit();                                                            \
        if (submitted_r.is_err()) [[unlikely]]                                                     \
            return VORTEX_NS(ErrSysRes(submitted_r.kind, __FILE__, __LINE__));                     \
                                                                                                   \
        auto copy_r = r->copy_cqe();                                                               \
        if (copy_r.is_err()) [[unlikely]]                                                          \
            return VORTEX_NS(ErrSysRes(copy_r.kind, __FILE__, __LINE__));                          \
        auto submitted = VORTEX_NS(move(copy_r.unwrap()));                                         \
                                                                                                   \
        if (submitted.is_err()) [[unlikely]]                                                       \
            return VORTEX_NS(ErrSysRes(submitted.kind, __FILE__, __LINE__));                       \
                                                                                                   \
        submitted.unwrap();                                                                        \
    })
#define IO_TRY_ADD_AND_GET(expr)                                                                   \
    __IO_TRY_ADD_AND_GET_INNER(                                                                    \
        expr, UNIQUE_NAME(sqe), UNIQUE_NAME(submitted_r), UNIQUE_NAME(copy_r),                     \
        UNIQUE_NAME(submitted)                                                                     \
    )

struct CqeRes;
using IoUringRes = SysRes<CqeRes>;

struct CqeRes {
    VAL_STRUCT(CqeRes, user_data, 0_u64, res, 0_u32, flags, 0_u32)

    static IoUringRes from_cqe(io_uring_cqe cqe) {
        SysRes<usize> r = SysRes<usize>::from_sys(static_cast<usize>(cqe.res));
        return IoUringRes::from_kind(
            CqeRes(cqe.user_data, static_cast<u32>(cqe.res), cqe.flags), r.kind
        );
    }
};

struct SubmissionQueue {
    PIN_STRUCT(
        SubmissionQueue, mask, 0_u32, array, (Slice<u32>()), head, reinterpret_cast<u32*>(0), tail,
        reinterpret_cast<u32*>(0), flags, reinterpret_cast<u32*>(0), dropped,
        reinterpret_cast<u32*>(0), sqe_head, 0_u32, sqe_tail, 0_u32, mem, Slice<u8>(), sqes,
        Slice<io_uring_sqe>()
    )

    static SysRes<SubmissionQueue> init(Fd fd, const io_uring_params* p) {
        const io_sqring_offsets* off = &p->sq_off;
        const usize sqes_size        = p->sq_entries;
        const usize mem_size =
            max(off->array + p->sq_entries * sizeof(u32),
                p->cq_off.cqes + p->cq_entries * sizeof(io_uring_sqe));

        u8* m      = TRY(mmap(
            null, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd,
            IORING_OFF_SQ_RING
        ));
        auto* sqes = reinterpret_cast<io_uring_sqe*>(TRY(mmap(
            null, sqes_size * sizeof(io_uring_sqe), PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_POPULATE, fd, IORING_OFF_SQES
        )));

        return SubmissionQueue(
            *off_cast(m, off->ring_mask), Slice<u32>::init(p->sq_entries, off_cast(m, off->array)),
            off_cast(m, off->head), off_cast(m, off->tail), off_cast(m, off->flags),
            off_cast(m, off->dropped), 0, 0, Slice<u8>::init(mem_size, m),
            Slice<io_uring_sqe>::init(sqes_size, sqes)
        );
    }

    SysRes<None> deinit(this SubmissionQueue& self) {
        TRY(munmap(self.mem.ptr, self.mem.len));
        TRY(munmap(self.sqes.ptr, self.sqes.len * sizeof(io_uring_sqe)));

        return None();
    }
};

struct CompletionQueue {
    PIN_STRUCT(
        CompletionQueue, mask, 0_u32, head, reinterpret_cast<u32*>(0), tail,
        reinterpret_cast<u32*>(0), overflow, reinterpret_cast<u32*>(0), cqes,
        (Slice<io_uring_cqe>())
    )

    static CompletionQueue init(const io_uring_params* p, const SubmissionQueue* sq) {
        const io_cqring_offsets* off = &p->cq_off;
        u8* m                        = sq->mem.ptr;
        return {
            *off_cast(m, off->ring_mask), off_cast(m, off->head), off_cast(m, off->tail),
            off_cast(m, off->overflow),
            Slice<io_uring_cqe>::init(
                p->cq_entries, reinterpret_cast<io_uring_cqe*>(off_cast(m, off->cqes))
            )
        };
    }

    void deinit() {}
};

struct IoUring {
    PIN_STRUCT(
        IoUring, io_fd, Fd(static_cast<u32>(-1)), features, 0_u32, setup_flags, 0_u32, sq,
        SubmissionQueue(), cq, CompletionQueue(), wait_for_sqe, true, registered_by_default, true
    )

    static bool is_supported(u32 features) {
        // since kernel 6.3
        return features & IORING_FEAT_REG_REG_RING &&
               // since kernel 5.19
               features & IORING_FEAT_NODROP &&
               // since kernel 5.17
               features & IORING_FEAT_CQE_SKIP &&
               // since kernel 5.13
               features & IORING_FEAT_RSRC_TAGS &&
               // since kernel 5.11
               features & IORING_FEAT_SQPOLL_NONFIXED &&
               // since kernel 5.7
               features & IORING_FEAT_FAST_POLL &&
               // since kernel 5.4
               features & IORING_FEAT_SINGLE_MMAP;
    }

    SysRes<usize> close_fd(this IoUring& self) {
        return VORTEX_NS(close(self.io_fd));
    }

    static SysRes<IoUring> init(
        u32 entries, u32 flags = 0, u32 thread_idle = 3000, u32 thread_cpu = 0,
        bool wait_for_sqe = true, bool registered_by_default = true
    ) {
        if (flags & IORING_SETUP_CQE32) return SysResKind::NOSYS;

        IoUring self;
        entries          = ceilPowerOfTwo(entries);
        auto p           = zeroed<io_uring_params>();
        p.flags          = flags;
        p.sq_thread_cpu  = thread_cpu;
        p.sq_thread_idle = thread_idle;

        self.io_fd       = TRY(io_uring_setup(entries, &p));
        if (!is_supported(p.features)) {
            self.close_fd();
            return SysResKind::NOSYS;
        }

        SysRes<SubmissionQueue> sq_r = SubmissionQueue::init(self.io_fd, &p);
        if (sq_r.is_err()) {
            self.close_fd();
            return ErrSysRes(sq_r.kind);
        }

        self.setup_flags           = flags;
        self.features              = p.features;
        self.wait_for_sqe          = wait_for_sqe;
        self.sq                    = move(sq_r.res);
        self.registered_by_default = registered_by_default;
        self.cq                    = CompletionQueue::init(&p, &self.sq);

        io_uring_rsrc_update u     = {
                .offset = -1U,
                .resv   = 0,
                .data   = static_cast<u64>(self.io_fd),
        };
        SysRes<usize> register_r = io_uring_register(self.io_fd, IORING_REGISTER_RING_FDS, &u, 1);
        if (register_r.is_err()) {
            self.close_fd();
            return ErrSysRes(register_r.kind);
        }

        return self;
    }

    SysRes<usize> enter(this IoUring&, u32 to_submit, u32 min_complete, u32 flags) {
        return io_uring_enter(
            0, to_submit, min_complete, flags | IORING_ENTER_REGISTERED_RING, null, NSIG / 8
        );
    }
    template <typename T>
    SysRes<usize> resource_register(this IoUring&, u32 opcode, T* arg, u32 nr_args) {
        return io_uring_register(
            0, opcode | IORING_REGISTER_USE_REGISTERED_RING, reinterpret_cast<void*>(arg), nr_args
        );
    }

    SysRes<None> deinit(this IoUring& self) {
        TRY(self.sq.deinit());
        TRY(self.close_fd());

        return None();
    }

    u32 sq_ready(this IoUring& self) {
        return self.sq.sqe_tail - __atomic_load_n(self.sq.head, __ATOMIC_ACQUIRE);
    }
    u32 flush_sq(this IoUring& self) {
        if (self.sq.sqe_head != self.sq.sqe_tail) {
            u32 to_submit = self.sq.sqe_tail - self.sq.sqe_head;
            u32 tail      = *self.sq.tail;
            usize i       = 0;
            while (i < to_submit) {
                self.sq.array.ptr [tail & self.sq.mask] = self.sq.sqe_head & self.sq.mask;
                tail += 1;
                self.sq.sqe_head += 1;
                i += 1;
            }

            __atomic_store_n(self.sq.tail, tail, __ATOMIC_RELEASE);
        }

        return self.sq_ready();
    }
    bool cq_ring_needs_flush(this IoUring& self) {
        return (__atomic_load_n(self.sq.flags, __ATOMIC_RELAXED) &
                (IORING_SQ_CQ_OVERFLOW | IORING_SQ_TASKRUN)) != 0;
    }
    bool cq_ring_needs_enter(this IoUring& self) {
        return (self.setup_flags & IORING_SETUP_IOPOLL) || self.cq_ring_needs_flush();
    }
    bool sq_ring_needs_enter(this IoUring& self, u32* flags) {
        if ((self.setup_flags & IORING_SETUP_SQPOLL) == 0) return true;

        // Ensure the kernel can see self.sq.tail before we read the flags.
        __atomic_thread_fence(__ATOMIC_SEQ_CST);

        if ((__atomic_load_n(self.sq.flags, __ATOMIC_RELAXED) & IORING_SQ_NEED_WAKEUP) != 0)
            [[unlikely]] {
            *flags |= IORING_ENTER_SQ_WAKEUP;
            return true;
        }

        return false;
    }
    u32 cq_ready(this IoUring& self) {
        return __atomic_load_n(self.cq.tail, __ATOMIC_ACQUIRE) - *self.cq.head;
    }
    void cq_advance(this IoUring& self, u32 count) {
        if (count > 0) __atomic_store_n(self.cq.head, *self.cq.head + count, __ATOMIC_RELEASE);
    }

    SysRes<usize> submit_and_wait(this IoUring& self, u32 wait_nr) {
        u32 flags  = 0;
        u32 submit = self.flush_sq();
        if (self.sq_ring_needs_enter(&flags) || wait_nr > 0) {
            if (wait_nr > 0 || self.cq_ring_needs_enter()) flags |= IORING_ENTER_GETEVENTS;

            return self.enter(submit, wait_nr, flags);
        }

        return submit;
    }
    SysRes<usize> submit(this IoUring& self) {
        return self.submit_and_wait(0);
    }

    usize copy_cqes_ready(this IoUring& self, Slice<IoUringRes>* cqes) {
        u32 ready = self.cq_ready();
        u32 count = min(static_cast<u32>(cqes->len), ready);
        u32 head  = *self.cq.head & self.cq.mask;

        // Before wrapping.
        u32 n     = min(static_cast<u32>(self.cq.cqes.len) - head, count);
        for (usize i = 0; i < n; i++) {
            io_uring_cqe e = self.cq.cqes.ptr [head + i];
            IoUringRes r   = CqeRes::from_cqe(e);
            cqes->ptr [i]  = move(r);
        }

        if (count > n) {
            // Wrap self.cq.cqes.
            u32 w = count - n;
            for (usize i = 0; i < w; i++)
                cqes->ptr [n + i] = move(CqeRes::from_cqe(self.cq.cqes.ptr [i]));
        }

        self.cq_advance(count);
        return static_cast<usize>(count);
    }
    template <typename T>
        requires is_integral_v<T>
    SysRes<usize> copy_cqes(this IoUring& self, Slice<IoUringRes>* cqes, T wait_nr) {
        usize count = self.copy_cqes_ready(cqes);
        if (count > 0) return count;

        if (self.cq_ring_needs_flush() or wait_nr > 0) {
            u32 flags = IORING_ENTER_GETEVENTS;
            self.sq_ring_needs_enter(&flags);
            TRY(self.enter(0, static_cast<u32>(wait_nr), flags));
            return self.copy_cqes_ready(cqes);
        }

        return 0;
    }
    SysRes<IoUringRes> copy_cqe(this IoUring& self) {
        IoUringRes cqes [1];
        Slice<IoUringRes> cqes_slice = Slice<IoUringRes>(1, cqes);
        while (true) {
            auto count = TRY(self.copy_cqes(&cqes_slice, 1));

            if (count > 0) return move(cqes [0]);
        }
    }

    SysRes<usize>
    register_files(this IoUring& self, Slice<i32>* files, const Slice<const u64>* tags) {
        io_uring_rsrc_register u = {
            .nr    = static_cast<u32>(files->len),
            .flags = 0,
            .resv2 = 0,
            .data  = reinterpret_cast<u64>(files->ptr),
            .tags  = reinterpret_cast<u64>(tags->ptr),
        };
        return self.resource_register(IORING_REGISTER_FILES2, &u, sizeof(io_uring_rsrc_register));
    }
    SysRes<usize> register_sparse_files(this IoUring& self, u32 nr_files) {
        io_uring_rsrc_register u = {
            .nr    = nr_files,
            .flags = IORING_RSRC_REGISTER_SPARSE,
            .resv2 = 0,
            .data  = 0,
            .tags  = 0_u64,
        };
        return self.resource_register(IORING_REGISTER_FILES2, &u, sizeof(io_uring_rsrc_register));
    }
    SysRes<usize> register_file(this IoUring& self, i32 file, u64 tag) {
        i32 files [1]          = {file};
        u64 tags [1]           = {tag};
        Slice<i32> file_s      = Slice<i32>::init(1, files);
        Slice<const u64> tag_s = Slice<const u64>::init(1, tags);
        return self.register_files(&file_s, &tag_s);
    }
    SysRes<usize>
    update_files(this IoUring& self, u32 off, Slice<i32>* files, const Slice<const u64>* tags) {
        io_uring_rsrc_update2 u = {
            .offset = off,
            .resv   = 0,
            .data   = reinterpret_cast<u64>(files->ptr),
            .tags   = reinterpret_cast<u64>(tags->ptr),
            .nr     = static_cast<u32>(files->len),
            .resv2  = 0,
        };
        return self.resource_register(
            IORING_REGISTER_FILES_UPDATE2, &u, sizeof(io_uring_rsrc_update2)
        );
    }
    SysRes<usize> update_file(this IoUring& self, u32 off, i32 file, u64 tag) {
        i32 files [1]          = {file};
        u64 tags [1]           = {tag};
        Slice<i32> file_s      = Slice<i32>::init(1, files);
        Slice<const u64> tag_s = Slice<const u64>::init(1, tags);
        return self.update_files(off, &file_s, &tag_s);
    }
    SysRes<usize>
    register_buffers(this IoUring& self, Slice<iovec>* buffers, const Slice<const u64>* tags) {
        io_uring_rsrc_register u = {
            .nr    = static_cast<u32>(buffers->len),
            .flags = 0,
            .resv2 = 0,
            .data  = reinterpret_cast<u64>(buffers->ptr),
            .tags  = reinterpret_cast<u64>(tags->ptr),
        };
        return self.resource_register(IORING_REGISTER_BUFFERS2, &u, sizeof(io_uring_rsrc_register));
    }
    SysRes<usize> register_buffer(this IoUring& self, iovec buffer, u64 tag) {
        iovec buffers [1]      = {buffer};
        u64 tags [1]           = {tag};
        Slice<iovec> buffer_s  = Slice<iovec>::init(1, buffers);
        Slice<const u64> tag_s = Slice<const u64>::init(1, tags);
        return self.register_buffers(&buffer_s, &tag_s);
    }
    SysRes<io_uring_buf_ring*> register_pbuffer(
        this IoUring& self, u16 bgid, u32 entries, usize size_shift, bool incr = true
    ) {
        auto* buf          = reinterpret_cast<io_uring_buf_ring*>(TRY(mmap(
            null, entries * (sizeof(io_uring_buf) + (1U << size_shift)), PROT_READ | PROT_WRITE,
            MAP_ANONYMOUS | MAP_PRIVATE, 0, 0
        )));
        io_uring_buf_reg u = {
            .ring_addr    = reinterpret_cast<u64>(buf),
            .ring_entries = entries,
            .bgid         = bgid,
            .flags        = static_cast<u16>(incr ? IOU_PBUF_RING_INC : 0),
            .resv         = {0_u64, 0_u64, 0_u64},
        };
        TRY(self.resource_register(IORING_REGISTER_PBUF_RING, &u, 1));

        return buf;
    }
    SysRes<usize> unregister_pbuffer(this IoUring& self, u16 bgid) {
        io_uring_buf_reg u = {
            .ring_addr    = 0,
            .ring_entries = 0,
            .bgid         = bgid,
            .flags        = 0,
            .resv         = {0_u64, 0_u64, 0_u64},
        };
        return self.resource_register(IORING_UNREGISTER_PBUF_RING, &u, 1);
    }
    SysRes<usize> register_file_alloc_range(this IoUring& self, u32 off, u32 len) {
        io_uring_file_index_range u = {.off = off, .len = len, .resv = 0};
        return self.resource_register(IORING_REGISTER_FILE_ALLOC_RANGE, &u, 0);
    }

    io_uring_sqe*
    get_sqe(this IoUring& self, u8 op, u64 user_data, Fd fd = 0, bool allow_fixed_file = true) {
        for (;;) {
            u32 next = self.sq.sqe_tail + 1;
            u32 head = __atomic_load_n(self.sq.head, __ATOMIC_ACQUIRE);
            if ((next - head) > self.sq.sqes.len) [[unlikely]] {
                if (self.wait_for_sqe &&
                    self.enter(0, 0, IORING_ENTER_SQ_WAIT).kind == SysResKind::SUCCESS) [[likely]] {
                    continue;
                }

                return null;
            }

            io_uring_sqe* sqe =
                &self.sq.sqes.ptr [static_cast<usize>(self.sq.sqe_tail & self.sq.mask)];
            self.sq.sqe_tail = next;

            if (allow_fixed_file && self.registered_by_default) sqe->flags = IOSQE_FIXED_FILE;
            sqe->opcode    = op;
            sqe->fd        = fd;
            sqe->user_data = user_data;
            return sqe;
        }
    }

    io_uring_sqe* nop(this IoUring& self, u64 user_data) {
        return self.get_sqe(IORING_OP_NOP, user_data);
    }

    io_uring_sqe* pread(
        this IoUring& self, u64 user_data, Fd fd, void* buf, usize count, off_t offset,
        bool allow_fixed_file = true
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_READ, user_data, fd, allow_fixed_file);
        if (sqe == null) return sqe;
        sqe->addr = (u64)buf;
        sqe->len  = (u32)count;
        sqe->off  = (u64)offset;
        return sqe;
    }

    io_uring_sqe* read(
        this IoUring& self, u64 user_data, Fd fd, void* buf, usize count,
        bool allow_fixed_file = true
    ) {
        return self.pread(user_data, fd, buf, count, 0, allow_fixed_file);
    }

    io_uring_sqe* pread_multishot(
        this IoUring& self, u64 user_data, Fd fd, u16 bgid, off_t offset,
        bool allow_fixed_file = true
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_READ_MULTISHOT, user_data, fd, allow_fixed_file);
        if (sqe == null) return sqe;
        sqe->off       = (u64)offset;
        sqe->buf_group = bgid;
        sqe->flags |= IOSQE_BUFFER_SELECT;
        return sqe;
    }

    io_uring_sqe* read_multishot(
        this IoUring& self, u64 user_data, Fd fd, u16 bgid, bool allow_fixed_file = true
    ) {
        return self.pread_multishot(user_data, fd, bgid, allow_fixed_file);
    }

    io_uring_sqe*
    pwrite(this IoUring& self, u64 user_data, Fd fd, const void* buf, usize count, off_t offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WRITE, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr = (u64)buf;
        sqe->len  = (u32)count;
        sqe->off  = (u64)offset;
        return sqe;
    }

    io_uring_sqe* write(this IoUring& self, u64 user_data, Fd fd, const void* buf, usize count) {
        return self.pwrite(user_data, fd, buf, count, 0);
    }

    io_uring_sqe* preadv2(
        this IoUring& self, u64 user_data, Fd fd, const struct iovec* iov, u32 iovcnt, off_t offset,
        rwf_t flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_READV, user_data, fd);
        if (sqe == null) return sqe;
        sqe->rw_flags = (__kernel_rwf_t)flags;
        sqe->len      = iovcnt;
        sqe->addr     = (u64)iov;
        sqe->off      = (u64)offset;
        return sqe;
    }

    io_uring_sqe* preadv(
        this IoUring& self, u64 user_data, Fd fd, const struct iovec* iov, u32 iovcnt, off_t offset
    ) {
        return self.preadv2(user_data, fd, iov, iovcnt, offset, 0);
    }

    io_uring_sqe*
    readv(this IoUring& self, u64 user_data, Fd fd, const struct iovec* iov, u32 iovcnt) {
        return self.preadv2(user_data, fd, iov, iovcnt, 0, 0);
    }

    io_uring_sqe* pwritev2(
        this IoUring& self, u64 user_data, Fd fd, const struct iovec* iov, u32 iovcnt, off_t offset,
        rwf_t flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WRITEV, user_data, fd);
        if (sqe == null) return sqe;
        sqe->rw_flags = (__kernel_rwf_t)flags;
        sqe->len      = iovcnt;
        sqe->addr     = (u64)iov;
        sqe->off      = (u64)offset;
        return sqe;
    }

    io_uring_sqe* pwritev(
        this IoUring& self, u64 user_data, Fd fd, const struct iovec* iov, u32 iovcnt, off_t offset
    ) {
        return self.pwritev2(user_data, fd, iov, iovcnt, offset, 0);
    }

    io_uring_sqe*
    writev(this IoUring& self, u64 user_data, Fd fd, const struct iovec* iov, u32 iovcnt) {
        return self.pwritev2(user_data, fd, iov, iovcnt, 0, 0);
    }

    io_uring_sqe* fsync(this IoUring& self, u64 user_data, Fd fd) {
        return self.get_sqe(IORING_OP_FSYNC, user_data, fd);
    }

    io_uring_sqe* poll_add(this IoUring& self, u64 user_data, Fd fd, u32 poll_mask) {
        static_assert(__BYTE_ORDER == __LITTLE_ENDIAN, "Unsupported endianness!");

        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_ADD, user_data, fd);
        if (sqe == null) return sqe;
        sqe->poll32_events = poll_mask;
        return sqe;
    }

    io_uring_sqe* poll_add_multishot(this IoUring& self, u64 user_data, Fd fd, u32 poll_mask) {
        io_uring_sqe* sqe = self.poll_add(user_data, fd, poll_mask);
        if (sqe == null) return sqe;
        sqe->len = IORING_POLL_ADD_MULTI;
        return sqe;
    }

    io_uring_sqe* poll_remove(this IoUring& self, u64 user_data) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_REMOVE, user_data, -1);
        if (sqe == null) return sqe;
        sqe->addr = user_data;
        sqe->len  = IORING_POLL_ADD_MULTI;
        return sqe;
    }

    io_uring_sqe* poll_update(
        this IoUring& self, u64 old_user_data, u64 new_user_data, u32 poll_mask, u32 flags
    ) {
        static_assert(__BYTE_ORDER == __LITTLE_ENDIAN, "Unsupported endianness!");

        io_uring_sqe* sqe = self.get_sqe(IORING_OP_POLL_REMOVE, new_user_data, -1);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr          = old_user_data;
        sqe->len           = flags;
        sqe->off           = new_user_data;
        sqe->poll32_events = poll_mask;
        return sqe;
    }

    io_uring_sqe*
    epoll_ctl(this IoUring& self, u64 user_data, i32 epfd, Fd fd, i32 op, struct epoll_event* ev) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_EPOLL_CTL, user_data, epfd);
        if (sqe == null) return sqe;
        sqe->off  = (u64)fd;
        sqe->len  = (u32)op;
        sqe->addr = (u64)ev;
        return sqe;
    }

    io_uring_sqe*
    sync_file_range(this IoUring& self, u64 user_data, Fd fd, usize len, off_t offset, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SYNC_FILE_RANGE, user_data, fd);
        if (sqe == null) return sqe;
        sqe->sync_range_flags = flags;
        sqe->len              = (u32)len;
        sqe->off              = (u64)offset;
        return sqe;
    }

    io_uring_sqe* sendmsg(this IoUring& self, u64 user_data, Fd fd, const msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SENDMSG, user_data, fd);
        if (sqe == null) return sqe;
        sqe->len       = 1;
        sqe->msg_flags = flags;
        sqe->addr      = (u64)msg;
        return sqe;
    }

    io_uring_sqe* accept(
        this IoUring& self, u64 user_data, Fd fd, sockaddr* addr, socklen_t* addrlen, int flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ACCEPT, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr         = (u64)addr;
        sqe->addr2        = (u64)addrlen;
        sqe->accept_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    connect(this IoUring& self, u64 user_data, Fd sockfd, const sockaddr* addr, socklen_t addrlen) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_CONNECT, user_data, sockfd);
        if (sqe == null) return sqe;
        sqe->addr = reinterpret_cast<u64>(addr);
        if (sqe == null) return sqe;
        sqe->off = addrlen;
        return sqe;
    }

    io_uring_sqe* shutdown(this IoUring& self, u64 user_data, Fd fd, int how) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SHUTDOWN, user_data, fd);
        if (sqe == null) return sqe;
        sqe->len = (u32)how;
        return sqe;
    }

    io_uring_sqe* renameat(
        this IoUring& self, u64 user_data, int olddfd, const char* oldpath, int newdfd,
        const char* newpath, int flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RENAMEAT, user_data, olddfd);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr         = (u64)path;
        sqe->unlink_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    openat(this IoUring& self, u64 user_data, int dfd, const char* path, int flags, mode_t mode) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_OPENAT, user_data, dfd);
        if (sqe == null) return sqe;
        sqe->addr       = (u64)path;
        sqe->len        = (u32)mode;
        sqe->open_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* openat2(
        this IoUring& self, u64 user_data, int dfd, const char* path, open_how* oh,
        bool file_index_alloc = true
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_OPENAT2, user_data, dfd, false);
        if (sqe == null) return sqe;
        sqe->off  = (u64)oh;
        sqe->addr = (u64)path;
        sqe->len  = sizeof(open_how);
        if (file_index_alloc) sqe->file_index |= IORING_FILE_INDEX_ALLOC;
        return sqe;
    }

    io_uring_sqe* statx(
        this IoUring& self, u64 user_data, int dfd, const char* path, int flags, unsigned mask,
        statx_t* statxbuf
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_STATX, user_data, dfd);
        if (sqe == null) return sqe;
        sqe->addr        = (u64)path;
        sqe->len         = mask;
        sqe->addr2       = (u64)statxbuf;
        sqe->statx_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* splice(
        this IoUring& self, u64 user_data, Fd fd_in, i64 off_in, Fd fd_out, i64 off_out, u32 nbytes,
        unsigned int splice_flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SPLICE, user_data, fd_out);
        if (sqe == null) return sqe;
        sqe->addr          = 0;
        sqe->len           = nbytes;
        sqe->off           = (u64)off_out;
        sqe->splice_off_in = (u64)off_in;
        sqe->splice_fd_in  = fd_in;
        sqe->splice_flags  = splice_flags;
        return sqe;
    }

    io_uring_sqe*
    tee(this IoUring& self, u64 user_data, Fd fd_in, Fd fd_out, u32 nbytes,
        unsigned int splice_flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TEE, user_data, fd_out);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr          = (u64)ts;
        sqe->len           = 1;
        sqe->off           = count;
        sqe->timeout_flags = flags;
        return sqe;
    }

    io_uring_sqe*
    timeout_remove(this IoUring& self, u64 user_data, u64 timeout_user_data, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TIMEOUT_REMOVE, user_data);
        if (sqe == null) return sqe;
        sqe->addr          = timeout_user_data;
        sqe->timeout_flags = flags;
        return sqe;
    }

    io_uring_sqe* timeout_update(
        this IoUring& self, u64 user_data, __kernel_timespec* ts, u64 user_data_to_update, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_TIMEOUT_REMOVE, user_data);
        if (sqe == null) return sqe;
        sqe->addr          = user_data_to_update;
        sqe->off           = (u64)ts;
        sqe->timeout_flags = flags | IORING_TIMEOUT_UPDATE;
        return sqe;
    }

    io_uring_sqe*
    link_timeout(this IoUring& self, u64 user_data, __kernel_timespec* ts, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_LINK_TIMEOUT, user_data);
        if (sqe == null) return sqe;
        sqe->addr          = (u64)ts;
        sqe->len           = 1;
        sqe->timeout_flags = flags;
        return sqe;
    }

    io_uring_sqe* files_update(this IoUring& self, u64 user_data, Slice<Fd>* files, Fd offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FILES_UPDATE, user_data, 0);
        if (sqe == null) return sqe;
        sqe->addr = (u64)files->ptr;
        sqe->len  = (u32)files->len;
        sqe->off  = (u64)offset;
        return sqe;
    }

    io_uring_sqe*
    fallocate(this IoUring& self, u64 user_data, Fd fd, int mode, i64 offset, u64 len) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FALLOCATE, user_data, fd);
        if (sqe == null) return sqe;
        sqe->len  = (u32)mode;
        sqe->off  = (u64)offset;
        sqe->addr = len;
        return sqe;
    }

    io_uring_sqe*
    bind(this IoUring& self, u64 user_data, Fd fd, sockaddr* addr, socklen_t addrlen) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_BIND, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr  = (u64)addr;
        sqe->addr2 = 0;
        sqe->off   = addrlen;
        return sqe;
    }

    io_uring_sqe* listen(this IoUring& self, u64 user_data, Fd fd, int backlog) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_LISTEN, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr = 0;
        sqe->len  = (u32)backlog;
        return sqe;
    }

    io_uring_sqe*
    recv(this IoUring& self, u64 user_data, Fd fd, void* buf, usize count, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RECV, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr      = (u64)buf;
        sqe->len       = (u32)count;
        sqe->msg_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* recvmsg(this IoUring& self, u64 user_data, Fd fd, msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RECVMSG, user_data, fd);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr      = (u64)addr;
        sqe->len       = (u32)len;
        sqe->off       = (u64)bid;
        sqe->buf_group = (u16)bgid;
        return sqe;
    }

    io_uring_sqe* remove_buffers(this IoUring& self, u64 user_data, i32 nr, i32 bgid) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_REMOVE_BUFFERS, user_data, nr);
        if (sqe == null) return sqe;
        sqe->buf_group = (u16)bgid;
        return sqe;
    }

    io_uring_sqe*
    sync_file_range(this IoUring& self, u64 user_data, Fd fd, u32 len, u64 offset, int flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SYNC_FILE_RANGE, user_data, fd);
        if (sqe == null) return sqe;
        sqe->len              = len;
        sqe->off              = offset;
        sqe->sync_range_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    socket(this IoUring& self, u64 user_data, int domain, int type, int protocol, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SOCKET, user_data, domain);
        if (sqe == null) return sqe;
        sqe->off      = (u64)type;
        sqe->len      = (u32)protocol;
        sqe->rw_flags = (__kernel_rwf_t)flags;
        return sqe;
    }

    io_uring_sqe* getxattr(
        this IoUring& self, u64 user_data, const char* name, char* value, const char* path, u32 len
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_GETXATTR, user_data);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->addr3       = (u64)path;
        sqe->len         = len;
        sqe->xattr_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    fgetxattr(this IoUring& self, u64 user_data, Fd fd, const char* name, char* value, u32 len) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FGETXATTR, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->len         = len;
        sqe->xattr_flags = 0;
        return sqe;
    }

    io_uring_sqe* fsetxattr(
        this IoUring& self, u64 user_data, Fd fd, const char* name, const char* value, int flags,
        u32 len
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FSETXATTR, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr        = (u64)name;
        sqe->addr2       = (u64)value;
        sqe->len         = len;
        sqe->xattr_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe*
    msg_ring(this IoUring& self, u64 user_data, int fd, u32 len, u64 data, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MSG_RING, user_data, fd);
        if (sqe == null) return sqe;
        sqe->len            = len;
        sqe->off            = data;
        sqe->msg_ring_flags = flags;
        return sqe;
    }

    io_uring_sqe* msg_ring_fd(
        this IoUring& self, u64 user_data, int fd, int source_fd, int target_fd, u64 data, u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MSG_RING, user_data, fd);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr         = cancel_user_data;
        sqe->cancel_flags = (u32)flags;
        return sqe;
    }

    io_uring_sqe* cancel_fd(this IoUring& self, u64 user_data, Fd fd, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ASYNC_CANCEL, user_data, fd);
        if (sqe == null) return sqe;
        sqe->cancel_flags = flags | IORING_ASYNC_CANCEL_FD;
        return sqe;
    }

    io_uring_sqe* fixed_fd_install(this IoUring& self, u64 user_data, Fd fd, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FIXED_FD_INSTALL, user_data, fd);
        if (sqe == null) return sqe;
        sqe->flags            = IOSQE_FIXED_FILE;
        sqe->install_fd_flags = flags;
        return sqe;
    }

    io_uring_sqe* ftruncate(this IoUring& self, u64 user_data, Fd fd, i64 offset) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_FTRUNCATE, user_data, fd);
        if (sqe == null) return sqe;
        sqe->off = (u64)offset;
        return sqe;
    }

    io_uring_sqe* madvise(this IoUring& self, u64 user_data, void* addr, u32 len, int advice) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_MADVISE, user_data);
        if (sqe == null) return sqe;
        sqe->addr           = (u64)addr;
        sqe->len            = len;
        sqe->fadvise_advice = (u32)advice;
        return sqe;
    }

    io_uring_sqe* send_zc(
        this IoUring& self, u64 user_data, Fd sockfd, const void* buf, usize len, int flags = 0,
        u16 zc_flags = 0
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SEND_ZC, user_data, sockfd);
        if (sqe == null) return sqe;
        sqe->addr      = (u64)buf;
        sqe->len       = (u32)len;
        sqe->msg_flags = (u32)flags;
        sqe->ioprio    = zc_flags;
        return sqe;
    }

    io_uring_sqe*
    sendmsg_zc(this IoUring& self, u64 user_data, Fd fd, const msghdr* msg, u32 flags) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_SENDMSG_ZC, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr      = (u64)msg;
        sqe->len       = 1;
        sqe->msg_flags = flags;
        return sqe;
    }

    io_uring_sqe*
    recv_multishot(this IoUring& self, u64 user_data, Fd sockfd, u16 bgid, int flags = 0) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_RECV, user_data, sockfd);
        if (sqe == null) return sqe;
        sqe->msg_flags = (u32)flags;
        sqe->buf_group = bgid;
        sqe->flags |= IOSQE_BUFFER_SELECT;
        sqe->ioprio |= IORING_RECV_MULTISHOT;
        return sqe;
    }

    io_uring_sqe* accept_multishot(
        this IoUring& self, u64 user_data, Fd fd, sockaddr* addr, socklen_t* addrlen, int flags = 0,
        bool file_index_alloc = true
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_ACCEPT, user_data, fd);
        if (sqe == null) return sqe;
        sqe->addr         = (u64)addr;
        sqe->addr2        = (u64)addrlen;
        sqe->accept_flags = (u32)flags;
        sqe->ioprio |= IORING_ACCEPT_MULTISHOT;
        if (file_index_alloc) sqe->file_index |= IORING_FILE_INDEX_ALLOC;
        return sqe;
    }

    io_uring_sqe* waitid(
        this IoUring& self, u64 user_data, int idtype, Fd id, siginfo_t* infop, int options,
        u32 flags
    ) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_WAITID, user_data, id);
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
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
        if (sqe == null) return sqe;
        sqe->addr        = (u64)futex;
        sqe->len         = nr_futex;
        sqe->futex_flags = flags;
        return sqe;
    }

    io_uring_sqe* close(this IoUring& self, u64 user_data, Fd fd) {
        return self.get_sqe(IORING_OP_CLOSE, user_data, fd);
    }

    io_uring_sqe* close_direct(this IoUring& self, u64 user_data, u32 file_index) {
        io_uring_sqe* sqe = self.get_sqe(IORING_OP_CLOSE, user_data, 0, false);
        if (sqe == null) return sqe;
        sqe->file_index = file_index + 1;
        return sqe;
    }

    io_uring_sqe* link(this IoUring&, io_uring_sqe* sqe) {
        sqe->flags |= IOSQE_IO_LINK;
        return sqe;
    }
    io_uring_sqe* skip_success(this IoUring&, io_uring_sqe* sqe) {
        sqe->flags |= IOSQE_CQE_SKIP_SUCCESS;
        return sqe;
    }
};

struct IoUringBufRing {
    PIN_STRUCT(
        IoUringBufRing, entries, 0_u32, bgid, 0_u16, br, reinterpret_cast<io_uring_buf_ring*>(0),
        buffer_base, reinterpret_cast<u8*>(0), size_shift, 0_usize, inc_pos, 0_usize, inc_bid,
        32769_u16
    )

    static constexpr u16 NO_BUF = 32769;

    static SysRes<IoUringBufRing> init(IoUring* r, u16 bgid, u32 count, usize size_shift = 12) {
        IoUringBufRing self;
        self.entries = count;
        if (!isPowerOfTwo(self.entries) || self.entries > 32768) [[unlikely]] {
            return SysResKind::RANGE;
        }

        u32 bucket_size  = 1U << size_shift;
        self.br          = TRY(r->register_pbuffer(bgid, self.entries, size_shift, true));
        self.br->tail    = 0;
        self.bgid        = bgid;
        self.size_shift  = size_shift;
        self.inc_pos     = 0;
        self.inc_bid     = 0;
        self.buffer_base = reinterpret_cast<u8*>(self.br + sizeof(io_uring_buf) * self.entries);

        for (u16 i = 0; i < self.entries; i++) self.add_buf(self.get_buf(i), bucket_size, i);
        self.advance_buffers(static_cast<u16>(self.entries));

        return self;
    }

    void advance_buffers(this IoUringBufRing& self, u16 count) {
        __atomic_store_n(&self.br->tail, self.br->tail + count, __ATOMIC_RELEASE);
    }
    void add_buf(this IoUringBufRing& self, u8* addr, u32 len, u16 bid, usize off) {
        self.br->bufs [(self.br->tail + off) & (self.entries - 1)] = {
            .addr = reinterpret_cast<u64>(addr),
            .len  = len,
            .bid  = bid,
            .resv = 0,
        };
    }
    void add_buf(this IoUringBufRing& self, u8* addr, u32 len, u16 bid) {
        return self.add_buf(addr, len, bid, bid);
    }

    void cq_advance(this IoUringBufRing& self, IoUring* r, u16 count) {
        self.advance_buffers(count);
        r->cq_advance(static_cast<u32>(count));
    }

    SysRes<u32> head(this IoUringBufRing& self, IoUring* r) {
        io_uring_buf_status u = {
            .buf_group = static_cast<u32>(self.bgid),
            .head      = 0,
            .resv      = {0_u32, 0_u32, 0_u32, 0_u32, 0_u32, 0_u32, 0_u32, 0_u32}
        };
        TRY(r->resource_register(IORING_REGISTER_PBUF_STATUS, &u, 1));

        return u.head;
    }

    SysRes<u16> available(this IoUringBufRing& self, IoUring* r) {
        return self.br->tail - static_cast<u16>(TRY(self.head(r)));
    }
    u32 bucket_size(this IoUringBufRing& self) {
        return 1U << self.size_shift;
    }
    u16 index(this IoUringBufRing&, const CqeRes& cqe) {
        return cqe.flags >> 16;
    }
    bool bid_in_use(this IoUringBufRing&, const CqeRes& cqe) {
        return cqe.flags & IORING_CQE_F_BUF_MORE;
    }

    u8* get_buf(this IoUringBufRing& self, u16 idx) {
        return reinterpret_cast<u8*>(self.buffer_base + (idx << self.size_shift));
    }
    Slice<u8> get_buf(this IoUringBufRing& self, const CqeRes& cqe) {
        return Slice<u8>::init(cqe.res, self.get_buf(self.index(cqe)) + self.inc_pos);
    }

    void recycle_buf(this IoUringBufRing& self, const CqeRes& cqe) {
        u16 bid_idx = self.index(cqe);
        if (!self.bid_in_use(cqe)) {
            self.inc_pos = 0;
            self.inc_bid = NO_BUF;
            self.add_buf(self.get_buf(bid_idx), self.bucket_size(), bid_idx, 0);
            return self.advance_buffers(1);
        }

        if (self.inc_bid == NO_BUF) self.inc_bid = bid_idx;

        self.inc_pos += cqe.res;
    }

    SysRes<None> deinit(this IoUringBufRing& self, IoUring* r) {
        TRY(r->unregister_pbuffer(self.bgid));
        TRY(munmap(self.br, self.entries * (sizeof(io_uring_buf) + self.bucket_size())));

        return None();
    }
};

DIAG_IGNORE_CLANG_POP
