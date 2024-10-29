// Warning: This file is auto generated. Don't edit this file directly!

#pragma once
#include "syscall_impl.hpp"
#include "wrapperHelper.hpp"

// NOLINTBEGIN

static SysRes<usize> io_setup(unsigned nr_reqs, aio_context_t *ctx) {
    return syscall(__NR_io_setup, nr_reqs, ctx);
}
static SysRes<usize> io_destroy(aio_context_t ctx) {
    return syscall(__NR_io_destroy, ctx);
}
static SysRes<usize> io_submit(aio_context_t a, long b, struct iocb **c) {
    return syscall(__NR_io_submit, a, b, c);
}
static SysRes<usize> io_cancel(aio_context_t ctx_id, struct iocb *iocb, struct io_event *result) {
    return syscall(__NR_io_cancel, ctx_id, iocb, result);
}
static SysRes<usize> io_getevents(
    aio_context_t ctx_id, long min_nr, long nr, struct io_event *events,
    struct __kernel_timespec *timeout
) {
    return syscall(__NR_io_getevents, ctx_id, min_nr, nr, events, timeout);
}
static SysRes<usize> io_pgetevents(
    aio_context_t ctx_id, long min_nr, long nr, struct io_event *events,
    struct __kernel_timespec *timeout, const __aio_sigset *sig
) {
    return syscall(__NR_io_pgetevents, ctx_id, min_nr, nr, events, timeout, sig);
}
static SysRes<usize> io_uring_setup(u32 entries, struct io_uring_params *p) {
    return syscall(__NR_io_uring_setup, entries, p);
}
static SysRes<usize>
io_uring_enter(FdU fd, u32 to_submit, u32 min_complete, u32 flags, const void *argp, usize argsz) {
    return syscall(__NR_io_uring_enter, fd, to_submit, min_complete, flags, argp, argsz);
}
static SysRes<usize> io_uring_register(FdU fd, unsigned int op, void *arg, unsigned int nr_args) {
    return syscall(__NR_io_uring_register, fd, op, arg, nr_args);
}
static SysRes<usize>
setxattr(const char *path, const char *name, const void *value, usize size, int flags) {
    return syscall(__NR_setxattr, path, name, value, size, flags);
}
static SysRes<usize>
lsetxattr(const char *path, const char *name, const void *value, usize size, int flags) {
    return syscall(__NR_lsetxattr, path, name, value, size, flags);
}
static SysRes<usize> fsetxattr(FdI fd, const char *name, const void *value, usize size, int flags) {
    return syscall(__NR_fsetxattr, fd, name, value, size, flags);
}
static SysRes<usize> getxattr(const char *path, const char *name, void *value, usize size) {
    return syscall(__NR_getxattr, path, name, value, size);
}
static SysRes<usize> lgetxattr(const char *path, const char *name, void *value, usize size) {
    return syscall(__NR_lgetxattr, path, name, value, size);
}
static SysRes<usize> fgetxattr(FdI fd, const char *name, void *value, usize size) {
    return syscall(__NR_fgetxattr, fd, name, value, size);
}
static SysRes<usize> listxattr(const char *path, char *list, usize size) {
    return syscall(__NR_listxattr, path, list, size);
}
static SysRes<usize> llistxattr(const char *path, char *list, usize size) {
    return syscall(__NR_llistxattr, path, list, size);
}
static SysRes<usize> flistxattr(FdI fd, char *list, usize size) {
    return syscall(__NR_flistxattr, fd, list, size);
}
static SysRes<usize> removexattr(const char *path, const char *name) {
    return syscall(__NR_removexattr, path, name);
}
static SysRes<usize> lremovexattr(const char *path, const char *name) {
    return syscall(__NR_lremovexattr, path, name);
}
static SysRes<usize> fremovexattr(FdI fd, const char *name) {
    return syscall(__NR_fremovexattr, fd, name);
}
static SysRes<char *> getcwd(char *buf, unsigned long size) {
    return syscall(__NR_getcwd, buf, size).unsafe_cast<char *>();
}
static SysRes<usize> eventfd2(unsigned int count, int flags) {
    return syscall(__NR_eventfd2, count, flags);
}
static SysRes<usize> epoll_create1(int flags) {
    return syscall(__NR_epoll_create1, flags);
}
static SysRes<usize> epoll_ctl(FdI epfd, int op, FdI fd, struct epoll_event *event) {
    return syscall(__NR_epoll_ctl, epfd, op, fd, event);
}
static SysRes<usize> epoll_pwait2(
    FdI epfd, struct epoll_event *events, int maxevents, const struct __kernel_timespec *timeout,
    const sigset_t *sigmask, usize sigsetsize
) {
    return syscall(__NR_epoll_pwait2, epfd, events, maxevents, timeout, sigmask, sigsetsize);
}
static SysRes<usize> dup3(FdU oldfd, FdU newfd, int flags) {
    return syscall(__NR_dup3, oldfd, newfd, flags);
}
static SysRes<usize> fcntl(FdU fd, unsigned int cmd, unsigned long arg) {
    return syscall(__NR_fcntl, fd, cmd, arg);
}
static SysRes<usize> inotify_init1(int flags) {
    return syscall(__NR_inotify_init1, flags);
}
static SysRes<usize> inotify_add_watch(FdI fd, const char *path, u32 mask) {
    return syscall(__NR_inotify_add_watch, fd, path, mask);
}
static SysRes<usize> inotify_rm_watch(FdI fd, __s32 wd) {
    return syscall(__NR_inotify_rm_watch, fd, wd);
}
static SysRes<usize> ioctl(FdU fd, unsigned int cmd, unsigned long arg) {
    return syscall(__NR_ioctl, fd, cmd, arg);
}
static SysRes<usize> ioprio_set(int which, int who, int ioprio) {
    return syscall(__NR_ioprio_set, which, who, ioprio);
}
static SysRes<usize> ioprio_get(int which, int who) {
    return syscall(__NR_ioprio_get, which, who);
}
static SysRes<usize> flock(FdU fd, unsigned int cmd) {
    return syscall(__NR_flock, fd, cmd);
}
static SysRes<usize> mknodat(FdI dfd, const char *filename, umode_t mode, unsigned dev) {
    return syscall(__NR_mknodat, dfd, filename, mode, dev);
}
static SysRes<usize> mkdirat(FdI dfd, const char *pathname, umode_t mode) {
    return syscall(__NR_mkdirat, dfd, pathname, mode);
}
static SysRes<usize> unlinkat(FdI dfd, const char *pathname, int flag) {
    return syscall(__NR_unlinkat, dfd, pathname, flag);
}
static SysRes<usize> symlinkat(const char *oldname, FdI newdfd, const char *newname) {
    return syscall(__NR_symlinkat, oldname, newdfd, newname);
}
static SysRes<usize>
linkat(FdI olddfd, const char *oldname, FdI newdfd, const char *newname, int flags) {
    return syscall(__NR_linkat, olddfd, oldname, newdfd, newname, flags);
}
static SysRes<usize> renameat(FdI olddfd, const char *oldname, FdI newdfd, const char *newname) {
    return syscall(__NR_renameat, olddfd, oldname, newdfd, newname);
}
static SysRes<usize> umount(char *name, int flags) {
    return syscall(__NR_umount2, name, flags);
}
static SysRes<usize>
mount(char *dev_name, char *dir_name, char *type, unsigned long flags, void *data) {
    return syscall(__NR_mount, dev_name, dir_name, type, flags, data);
}
static SysRes<usize> pivot_root(const char *new_root, const char *put_old) {
    return syscall(__NR_pivot_root, new_root, put_old);
}
static SysRes<usize> statfs(const char *path, usize sz, struct statfs64 *buf) {
    return syscall(__NR_statfs, path, sz, buf);
}
static SysRes<usize> fstatfs(FdU fd, usize sz, struct statfs64 *buf) {
    return syscall(__NR_fstatfs, fd, sz, buf);
}
static SysRes<usize>
statmount(const struct mnt_id_req *req, struct statmount *buf, usize bufsize, unsigned int flags) {
    return syscall(__NR_statmount, req, buf, bufsize, flags);
}
static SysRes<usize>
listmount(const struct mnt_id_req *req, u64 *mnt_ids, usize nr_mnt_ids, unsigned int flags) {
    return syscall(__NR_listmount, req, mnt_ids, nr_mnt_ids, flags);
}
static SysRes<usize> truncate(const char *path, long length) {
    return syscall(__NR_truncate, path, length);
}
static SysRes<usize> ftruncate(FdU fd, off_t length) {
    return syscall(__NR_ftruncate, fd, length);
}
static SysRes<usize> fallocate(FdI fd, int mode, loff_t offset, loff_t len) {
    return syscall(__NR_fallocate, fd, mode, offset, len);
}
static SysRes<usize> faccessat2(FdI dfd, const char *filename, int mode, int flags) {
    return syscall(__NR_faccessat2, dfd, filename, mode, flags);
}
static SysRes<usize> chdir(const char *filename) {
    return syscall(__NR_chdir, filename);
}
static SysRes<usize> fchdir(FdU fd) {
    return syscall(__NR_fchdir, fd);
}
static SysRes<usize> chroot(const char *filename) {
    return syscall(__NR_chroot, filename);
}
static SysRes<usize> fchmod(FdU fd, umode_t mode) {
    return syscall(__NR_fchmod, fd, mode);
}
static SysRes<usize> fchmodat2(FdI dfd, const char *filename, umode_t mode, unsigned int flags) {
    return syscall(__NR_fchmodat2, dfd, filename, mode, flags);
}
static SysRes<usize> fchownat(FdI dfd, const char *filename, uid_t user, gid_t group, int flag) {
    return syscall(__NR_fchownat, dfd, filename, user, group, flag);
}
static SysRes<usize> fchown(FdU fd, uid_t user, gid_t group) {
    return syscall(__NR_fchown, fd, user, group);
}
static SysRes<usize> openat2(FdI dfd, const char *filename, struct open_how *how, usize size) {
    return syscall(__NR_openat2, dfd, filename, how, size);
}
static SysRes<usize> close(FdU fd) {
    return syscall(__NR_close, fd);
}
static SysRes<usize> close_range(FdU fd, FdU max_fd, unsigned int flags) {
    return syscall(__NR_close_range, fd, max_fd, flags);
}
static SysRes<usize> vhangup() {
    return syscall(__NR_vhangup);
}
static SysRes<usize> pipe2(int *fildes, int flags) {
    return syscall(__NR_pipe2, fildes, flags);
}
static SysRes<usize> quotactl(unsigned int cmd, const char *special, qid_t id, void *addr) {
    return syscall(__NR_quotactl, cmd, special, id, addr);
}
static SysRes<usize> quotactl_fd(FdU fd, unsigned int cmd, qid_t id, void *addr) {
    return syscall(__NR_quotactl_fd, fd, cmd, id, addr);
}
static SysRes<usize> getdents64(FdU fd, linux_dirent64 *dirent, unsigned int count) {
    return syscall(__NR_getdents64, fd, dirent, count);
}
static SysRes<off_t> lseek(FdU fd, off_t offset, unsigned int whence) {
    return syscall(__NR_lseek, fd, offset, whence).unsafe_cast<off_t>();
}
static SysRes<usize> read(FdU fd, char *buf, usize count) {
    return syscall(__NR_read, fd, buf, count);
}
static SysRes<usize> write(FdU fd, const char *buf, usize count) {
    return syscall(__NR_write, fd, buf, count);
}
static SysRes<usize> readv(FdUL fd, const struct iovec *vec, unsigned long vlen) {
    return syscall(__NR_readv, fd, vec, vlen);
}
static SysRes<usize> writev(FdUL fd, const struct iovec *vec, unsigned long vlen) {
    return syscall(__NR_writev, fd, vec, vlen);
}
static SysRes<usize> pread64(FdU fd, char *buf, usize count, loff_t pos) {
    return syscall(__NR_pread64, fd, buf, count, pos);
}
static SysRes<usize> pwrite64(FdU fd, const char *buf, usize count, loff_t pos) {
    return syscall(__NR_pwrite64, fd, buf, count, pos);
}
static SysRes<usize> preadv(
    FdUL fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h
) {
    return syscall(__NR_preadv, fd, vec, vlen, pos_l, pos_h);
}
static SysRes<usize> pwritev(
    FdUL fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h
) {
    return syscall(__NR_pwritev, fd, vec, vlen, pos_l, pos_h);
}
static SysRes<usize> sendfile(FdI out_fd, FdI in_fd, loff_t *offset, usize count) {
    return syscall(__NR_sendfile, out_fd, in_fd, offset, count);
}
static SysRes<usize>
pselect6(int a, fd_set *b, fd_set *c, fd_set *d, struct __kernel_timespec *e, void *f) {
    return syscall(__NR_pselect6, a, b, c, d, e, f);
}
static SysRes<usize>
ppoll(struct pollfd *a, unsigned int b, struct __kernel_timespec *c, const sigset_t *d, usize e) {
    return syscall(__NR_ppoll, a, b, c, d, e);
}
static SysRes<usize> signalfd4(FdI ufd, sigset_t *user_mask, usize sizemask, int flags) {
    return syscall(__NR_signalfd4, ufd, user_mask, sizemask, flags);
}
static SysRes<usize>
vmsplice(FdI fd, const struct iovec *iov, unsigned long nr_segs, unsigned int flags) {
    return syscall(__NR_vmsplice, fd, iov, nr_segs, flags);
}
static SysRes<usize>
splice(FdI fd_in, loff_t *off_in, FdI fd_out, loff_t *off_out, usize len, unsigned int flags) {
    return syscall(__NR_splice, fd_in, off_in, fd_out, off_out, len, flags);
}
static SysRes<usize> tee(FdI fdin, FdI fdout, usize len, unsigned int flags) {
    return syscall(__NR_tee, fdin, fdout, len, flags);
}
static SysRes<usize> readlinkat(FdI dfd, const char *path, char *buf, int bufsiz) {
    return syscall(__NR_readlinkat, dfd, path, buf, bufsiz);
}
static SysRes<usize> newfstatat(FdI dfd, const char *filename, struct stat *statbuf, int flag) {
    return syscall(__NR_newfstatat, dfd, filename, statbuf, flag);
}
static SysRes<usize> sync() {
    return syscall(__NR_sync);
}
static SysRes<usize> fsync(FdU fd) {
    return syscall(__NR_fsync, fd);
}
static SysRes<usize> fdatasync(FdU fd) {
    return syscall(__NR_fdatasync, fd);
}
static SysRes<usize> sync_file_range(FdI fd, unsigned int flags, loff_t offset, loff_t nbytes) {
    return syscall(__NR_sync_file_range, fd, flags, offset, nbytes);
}
static SysRes<usize> timerfd_create(int clockid, int flags) {
    return syscall(__NR_timerfd_create, clockid, flags);
}
static SysRes<usize> timerfd_settime(
    FdI ufd, int flags, const struct __kernel_itimerspec *utmr, struct __kernel_itimerspec *otmr
) {
    return syscall(__NR_timerfd_settime, ufd, flags, utmr, otmr);
}
static SysRes<usize> timerfd_gettime(FdI ufd, struct __kernel_itimerspec *otmr) {
    return syscall(__NR_timerfd_gettime, ufd, otmr);
}
static SysRes<usize>
utimensat(FdI dfd, const char *filename, struct __kernel_timespec *utimes, int flags) {
    return syscall(__NR_utimensat, dfd, filename, utimes, flags);
}
static SysRes<usize> acct(const char *name) {
    return syscall(__NR_acct, name);
}
static SysRes<usize> capget(cap_user_header_t header, cap_user_data_t dataptr) {
    return syscall(__NR_capget, header, dataptr);
}
static SysRes<usize> capset(cap_user_header_t header, const cap_user_data_t data) {
    return syscall(__NR_capset, header, data);
}
static SysRes<usize> personality(unsigned int personality) {
    return syscall(__NR_personality, personality);
}
static SysRes<usize> exit(int error_code) {
    return syscall(__NR_exit, error_code);
}
static SysRes<usize> exit_group(int error_code) {
    return syscall(__NR_exit_group, error_code);
}
static SysRes<pid_t>
waitid(int which, pid_t pid, struct siginfo *infop, int options, struct rusage *ru) {
    return syscall(__NR_waitid, which, pid, infop, options, ru).unsafe_cast<pid_t>();
}
static SysRes<usize> set_tid_address(int *tidptr) {
    return syscall(__NR_set_tid_address, tidptr);
}
static SysRes<usize> unshare(unsigned long unshare_flags) {
    return syscall(__NR_unshare, unshare_flags);
}
static SysRes<usize>
futex(u32 *uaddr, int op, u32 val, const struct __kernel_timespec *utime, u32 *uaddr2, u32 val3) {
    return syscall(__NR_futex, uaddr, op, val, utime, uaddr2, val3);
}
static SysRes<usize> get_robust_list(int pid, struct robust_list_head **head_ptr, usize *len_ptr) {
    return syscall(__NR_get_robust_list, pid, head_ptr, len_ptr);
}
static SysRes<usize> set_robust_list(struct robust_list_head *head, usize len) {
    return syscall(__NR_set_robust_list, head, len);
}
static SysRes<usize> futex_waitv(
    struct futex_waitv *waiters, unsigned int nr_futexes, unsigned int flags,
    struct __kernel_timespec *timeout, clockid_t clockid
) {
    return syscall(__NR_futex_waitv, waiters, nr_futexes, flags, timeout, clockid);
}
static SysRes<usize> futex_wake(void *uaddr, unsigned long mask, int nr, unsigned int flags) {
    return syscall(__NR_futex_wake, uaddr, mask, nr, flags);
}
static SysRes<usize> futex_wait(
    void *uaddr, unsigned long val, unsigned long mask, unsigned int flags,
    struct __kernel_timespec *timespec, clockid_t clockid
) {
    return syscall(__NR_futex_wait, uaddr, val, mask, flags, timespec, clockid);
}
static SysRes<usize>
futex_requeue(struct futex_waitv *waiters, unsigned int flags, int nr_wake, int nr_requeue) {
    return syscall(__NR_futex_requeue, waiters, flags, nr_wake, nr_requeue);
}
static SysRes<usize> nanosleep(struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp) {
    return syscall(__NR_nanosleep, rqtp, rmtp);
}
static SysRes<usize> getitimer(int which, struct __kernel_old_itimerval *value) {
    return syscall(__NR_getitimer, which, value);
}
static SysRes<usize>
setitimer(int which, struct __kernel_old_itimerval *value, struct __kernel_old_itimerval *ovalue) {
    return syscall(__NR_setitimer, which, value, ovalue);
}
static SysRes<usize> kexec_load(
    unsigned long entry, unsigned long nr_segments, struct kexec_segment *segments,
    unsigned long flags
) {
    return syscall(__NR_kexec_load, entry, nr_segments, segments, flags);
}
static SysRes<usize> init_module(void *umod, unsigned long len, const char *uargs) {
    return syscall(__NR_init_module, umod, len, uargs);
}
static SysRes<usize> delete_module(const char *name_user, unsigned int flags) {
    return syscall(__NR_delete_module, name_user, flags);
}
static SysRes<usize>
timer_create(clockid_t which_clock, struct sigevent *timer_event_spec, timer_t *created_timer_id) {
    return syscall(__NR_timer_create, which_clock, timer_event_spec, created_timer_id);
}
static SysRes<usize> timer_gettime(timer_t timer_id, struct __kernel_itimerspec *setting) {
    return syscall(__NR_timer_gettime, timer_id, setting);
}
static SysRes<usize> timer_getoverrun(timer_t timer_id) {
    return syscall(__NR_timer_getoverrun, timer_id);
}
static SysRes<usize> timer_settime(
    timer_t timer_id, int flags, const struct __kernel_itimerspec *new_setting,
    struct __kernel_itimerspec *old_setting
) {
    return syscall(__NR_timer_settime, timer_id, flags, new_setting, old_setting);
}
static SysRes<usize> timer_delete(timer_t timer_id) {
    return syscall(__NR_timer_delete, timer_id);
}
static SysRes<usize> clock_settime(clockid_t which_clock, const struct __kernel_timespec *tp) {
    return syscall(__NR_clock_settime, which_clock, tp);
}
static SysRes<usize> clock_gettime(clockid_t which_clock, struct __kernel_timespec *tp) {
    return syscall(__NR_clock_gettime, which_clock, tp);
}
static SysRes<usize> clock_getres(clockid_t which_clock, struct __kernel_timespec *tp) {
    return syscall(__NR_clock_getres, which_clock, tp);
}
static SysRes<usize> clock_nanosleep(
    clockid_t which_clock, int flags, const struct __kernel_timespec *rqtp,
    struct __kernel_timespec *rmtp
) {
    return syscall(__NR_clock_nanosleep, which_clock, flags, rqtp, rmtp);
}
static SysRes<usize> syslog(int type, char *buf, int len) {
    return syscall(__NR_syslog, type, buf, len);
}
static SysRes<usize> ptrace(long request, long pid, void *addr, unsigned long data) {
    return syscall(__NR_ptrace, request, pid, addr, data);
}
static SysRes<usize> sched_setparam(pid_t pid, sched_param *param) {
    return syscall(__NR_sched_setparam, pid, param);
}
static SysRes<usize> sched_setscheduler(pid_t pid, int policy, sched_param *param) {
    return syscall(__NR_sched_setscheduler, pid, policy, param);
}
static SysRes<usize> sched_getscheduler(pid_t pid) {
    return syscall(__NR_sched_getscheduler, pid);
}
static SysRes<usize> sched_getparam(pid_t pid, sched_param *param) {
    return syscall(__NR_sched_getparam, pid, param);
}
static SysRes<usize> sched_setaffinity(pid_t pid, unsigned int len, unsigned long *user_mask_ptr) {
    return syscall(__NR_sched_setaffinity, pid, len, user_mask_ptr);
}
static SysRes<usize> sched_getaffinity(pid_t pid, unsigned int len, unsigned long *user_mask_ptr) {
    return syscall(__NR_sched_getaffinity, pid, len, user_mask_ptr);
}
static SysRes<usize> sched_yield() {
    return syscall(__NR_sched_yield);
}
static SysRes<usize> sched_get_priority_max(int policy) {
    return syscall(__NR_sched_get_priority_max, policy);
}
static SysRes<usize> sched_get_priority_min(int policy) {
    return syscall(__NR_sched_get_priority_min, policy);
}
static SysRes<usize> sched_rr_get_interval(pid_t pid, struct __kernel_timespec *interval) {
    return syscall(__NR_sched_rr_get_interval, pid, interval);
}
static SysRes<usize> restart_syscall() {
    return syscall(__NR_restart_syscall);
}
static SysRes<usize> kill(pid_t pid, int sig) {
    return syscall(__NR_kill, pid, sig);
}
static SysRes<usize> tkill(pid_t pid, int sig) {
    return syscall(__NR_tkill, pid, sig);
}
static SysRes<usize> tgkill(pid_t tgid, pid_t pid, int sig) {
    return syscall(__NR_tgkill, tgid, pid, sig);
}
static SysRes<usize> sigaltstack(const struct sigaltstack *uss, struct sigaltstack *uoss) {
    return syscall(__NR_sigaltstack, uss, uoss);
}
static SysRes<usize> rt_sigsuspend(sigset_t *unewset, usize sigsetsize) {
    return syscall(__NR_rt_sigsuspend, unewset, sigsetsize);
}
static SysRes<usize> rt_sigprocmask(int how, sigset_t *set, sigset_t *oset, usize sigsetsize) {
    return syscall(__NR_rt_sigprocmask, how, set, oset, sigsetsize);
}
static SysRes<usize> rt_sigpending(sigset_t *set, usize sigsetsize) {
    return syscall(__NR_rt_sigpending, set, sigsetsize);
}
static SysRes<usize> rt_sigtimedwait(
    const sigset_t *uthese, siginfo_t *uinfo, const struct __kernel_timespec *uts, usize sigsetsize
) {
    return syscall(__NR_rt_sigtimedwait, uthese, uinfo, uts, sigsetsize);
}
static SysRes<usize> rt_sigqueueinfo(pid_t pid, int sig, siginfo_t *uinfo) {
    return syscall(__NR_rt_sigqueueinfo, pid, sig, uinfo);
}
static SysRes<usize> setpriority(int which, int who, int niceval) {
    return syscall(__NR_setpriority, which, who, niceval);
}
static SysRes<usize> getpriority(int which, int who) {
    return syscall(__NR_getpriority, which, who);
}
static SysRes<usize> reboot(int magic1, int magic2, unsigned int cmd, void *arg) {
    return syscall(__NR_reboot, magic1, magic2, cmd, arg);
}
static SysRes<usize> setregid(gid_t rgid, gid_t egid) {
    return syscall(__NR_setregid, rgid, egid);
}
static SysRes<usize> setgid(gid_t gid) {
    return syscall(__NR_setgid, gid);
}
static SysRes<usize> setreuid(uid_t ruid, uid_t euid) {
    return syscall(__NR_setreuid, ruid, euid);
}
static SysRes<usize> setuid(uid_t uid) {
    return syscall(__NR_setuid, uid);
}
static SysRes<usize> setresuid(uid_t ruid, uid_t euid, uid_t suid) {
    return syscall(__NR_setresuid, ruid, euid, suid);
}
static SysRes<usize> getresuid(uid_t *ruid, uid_t *euid, uid_t *suid) {
    return syscall(__NR_getresuid, ruid, euid, suid);
}
static SysRes<usize> setresgid(gid_t rgid, gid_t egid, gid_t sgid) {
    return syscall(__NR_setresgid, rgid, egid, sgid);
}
static SysRes<usize> getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid) {
    return syscall(__NR_getresgid, rgid, egid, sgid);
}
static SysRes<usize> setfsuid(uid_t uid) {
    return syscall(__NR_setfsuid, uid);
}
static SysRes<usize> setfsgid(gid_t gid) {
    return syscall(__NR_setfsgid, gid);
}
static SysRes<usize> times(struct tms *tbuf) {
    return syscall(__NR_times, tbuf);
}
static SysRes<usize> setpgid(pid_t pid, pid_t pgid) {
    return syscall(__NR_setpgid, pid, pgid);
}
static SysRes<pid_t> getpgid(pid_t pid) {
    return syscall(__NR_getpgid, pid).unsafe_cast<pid_t>();
}
static SysRes<pid_t> getsid(pid_t pid) {
    return syscall(__NR_getsid, pid).unsafe_cast<pid_t>();
}
static SysRes<usize> setsid() {
    return syscall(__NR_setsid);
}
static SysRes<usize> getgroups(int gidsetsize, gid_t *grouplist) {
    return syscall(__NR_getgroups, gidsetsize, grouplist);
}
static SysRes<usize> setgroups(int gidsetsize, gid_t *grouplist) {
    return syscall(__NR_setgroups, gidsetsize, grouplist);
}
static SysRes<usize> uname(struct new_utsname *name) {
    return syscall(__NR_uname, name);
}
static SysRes<usize> sethostname(char *name, int len) {
    return syscall(__NR_sethostname, name, len);
}
static SysRes<usize> setdomainname(char *name, int len) {
    return syscall(__NR_setdomainname, name, len);
}
static SysRes<usize> getrlimit(unsigned int resource, struct rlimit *rlim) {
    return syscall(__NR_getrlimit, resource, rlim);
}
static SysRes<usize> setrlimit(unsigned int resource, struct rlimit *rlim) {
    return syscall(__NR_setrlimit, resource, rlim);
}
static SysRes<usize> getrusage(int who, struct rusage *ru) {
    return syscall(__NR_getrusage, who, ru);
}
static SysRes<usize> umask(int mask) {
    return syscall(__NR_umask, mask);
}
static SysRes<usize>
prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
    return syscall(__NR_prctl, option, arg2, arg3, arg4, arg5);
}
static SysRes<usize> getcpu(unsigned *cpu, unsigned *node, getcpu_cache *cache) {
    return syscall(__NR_getcpu, cpu, node, cache);
}
static SysRes<usize> gettimeofday(struct __kernel_old_timeval *tv, struct timezone *tz) {
    return syscall(__NR_gettimeofday, tv, tz);
}
static SysRes<usize> settimeofday(struct __kernel_old_timeval *tv, struct timezone *tz) {
    return syscall(__NR_settimeofday, tv, tz);
}
static SysRes<usize> adjtimex(struct __kernel_timex *txc_p) {
    return syscall(__NR_adjtimex, txc_p);
}
static SysRes<pid_t> getpid() {
    return syscall(__NR_getpid).unsafe_cast<pid_t>();
}
static SysRes<pid_t> getppid() {
    return syscall(__NR_getppid).unsafe_cast<pid_t>();
}
static SysRes<usize> getuid() {
    return syscall(__NR_getuid);
}
static SysRes<usize> geteuid() {
    return syscall(__NR_geteuid);
}
static SysRes<usize> getgid() {
    return syscall(__NR_getgid);
}
static SysRes<usize> getegid() {
    return syscall(__NR_getegid);
}
static SysRes<pid_t> gettid() {
    return syscall(__NR_gettid).unsafe_cast<pid_t>();
}
static SysRes<usize> sysinfo(struct sysinfo *info) {
    return syscall(__NR_sysinfo, info);
}
static SysRes<usize> mq_open(const char *name, int oflag, umode_t mode, struct mq_attr *attr) {
    return syscall(__NR_mq_open, name, oflag, mode, attr);
}
static SysRes<usize> mq_unlink(const char *name) {
    return syscall(__NR_mq_unlink, name);
}
static SysRes<usize> mq_timedsend(
    mqd_t mqdes, const char *msg_ptr, usize msg_len, unsigned int msg_prio,
    const struct __kernel_timespec *abs_timeout
) {
    return syscall(__NR_mq_timedsend, mqdes, msg_ptr, msg_len, msg_prio, abs_timeout);
}
static SysRes<usize> mq_timedreceive(
    mqd_t mqdes, char *msg_ptr, usize msg_len, unsigned int *msg_prio,
    const struct __kernel_timespec *abs_timeout
) {
    return syscall(__NR_mq_timedreceive, mqdes, msg_ptr, msg_len, msg_prio, abs_timeout);
}
static SysRes<usize> mq_notify(mqd_t mqdes, const struct sigevent *notification) {
    return syscall(__NR_mq_notify, mqdes, notification);
}
static SysRes<usize>
mq_getsetattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat) {
    return syscall(__NR_mq_getsetattr, mqdes, mqstat, omqstat);
}
static SysRes<usize> msgget(key_t key, int msgflg) {
    return syscall(__NR_msgget, key, msgflg);
}
static SysRes<usize> msgctl(int msqid, int cmd, struct msqid_ds *buf) {
    return syscall(__NR_msgctl, msqid, cmd, buf);
}
static SysRes<usize> msgrcv(int msqid, struct msgbuf *msgp, usize msgsz, long msgtyp, int msgflg) {
    return syscall(__NR_msgrcv, msqid, msgp, msgsz, msgtyp, msgflg);
}
static SysRes<usize> msgsnd(int msqid, struct msgbuf *msgp, usize msgsz, int msgflg) {
    return syscall(__NR_msgsnd, msqid, msgp, msgsz, msgflg);
}
static SysRes<usize> semget(key_t key, int nsems, int semflg) {
    return syscall(__NR_semget, key, nsems, semflg);
}
static SysRes<usize> semctl(int semid, int semnum, int cmd, unsigned long arg) {
    return syscall(__NR_semctl, semid, semnum, cmd, arg);
}
static SysRes<usize> semtimedop(
    int semid, struct sembuf *sops, unsigned nsops, const struct __kernel_timespec *timeout
) {
    return syscall(__NR_semtimedop, semid, sops, nsops, timeout);
}
static SysRes<usize> semop(int semid, struct sembuf *sops, unsigned nsops) {
    return syscall(__NR_semop, semid, sops, nsops);
}
static SysRes<usize> shmget(key_t key, usize size, int flag) {
    return syscall(__NR_shmget, key, size, flag);
}
static SysRes<usize> shmctl(int shmid, int cmd, struct shmid_ds *buf) {
    return syscall(__NR_shmctl, shmid, cmd, buf);
}
static SysRes<void *> shmat(int shmid, char *shmaddr, int shmflg) {
    return syscall(__NR_shmat, shmid, shmaddr, shmflg).unsafe_cast<void *>();
}
static SysRes<usize> shmdt(char *shmaddr) {
    return syscall(__NR_shmdt, shmaddr);
}
static SysRes<usize> socket(int a, int b, int c) {
    return syscall(__NR_socket, a, b, c);
}
static SysRes<usize> socketpair(int a, int b, int c, int *d) {
    return syscall(__NR_socketpair, a, b, c, d);
}
static SysRes<usize> bind(int a, sockaddr *b, int c) {
    return syscall(__NR_bind, a, b, c);
}
static SysRes<usize> listen(int a, int b) {
    return syscall(__NR_listen, a, b);
}
static SysRes<usize> connect(int a, sockaddr *b, int c) {
    return syscall(__NR_connect, a, b, c);
}
static SysRes<usize> getsockname(int a, sockaddr *b, int *c) {
    return syscall(__NR_getsockname, a, b, c);
}
static SysRes<usize> getpeername(int a, sockaddr *b, int *c) {
    return syscall(__NR_getpeername, a, b, c);
}
static SysRes<usize> sendto(int a, void *b, usize c, unsigned d, sockaddr *e, int f) {
    return syscall(__NR_sendto, a, b, c, d, e, f);
}
static SysRes<usize> recvfrom(int a, void *b, usize c, unsigned d, sockaddr *e, int *f) {
    return syscall(__NR_recvfrom, a, b, c, d, e, f);
}
static SysRes<usize> setsockopt(FdI fd, int level, int optname, char *optval, int optlen) {
    return syscall(__NR_setsockopt, fd, level, optname, optval, optlen);
}
static SysRes<usize> getsockopt(FdI fd, int level, int optname, char *optval, int *optlen) {
    return syscall(__NR_getsockopt, fd, level, optname, optval, optlen);
}
static SysRes<usize> shutdown(int a, int b) {
    return syscall(__NR_shutdown, a, b);
}
static SysRes<usize> sendmsg(FdI fd, user_msghdr *msg, unsigned flags) {
    return syscall(__NR_sendmsg, fd, msg, flags);
}
static SysRes<usize> recvmsg(FdI fd, user_msghdr *msg, unsigned flags) {
    return syscall(__NR_recvmsg, fd, msg, flags);
}
static SysRes<usize> readahead(FdI fd, loff_t offset, usize count) {
    return syscall(__NR_readahead, fd, offset, count);
}
static SysRes<u8 *> brk(unsigned long brk) {
    return syscall(__NR_brk, brk).unsafe_cast<u8 *>();
}
static SysRes<usize> munmap(void *addr, usize len) {
    return syscall(__NR_munmap, addr, len);
}
static SysRes<u8 *> mremap(
    void *addr, unsigned long old_len, unsigned long new_len, unsigned long flags,
    unsigned long new_addr
) {
    return syscall(__NR_mremap, addr, old_len, new_len, flags, new_addr).unsafe_cast<u8 *>();
}
static SysRes<usize> add_key(
    const char *_type, const char *_description, const void *_payload, usize plen,
    key_serial_t destringid
) {
    return syscall(__NR_add_key, _type, _description, _payload, plen, destringid);
}
static SysRes<usize> request_key(
    const char *_type, const char *_description, const char *_callout_info, key_serial_t destringid
) {
    return syscall(__NR_request_key, _type, _description, _callout_info, destringid);
}
static SysRes<usize>
keyctl(int cmd, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
    return syscall(__NR_keyctl, cmd, arg2, arg3, arg4, arg5);
}
static SysRes<usize> clone3(struct clone_args *uargs, usize size) {
    return syscall(__NR_clone3, uargs, size);
}
static SysRes<usize>
execve(const char *filename, const char *const *argv, const char *const *envp) {
    return syscall(__NR_execve, filename, argv, envp);
}
static SysRes<usize> fadvise64(FdI fd, loff_t offset, loff_t len, int advice) {
    return syscall(__NR_fadvise64, fd, offset, len, advice);
}
static SysRes<usize> swapon(const char *specialfile, int swap_flags) {
    return syscall(__NR_swapon, specialfile, swap_flags);
}
static SysRes<usize> swapoff(const char *specialfile) {
    return syscall(__NR_swapoff, specialfile);
}
static SysRes<usize> mprotect(unsigned long start, usize len, unsigned long prot) {
    return syscall(__NR_mprotect, start, len, prot);
}
static SysRes<usize> msync(unsigned long start, usize len, int flags) {
    return syscall(__NR_msync, start, len, flags);
}
static SysRes<usize> mlock(unsigned long start, usize len) {
    return syscall(__NR_mlock, start, len);
}
static SysRes<usize> munlock(unsigned long start, usize len) {
    return syscall(__NR_munlock, start, len);
}
static SysRes<usize> mlockall(int flags) {
    return syscall(__NR_mlockall, flags);
}
static SysRes<usize> munlockall() {
    return syscall(__NR_munlockall);
}
static SysRes<usize> mincore(unsigned long start, usize len, unsigned char *vec) {
    return syscall(__NR_mincore, start, len, vec);
}
static SysRes<usize> madvise(unsigned long start, usize len, int behavior) {
    return syscall(__NR_madvise, start, len, behavior);
}
static SysRes<usize>
process_madvise(FdI pidfd, const struct iovec *vec, usize vlen, int behavior, unsigned int flags) {
    return syscall(__NR_process_madvise, pidfd, vec, vlen, behavior, flags);
}
static SysRes<usize> process_mrelease(FdI pidfd, unsigned int flags) {
    return syscall(__NR_process_mrelease, pidfd, flags);
}
static SysRes<usize> remap_file_pages(
    unsigned long start, unsigned long size, unsigned long prot, unsigned long pgoff,
    unsigned long flags
) {
    return syscall(__NR_remap_file_pages, start, size, prot, pgoff, flags);
}
static SysRes<usize> mseal(unsigned long start, usize len, unsigned long flags) {
    return syscall(__NR_mseal, start, len, flags);
}
static SysRes<usize> mbind(
    unsigned long start, unsigned long len, unsigned long mode, const unsigned long *nmask,
    unsigned long maxnode, unsigned flags
) {
    return syscall(__NR_mbind, start, len, mode, nmask, maxnode, flags);
}
static SysRes<usize> get_mempolicy(
    int *policy, unsigned long *nmask, unsigned long maxnode, void *addr, unsigned long flags
) {
    return syscall(__NR_get_mempolicy, policy, nmask, maxnode, addr, flags);
}
static SysRes<usize> set_mempolicy(int mode, const unsigned long *nmask, unsigned long maxnode) {
    return syscall(__NR_set_mempolicy, mode, nmask, maxnode);
}
static SysRes<usize> migrate_pages(
    pid_t pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to
) {
    return syscall(__NR_migrate_pages, pid, maxnode, from, to);
}
static SysRes<usize> move_pages(
    pid_t pid, unsigned long nr_pages, const void **pages, const int *nodes, int *status, int flags
) {
    return syscall(__NR_move_pages, pid, nr_pages, pages, nodes, status, flags);
}
static SysRes<usize> rt_tgsigqueueinfo(pid_t tgid, pid_t pid, int sig, siginfo_t *uinfo) {
    return syscall(__NR_rt_tgsigqueueinfo, tgid, pid, sig, uinfo);
}
static SysRes<usize> perf_event_open(
    struct perf_event_attr *attr_uptr, pid_t pid, int cpu, FdI group_fd, unsigned long flags
) {
    return syscall(__NR_perf_event_open, attr_uptr, pid, cpu, group_fd, flags);
}
static SysRes<usize> accept4(int a, sockaddr *b, int *c, int d) {
    return syscall(__NR_accept4, a, b, c, d);
}
static SysRes<usize> recvmmsg(
    FdI fd, mmsghdr *msg, unsigned int vlen, unsigned flags, struct __kernel_timespec *timeout
) {
    return syscall(__NR_recvmmsg, fd, msg, vlen, flags, timeout);
}
static SysRes<pid_t> wait4(pid_t pid, int *stat_addr, int options, struct rusage *ru) {
    return syscall(__NR_wait4, pid, stat_addr, options, ru).unsafe_cast<pid_t>();
}
static SysRes<usize> prlimit64(
    pid_t pid, unsigned int resource, const struct rlimit64 *new_rlim, struct rlimit64 *old_rlim
) {
    return syscall(__NR_prlimit64, pid, resource, new_rlim, old_rlim);
}
static SysRes<usize> fanotify_init(unsigned int flags, unsigned int event_f_flags) {
    return syscall(__NR_fanotify_init, flags, event_f_flags);
}
static SysRes<usize> fanotify_mark(
    FdI fanotify_fd, unsigned int flags, unsigned int mask_1, unsigned int mask_2, FdI dfd,
    const char *pathname
) {
    return syscall(__NR_fanotify_mark, fanotify_fd, flags, mask_1, mask_2, dfd, pathname);
}
static SysRes<usize>
name_to_handle_at(FdI dfd, const char *name, file_handle *handle, void *mnt_id, int flag) {
    return syscall(__NR_name_to_handle_at, dfd, name, handle, mnt_id, flag);
}
static SysRes<usize> open_by_handle_at(FdI mountdirfd, file_handle *handle, int flags) {
    return syscall(__NR_open_by_handle_at, mountdirfd, handle, flags);
}
static SysRes<usize> clock_adjtime(clockid_t which_clock, struct __kernel_timex *tx) {
    return syscall(__NR_clock_adjtime, which_clock, tx);
}
static SysRes<usize> syncfs(FdI fd) {
    return syscall(__NR_syncfs, fd);
}
static SysRes<usize> setns(FdI fd, int nstype) {
    return syscall(__NR_setns, fd, nstype);
}
static SysRes<usize> pidfd_open(pid_t pid, unsigned int flags) {
    return syscall(__NR_pidfd_open, pid, flags);
}
static SysRes<usize> sendmmsg(FdI fd, mmsghdr *msg, unsigned int vlen, unsigned flags) {
    return syscall(__NR_sendmmsg, fd, msg, vlen, flags);
}
static SysRes<usize> process_vm_readv(
    pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec,
    unsigned long riovcnt, unsigned long flags
) {
    return syscall(__NR_process_vm_readv, pid, lvec, liovcnt, rvec, riovcnt, flags);
}
static SysRes<usize> process_vm_writev(
    pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec,
    unsigned long riovcnt, unsigned long flags
) {
    return syscall(__NR_process_vm_writev, pid, lvec, liovcnt, rvec, riovcnt, flags);
}
static SysRes<usize>
kcmp(pid_t pid1, pid_t pid2, int type, unsigned long idx1, unsigned long idx2) {
    return syscall(__NR_kcmp, pid1, pid2, type, idx1, idx2);
}
static SysRes<usize> finit_module(FdI fd, const char *uargs, int flags) {
    return syscall(__NR_finit_module, fd, uargs, flags);
}
static SysRes<usize> sched_setattr(pid_t pid, struct sched_attr *attr, unsigned int flags) {
    return syscall(__NR_sched_setattr, pid, attr, flags);
}
static SysRes<usize>
sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags) {
    return syscall(__NR_sched_getattr, pid, attr, size, flags);
}
static SysRes<usize>
renameat2(FdI olddfd, const char *oldname, FdI newdfd, const char *newname, unsigned int flags) {
    return syscall(__NR_renameat2, olddfd, oldname, newdfd, newname, flags);
}
static SysRes<usize> seccomp(unsigned int op, unsigned int flags, void *uargs) {
    return syscall(__NR_seccomp, op, flags, uargs);
}
static SysRes<usize> getrandom(char *buf, usize count, unsigned int flags) {
    return syscall(__NR_getrandom, buf, count, flags);
}
static SysRes<usize> memfd_create(const char *uname_ptr, unsigned int flags) {
    return syscall(__NR_memfd_create, uname_ptr, flags);
}
static SysRes<usize> bpf(int cmd, union bpf_attr *attr, unsigned int size) {
    return syscall(__NR_bpf, cmd, attr, size);
}
static SysRes<usize> execveat(
    FdI dfd, const char *filename, const char *const *argv, const char *const *envp, int flags
) {
    return syscall(__NR_execveat, dfd, filename, argv, envp, flags);
}
static SysRes<usize> userfaultfd(int flags) {
    return syscall(__NR_userfaultfd, flags);
}
static SysRes<usize> membarrier(int cmd, unsigned int flags, int cpu_id) {
    return syscall(__NR_membarrier, cmd, flags, cpu_id);
}
static SysRes<usize> mlock2(unsigned long start, usize len, int flags) {
    return syscall(__NR_mlock2, start, len, flags);
}
static SysRes<usize> copy_file_range(
    FdI fd_in, loff_t *off_in, FdI fd_out, loff_t *off_out, usize len, unsigned int flags
) {
    return syscall(__NR_copy_file_range, fd_in, off_in, fd_out, off_out, len, flags);
}
static SysRes<usize> preadv2(
    FdUL fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h,
    rwf_t flags
) {
    return syscall(__NR_preadv2, fd, vec, vlen, pos_l, pos_h, flags);
}
static SysRes<usize> pwritev2(
    FdUL fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h,
    rwf_t flags
) {
    return syscall(__NR_pwritev2, fd, vec, vlen, pos_l, pos_h, flags);
}
static SysRes<usize> pkey_mprotect(unsigned long start, usize len, unsigned long prot, int pkey) {
    return syscall(__NR_pkey_mprotect, start, len, prot, pkey);
}
static SysRes<usize> pkey_alloc(unsigned long flags, unsigned long init_val) {
    return syscall(__NR_pkey_alloc, flags, init_val);
}
static SysRes<usize> pkey_free(int pkey) {
    return syscall(__NR_pkey_free, pkey);
}
static SysRes<usize>
statx(FdI dfd, const char *path, unsigned flags, unsigned mask, statx_t *buffer) {
    return syscall(__NR_statx, dfd, path, flags, mask, buffer);
}
static SysRes<usize> rseq(struct rseq *rseq, u32 rseq_len, int flags, u32 sig) {
    return syscall(__NR_rseq, rseq, rseq_len, flags, sig);
}
static SysRes<usize> open_tree(FdI dfd, const char *path, unsigned flags) {
    return syscall(__NR_open_tree, dfd, path, flags);
}
static SysRes<usize> move_mount(
    FdI from_dfd, const char *from_path, FdI to_dfd, const char *to_path, unsigned int ms_flags
) {
    return syscall(__NR_move_mount, from_dfd, from_path, to_dfd, to_path, ms_flags);
}
static SysRes<usize> mount_setattr(
    FdI dfd, const char *path, unsigned int flags, struct mount_attr *uattr, usize usize
) {
    return syscall(__NR_mount_setattr, dfd, path, flags, uattr, usize);
}
static SysRes<usize> fsopen(const char *fs_name, unsigned int flags) {
    return syscall(__NR_fsopen, fs_name, flags);
}
static SysRes<usize>
fsconfig(FdI fs_fd, unsigned int cmd, const char *key, const void *value, int aux) {
    return syscall(__NR_fsconfig, fs_fd, cmd, key, value, aux);
}
static SysRes<usize> fsmount(FdI fs_fd, unsigned int flags, unsigned int ms_flags) {
    return syscall(__NR_fsmount, fs_fd, flags, ms_flags);
}
static SysRes<usize> fspick(FdI dfd, const char *path, unsigned int flags) {
    return syscall(__NR_fspick, dfd, path, flags);
}
static SysRes<usize> pidfd_send_signal(FdI pidfd, int sig, siginfo_t *info, unsigned int flags) {
    return syscall(__NR_pidfd_send_signal, pidfd, sig, info, flags);
}
static SysRes<usize> pidfd_getfd(FdI pidfd, FdI fd, unsigned int flags) {
    return syscall(__NR_pidfd_getfd, pidfd, fd, flags);
}
static SysRes<usize>
landlock_create_ruleset(const struct landlock_ruleset_attr *attr, usize size, __u32 flags) {
    return syscall(__NR_landlock_create_ruleset, attr, size, flags);
}
static SysRes<usize> landlock_add_rule(
    FdI ruleset_fd, enum landlock_rule_type rule_type, const void *rule_attr, __u32 flags
) {
    return syscall(__NR_landlock_add_rule, ruleset_fd, rule_type, rule_attr, flags);
}
static SysRes<usize> landlock_restrict_self(FdI ruleset_fd, __u32 flags) {
    return syscall(__NR_landlock_restrict_self, ruleset_fd, flags);
}
static SysRes<usize> memfd_secret(unsigned int flags) {
    return syscall(__NR_memfd_secret, flags);
}
static SysRes<usize> set_mempolicy_home_node(
    unsigned long start, unsigned long len, unsigned long home_node, unsigned long flags
) {
    return syscall(__NR_set_mempolicy_home_node, start, len, home_node, flags);
}
static SysRes<usize> cachestat(
    FdU fd, struct cachestat_range *cstat_range, struct cachestat *cstat, unsigned int flags
) {
    return syscall(__NR_cachestat, fd, cstat_range, cstat, flags);
}
static SysRes<usize> map_shadow_stack(void *addr, unsigned long size, unsigned int flags) {
    return syscall(__NR_map_shadow_stack, addr, size, flags);
}
static SysRes<usize>
lsm_get_self_attr(unsigned int attr, struct lsm_ctx *ctx, u32 *size, u32 flags) {
    return syscall(__NR_lsm_get_self_attr, attr, ctx, size, flags);
}
static SysRes<usize>
lsm_set_self_attr(unsigned int attr, struct lsm_ctx *ctx, u32 size, u32 flags) {
    return syscall(__NR_lsm_set_self_attr, attr, ctx, size, flags);
}
static SysRes<usize> lsm_list_modules(u64 *ids, u32 *size, u32 flags) {
    return syscall(__NR_lsm_list_modules, ids, size, flags);
}
static SysRes<usize> utime(char *filename, struct utimbuf *times) {
    return syscall(__NR_utime, filename, times);
}
static SysRes<usize> utimes(char *filename, struct __kernel_old_timeval *utimes) {
    return syscall(__NR_utimes, filename, utimes);
}
static SysRes<usize> futimesat(FdI dfd, const char *filename, struct __kernel_old_timeval *utimes) {
    return syscall(__NR_futimesat, dfd, filename, utimes);
}
static SysRes<usize> creat(const char *pathname, umode_t mode) {
    return syscall(__NR_creat, pathname, mode);
}
static SysRes<usize>
select(int n, fd_set *inp, fd_set *outp, fd_set *exp, struct __kernel_old_timeval *tvp) {
    return syscall(__NR_select, n, inp, outp, exp, tvp);
}
static SysRes<usize> poll(struct pollfd *ufds, unsigned int nfds, int timeout) {
    return syscall(__NR_poll, ufds, nfds, timeout);
}
static SysRes<usize> epoll_wait(FdI epfd, struct epoll_event *events, int maxevents, int timeout) {
    return syscall(__NR_epoll_wait, epfd, events, maxevents, timeout);
}
static SysRes<pid_t> vfork() {
    return syscall(__NR_vfork).unsafe_cast<pid_t>();
}
static SysRes<usize> uselib(const char *library) {
    return syscall(__NR_uselib, library);
}
static SysRes<usize> sysfs(int option, unsigned long arg1, unsigned long arg2) {
    return syscall(__NR_sysfs, option, arg1, arg2);
}
static SysRes<pid_t> fork() {
    return syscall(__NR_fork).unsafe_cast<pid_t>();
}
static SysRes<u8 *> mmap(
    void *addr, unsigned long len, unsigned long prot, unsigned long flags, FdUL fd,
    unsigned long pgoff
) {
    return syscall(__NR_mmap, addr, len, prot, flags, fd, pgoff).unsafe_cast<u8 *>();
}

// NOLINTEND
