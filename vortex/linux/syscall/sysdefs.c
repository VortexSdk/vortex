// This file is manually extracted from https://github.com/torvalds/linux/blob/master/include/linux/syscalls.h. 
// The syscalls are ordered by how they're defnied in https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/unistd.h.
// Please insert new syscalls by the defined order and rearrange them if you spot a problem.

long io_setup(unsigned nr_reqs, aio_context_t *ctx);
long io_destroy(aio_context_t ctx);
long io_submit(aio_context_t, long, struct iocb **);
long io_cancel(aio_context_t ctx_id, struct iocb *iocb, struct io_event *result);
long io_getevents(aio_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct __kernel_timespec *timeout);
long io_pgetevents(aio_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct __kernel_timespec *timeout, const struct __aio_sigset *sig);
long io_uring_setup(u32 entries, struct io_uring_params *p);
long io_uring_enter(unsigned int fd, u32 to_submit, u32 min_complete, u32 flags, const void *argp, size_t argsz);
long io_uring_register(unsigned int fd, unsigned int op, void *arg, unsigned int nr_args);
long setxattr(const char *path, const char *name, const void *value, size_t size, int flags);
long lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags);
long fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);
long getxattr(const char *path, const char *name, void *value, size_t size);
long lgetxattr(const char *path, const char *name, void *value, size_t size);
long fgetxattr(int fd, const char *name, void *value, size_t size);
long listxattr(const char *path, char *list, size_t size);
long llistxattr(const char *path, char *list, size_t size);
long flistxattr(int fd, char *list, size_t size);
long removexattr(const char *path, const char *name);
long lremovexattr(const char *path, const char *name);
long fremovexattr(int fd, const char *name);
long getcwd(char *buf, unsigned long size);
long eventfd2(unsigned int count, int flags);
long epoll_create1(int flags);
long epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
long epoll_pwait2(int epfd, struct epoll_event *events, int maxevents, const struct __kernel_timespec *timeout, const sigset_t *sigmask, size_t sigsetsize);
long dup3(unsigned int oldfd, unsigned int newfd, int flags);
long fcntl(unsigned int fd, unsigned int cmd, unsigned long arg);
long inotify_init1(int flags);
long inotify_add_watch(int fd, const char *path, u32 mask);
long inotify_rm_watch(int fd, __s32 wd);
long ioctl(unsigned int fd, unsigned int cmd, unsigned long arg);
long ioprio_set(int which, int who, int ioprio);
long ioprio_get(int which, int who);
long flock(unsigned int fd, unsigned int cmd);
long mknodat(int dfd, const char *filename, umode_t mode, unsigned dev);
long mkdirat(int dfd, const char *pathname, umode_t mode);
long unlinkat(int dfd, const char *pathname, int flag);
long symlinkat(const char *oldname, int newdfd, const char *newname);
long linkat(int olddfd, const char *oldname, int newdfd, const char *newname, int flags);
long renameat(int olddfd, const char *oldname, int newdfd, const char *newname);
long umount(char *name, int flags);
long mount(char *dev_name, char *dir_name, char *type, unsigned long flags, void *data);
long pivot_root(const char *new_root, const char *put_old);
long statfs64(const char *path, size_t sz, struct statfs64 *buf);
long fstatfs64(unsigned int fd, size_t sz, struct statfs64 *buf);
long statmount(const struct mnt_id_req *req, struct statmount *buf, size_t bufsize, unsigned int flags);
long listmount(const struct mnt_id_req *req, u64 *mnt_ids, size_t nr_mnt_ids, unsigned int flags);
long truncate(const char *path, long length);
long ftruncate(unsigned int fd, off_t length);
long fallocate(int fd, int mode, loff_t offset, loff_t len);
long faccessat2(int dfd, const char *filename, int mode, int flags);
long chdir(const char *filename);
long fchdir(unsigned int fd);
long chroot(const char *filename);
long fchmod(unsigned int fd, umode_t mode);
long fchmodat2(int dfd, const char *filename, umode_t mode, unsigned int flags);
long fchownat(int dfd, const char *filename, uid_t user, gid_t group, int flag);
long fchown(unsigned int fd, uid_t user, gid_t group);
long openat2(int dfd, const char *filename, struct open_how *how, size_t size);
long close(unsigned int fd);
long close_range(unsigned int fd, unsigned int max_fd, unsigned int flags);
long vhangup(void);
long pipe2(int *fildes, int flags);
long quotactl(unsigned int cmd, const char *special, qid_t id, void *addr);
long quotactl_fd(unsigned int fd, unsigned int cmd, qid_t id, void *addr);
long getdents64(unsigned int fd, struct linux_dirent64 *dirent, unsigned int count);
long lseek(unsigned int fd, off_t offset, unsigned int whence);
long read(unsigned int fd, char *buf, size_t count);
long write(unsigned int fd, const char *buf, size_t count);
long readv(unsigned long fd, const struct iovec *vec, unsigned long vlen);
long writev(unsigned long fd, const struct iovec *vec, unsigned long vlen);
long pread64(unsigned int fd, char *buf, size_t count, loff_t pos);
long pwrite64(unsigned int fd, const char *buf, size_t count, loff_t pos);
long preadv(unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h);
long pwritev(unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h);
long sendfile64(int out_fd, int in_fd, loff_t *offset, size_t count);
long pselect6(int, fd_set *, fd_set *, fd_set *, struct __kernel_timespec *, void *);
long ppoll(struct pollfd *, unsigned int, struct __kernel_timespec *, const sigset_t *, size_t);
long signalfd4(int ufd, sigset_t *user_mask, size_t sizemask, int flags);
long vmsplice(int fd, const struct iovec *iov, unsigned long nr_segs, unsigned int flags);
long splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
long tee(int fdin, int fdout, size_t len, unsigned int flags);
long readlinkat(int dfd, const char *path, char *buf, int bufsiz);
long newfstatat(int dfd, const char *filename, struct stat *statbuf, int flag);
long sync(void);
long fsync(unsigned int fd);
long fdatasync(unsigned int fd);
long sync_file_range2(int fd, unsigned int flags, loff_t offset, loff_t nbytes);
long timerfd_create(int clockid, int flags);
long timerfd_settime(int ufd, int flags, const struct __kernel_itimerspec *utmr, struct __kernel_itimerspec *otmr);
long timerfd_gettime(int ufd, struct __kernel_itimerspec *otmr);
long utimensat(int dfd, const char *filename, struct __kernel_timespec *utimes, int flags);
long acct(const char *name);
long capget(cap_user_header_t header, cap_user_data_t dataptr);
long capset(cap_user_header_t header, const cap_user_data_t data);
long personality(unsigned int personality);
long exit(int error_code);
long exit_group(int error_code);
long waitid(int which, pid_t pid, struct siginfo *infop, int options, struct rusage *ru);
long set_tid_address(int *tidptr);
long unshare(unsigned long unshare_flags);
long futex(u32 *uaddr, int op, u32 val, const struct __kernel_timespec *utime, u32 *uaddr2, u32 val3);
long get_robust_list(int pid, struct robust_list_head **head_ptr, size_t *len_ptr);
long set_robust_list(struct robust_list_head *head, size_t len);
long futex_waitv(struct futex_waitv *waiters, unsigned int nr_futexes, unsigned int flags, struct __kernel_timespec *timeout, clockid_t clockid);
long futex_wake(void *uaddr, unsigned long mask, int nr, unsigned int flags);
long futex_wait(void *uaddr, unsigned long val, unsigned long mask, unsigned int flags, struct __kernel_timespec *timespec, clockid_t clockid);
long futex_requeue(struct futex_waitv *waiters, unsigned int flags, int nr_wake, int nr_requeue);
long nanosleep(struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp);
long getitimer(int which, struct __kernel_old_itimerval *value);
long setitimer(int which, struct __kernel_old_itimerval *value, struct __kernel_old_itimerval *ovalue);
long kexec_load(unsigned long entry, unsigned long nr_segments, struct kexec_segment *segments, unsigned long flags);
long init_module(void *umod, unsigned long len, const char *uargs);
long delete_module(const char *name_user, unsigned int flags);
long timer_create(clockid_t which_clock, struct sigevent *timer_event_spec, timer_t *created_timer_id);
long timer_gettime(timer_t timer_id, struct __kernel_itimerspec *setting);
long timer_getoverrun(timer_t timer_id);
long timer_settime(timer_t timer_id, int flags, const struct __kernel_itimerspec *new_setting, struct __kernel_itimerspec *old_setting);
long timer_delete(timer_t timer_id);
long clock_settime(clockid_t which_clock, const struct __kernel_timespec *tp);
long clock_gettime(clockid_t which_clock, struct __kernel_timespec *tp);
long clock_getres(clockid_t which_clock, struct __kernel_timespec *tp);
long clock_nanosleep(clockid_t which_clock, int flags, const struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp);
long syslog(int type, char *buf, int len);
long ptrace(long request, long pid, unsigned long addr, unsigned long data);
long sched_setparam(pid_t pid, struct sched_param *param);
long sched_setscheduler(pid_t pid, int policy, struct sched_param *param);
long sched_getscheduler(pid_t pid);
long sched_getparam(pid_t pid, struct sched_param *param);
long sched_setaffinity(pid_t pid, unsigned int len, unsigned long *user_mask_ptr);
long sched_getaffinity(pid_t pid, unsigned int len, unsigned long *user_mask_ptr);
long sched_yield(void);
long sched_get_priority_max(int policy);
long sched_get_priority_min(int policy);
long sched_rr_get_interval(pid_t pid, struct __kernel_timespec *interval);
long restart_syscall(void);
long kill(pid_t pid, int sig);
long tkill(pid_t pid, int sig);
long tgkill(pid_t tgid, pid_t pid, int sig);
long sigaltstack(const struct sigaltstack *uss, struct sigaltstack *uoss);
long rt_sigsuspend(sigset_t *unewset, size_t sigsetsize);
long rt_sigprocmask(int how, sigset_t *set, sigset_t *oset, size_t sigsetsize);
long rt_sigpending(sigset_t *set, size_t sigsetsize);
long rt_sigtimedwait(const sigset_t *uthese, siginfo_t *uinfo, const struct __kernel_timespec *uts, size_t sigsetsize);
long rt_sigqueueinfo(pid_t pid, int sig, siginfo_t *uinfo);
long setpriority(int which, int who, int niceval);
long getpriority(int which, int who);
long reboot(int magic1, int magic2, unsigned int cmd, void *arg);
long setregid(gid_t rgid, gid_t egid);
long setgid(gid_t gid);
long setreuid(uid_t ruid, uid_t euid);
long setuid(uid_t uid);
long setresuid(uid_t ruid, uid_t euid, uid_t suid);
long getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
long setresgid(gid_t rgid, gid_t egid, gid_t sgid);
long getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);
long setfsuid(uid_t uid);
long setfsgid(gid_t gid);
long times(struct tms *tbuf);
long setpgid(pid_t pid, pid_t pgid);
long getpgid(pid_t pid);
long getsid(pid_t pid);
long setsid(void);
long getgroups(int gidsetsize, gid_t *grouplist);
long setgroups(int gidsetsize, gid_t *grouplist);
long newuname(struct new_utsname *name);
long sethostname(char *name, int len);
long setdomainname(char *name, int len);
long getrlimit(unsigned int resource, struct rlimit *rlim);
long setrlimit(unsigned int resource, struct rlimit *rlim);
long getrusage(int who, struct rusage *ru);
long umask(int mask);
long prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
long getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *cache);
long gettimeofday(struct __kernel_old_timeval *tv, struct timezone *tz);
long settimeofday(struct __kernel_old_timeval *tv, struct timezone *tz);
long adjtimex(struct __kernel_timex *txc_p);
long getpid(void);
long getppid(void);
long getuid(void);
long geteuid(void);
long getgid(void);
long getegid(void);
long gettid(void);
long sysinfo(struct sysinfo *info);
long mq_open(const char *name, int oflag, umode_t mode, struct mq_attr *attr);
long mq_unlink(const char *name);
long mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct __kernel_timespec *abs_timeout);
long mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio, const struct __kernel_timespec *abs_timeout);
long mq_notify(mqd_t mqdes, const struct sigevent *notification);
long mq_getsetattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat);
long msgget(key_t key, int msgflg);
long msgctl(int msqid, int cmd, struct msqid_ds *buf);
long msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg);
long msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg);
long semget(key_t key, int nsems, int semflg);
long semctl(int semid, int semnum, int cmd, unsigned long arg);
long semtimedop(int semid, struct sembuf *sops, unsigned nsops, const struct __kernel_timespec *timeout);
long semop(int semid, struct sembuf *sops, unsigned nsops);
long shmget(key_t key, size_t size, int flag);
long shmctl(int shmid, int cmd, struct shmid_ds *buf);
long shmat(int shmid, char *shmaddr, int shmflg);
long shmdt(char *shmaddr);
long socket(int, int, int);
long socketpair(int, int, int, int *);
long bind(int, struct sockaddr *, int);
long listen(int, int);
long connect(int, struct sockaddr *, int);
long getsockname(int, struct sockaddr *, int *);
long getpeername(int, struct sockaddr *, int *);
long sendto(int, void *, size_t, unsigned, struct sockaddr *, int);
long recvfrom(int, void *, size_t, unsigned, struct sockaddr *, int *);
long setsockopt(int fd, int level, int optname, char *optval, int optlen);
long getsockopt(int fd, int level, int optname, char *optval, int *optlen);
long shutdown(int, int);
long sendmsg(int fd, struct user_msghdr *msg, unsigned flags);
long recvmsg(int fd, struct user_msghdr *msg, unsigned flags);
long readahead(int fd, loff_t offset, size_t count);
long brk(unsigned long brk);
long munmap(unsigned long addr, size_t len);
long mremap(unsigned long addr, unsigned long old_len, unsigned long new_len, unsigned long flags, unsigned long new_addr);
long add_key(const char *_type, const char *_description, const void *_payload, size_t plen, key_serial_t destringid);
long request_key(const char *_type, const char *_description, const char *_callout_info, key_serial_t destringid);
long keyctl(int cmd, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
long execve(const char *filename, const char *const *argv, const char *const *envp);
long fadvise64_64(int fd, loff_t offset, loff_t len, int advice);
long swapon(const char *specialfile, int swap_flags);
long swapoff(const char *specialfile);
long mprotect(unsigned long start, size_t len, unsigned long prot);
long msync(unsigned long start, size_t len, int flags);
long mlock(unsigned long start, size_t len);
long munlock(unsigned long start, size_t len);
long mlockall(int flags);
long munlockall(void);
long mincore(unsigned long start, size_t len, unsigned char *vec);
long madvise(unsigned long start, size_t len, int behavior);
long process_madvise(int pidfd, const struct iovec *vec, size_t vlen, int behavior, unsigned int flags);
long process_mrelease(int pidfd, unsigned int flags);
long remap_file_pages(unsigned long start, unsigned long size, unsigned long prot, unsigned long pgoff, unsigned long flags);
long mseal(unsigned long start, size_t len, unsigned long flags);
long mbind(unsigned long start, unsigned long len, unsigned long mode, const unsigned long *nmask, unsigned long maxnode, unsigned flags);
long get_mempolicy(int *policy, unsigned long *nmask, unsigned long maxnode, unsigned long addr, unsigned long flags);
long set_mempolicy(int mode, const unsigned long *nmask, unsigned long maxnode);
long migrate_pages(pid_t pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to);
long move_pages(pid_t pid, unsigned long nr_pages, const void **pages, const int *nodes, int *status, int flags);
long rt_tgsigqueueinfo(pid_t tgid, pid_t pid, int sig, siginfo_t *uinfo);
long perf_event_open(struct perf_event_attr *attr_uptr, pid_t pid, int cpu, int group_fd, unsigned long flags);
long accept4(int, struct sockaddr *, int *, int);
long recvmmsg(int fd, struct mmsghdr *msg, unsigned int vlen, unsigned flags, struct __kernel_timespec *timeout);
long wait4(pid_t pid, int *stat_addr, int options, struct rusage *ru);
long prlimit64(pid_t pid, unsigned int resource, const struct rlimit64 *new_rlim, struct rlimit64 *old_rlim);
long fanotify_init(unsigned int flags, unsigned int event_f_flags);
long fanotify_mark(int fanotify_fd, unsigned int flags, unsigned int mask_1, unsigned int mask_2, int dfd, const char *pathname);
long name_to_handle_at(int dfd, const char *name, struct file_handle *handle, void *mnt_id, int flag);
long open_by_handle_at(int mountdirfd, struct file_handle *handle, int flags);
long clock_adjtime(clockid_t which_clock, struct __kernel_timex *tx);
long syncfs(int fd);
long setns(int fd, int nstype);
long pidfd_open(pid_t pid, unsigned int flags);
long sendmmsg(int fd, struct mmsghdr *msg, unsigned int vlen, unsigned flags);
long process_vm_readv(pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags);
long process_vm_writev(pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags);
long kcmp(pid_t pid1, pid_t pid2, int type, unsigned long idx1, unsigned long idx2);
long finit_module(int fd, const char *uargs, int flags);
long sched_setattr(pid_t pid, struct sched_attr *attr, unsigned int flags);
long sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags);
long renameat2(int olddfd, const char *oldname, int newdfd, const char *newname, unsigned int flags);
long seccomp(unsigned int op, unsigned int flags, void *uargs);
long getrandom(char *buf, size_t count, unsigned int flags);
long memfd_create(const char *uname_ptr, unsigned int flags);
long bpf(int cmd, union bpf_attr *attr, unsigned int size);
long execveat(int dfd, const char *filename, const char *const *argv, const char *const *envp, int flags);
long userfaultfd(int flags);
long membarrier(int cmd, unsigned int flags, int cpu_id);
long mlock2(unsigned long start, size_t len, int flags);
long copy_file_range(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
long preadv2(unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags);
long pwritev2(unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags);
long pkey_mprotect(unsigned long start, size_t len, unsigned long prot, int pkey);
long pkey_alloc(unsigned long flags, unsigned long init_val);
long pkey_free(int pkey);
long statx(int dfd, const char *path, unsigned flags, unsigned mask, struct statx *buffer);
long rseq(struct rseq *rseq, uint32_t rseq_len, int flags, uint32_t sig);
long open_tree(int dfd, const char *path, unsigned flags);
long move_mount(int from_dfd, const char *from_path, int to_dfd, const char *to_path, unsigned int ms_flags);
long mount_setattr(int dfd, const char *path, unsigned int flags, struct mount_attr *uattr, size_t usize);
long fsopen(const char *fs_name, unsigned int flags);
long fsconfig(int fs_fd, unsigned int cmd, const char *key, const void *value, int aux);
long fsmount(int fs_fd, unsigned int flags, unsigned int ms_flags);
long fspick(int dfd, const char *path, unsigned int flags);
long pidfd_send_signal(int pidfd, int sig, siginfo_t *info, unsigned int flags);
long pidfd_getfd(int pidfd, int fd, unsigned int flags);
long landlock_create_ruleset(const struct landlock_ruleset_attr *attr, size_t size, __u32 flags);
long landlock_add_rule(int ruleset_fd, enum landlock_rule_type rule_type, const void *rule_attr, __u32 flags);
long landlock_restrict_self(int ruleset_fd, __u32 flags);
long memfd_secret(unsigned int flags);
long set_mempolicy_home_node(unsigned long start, unsigned long len, unsigned long home_node, unsigned long flags);
long cachestat(unsigned int fd, struct cachestat_range *cstat_range, struct cachestat *cstat, unsigned int flags);
long map_shadow_stack(unsigned long addr, unsigned long size, unsigned int flags);
long lsm_get_self_attr(unsigned int attr, struct lsm_ctx *ctx, u32 *size, u32 flags);
long lsm_set_self_attr(unsigned int attr, struct lsm_ctx *ctx, u32 size, u32 flags);
long lsm_list_modules(u64 *ids, u32 *size, u32 flags);
long utime(char *filename, struct utimbuf *times);
long utimes(char *filename, struct __kernel_old_timeval *utimes);
long futimesat(int dfd, const char *filename, struct __kernel_old_timeval *utimes);
long creat(const char *pathname, umode_t mode);
long select(int n, fd_set *inp, fd_set *outp, fd_set *exp, struct __kernel_old_timeval *tvp);
long poll(struct pollfd *ufds, unsigned int nfds, int timeout);
long epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
long vfork(void);
long uselib(const char *library);
long sysfs(int option, unsigned long arg1, unsigned long arg2);
long fork(void);
long mmap(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long pgoff);

