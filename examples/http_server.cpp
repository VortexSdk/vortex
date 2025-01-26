#define VORTEX_NO_ENV       true
#define VORTEX_NO_NAMESPACE true

#include <vortex/vortex.hpp>

static SignalFd sigint_handle;
static const char *host_dir = ".";
#define MS_ACCEPT_UD I64_MAX
#define INOTIFY_UD   (I64_MAX - 1)

static SysRes<None> main_w() {
    IoUring r = TRY(IoUring::init(4096, IORING_SETUP_SINGLE_ISSUER | IORING_SETUP_SQPOLL));
    TRY(r.register_sparse_files(1024));

    socklen_t addrlen      = sizeof(sockaddr_in);
    sockaddr_in sockaddrin = init_sockaddr_in(AF_INET, 8080);
    auto *sock_addr        = reinterpret_cast<sockaddr *>(&sockaddrin);
    Fd sockfd              = TRY(init_tcp_server(sock_addr));
    TRY(r.update_file(0, sockfd, 0));

    PageAllocator p1    = TRY(PageAllocator::init(256));
    PageAllocator p2    = TRY(PageAllocator::init(256));
    Allocator<Arena> pa = Allocator<Arena>::init(p1.len, p1.ptr);
    Allocator<Arena> ta = Allocator<Arena>::init(p2.len, p2.ptr);
    FsMap fm            = TRY(FsMap::init(&pa, &r, INOTIFY_UD, 1, 1, "vortex"));
    TRY(fm.load_dir(&pa, &ta));
    ta.reset();

    IoUringBufRing br = TRY(IoUringBufRing::init(&r, 0, 1024, 14));
    TRY(SignalFd::init_and_read(&sigint_handle, 2UL, &r, 0));
    IO_TRY_SUBMIT(r, r.accept_multishot(MS_ACCEPT_UD, 0, sock_addr, &addrlen));
    for (;;) {
        CqeRes sub = TRY(TRY(r.copy_cqe()));
        if (sub.user_data == 0) {
            println("CRTL received");
            break;
        }
        if (sub.user_data == MS_ACCEPT_UD) {
            IO_TRY_SUBMIT(r, r.recv_multishot(static_cast<u64>(sub.res), Fd(sub.res), 0));
            continue;
        }
        if (sub.user_data == INOTIFY_UD) {
            fm.process_cqe(&pa, sub);
            continue;
        }

        Slice<u8> buf = br.get_buf(sub);
        println(reinterpret_cast<char *>(buf.ptr));
        if ((sub.flags & IORING_CQE_F_MORE) == 0) {
            IO_TRY_SUBMIT(r, r.skip_success(r.close_direct(0, Fd(sub.user_data))));
        }
        br.recycle_buf(sub);
    }

    TRY(fm.deinit());
    TRY(br.deinit(&r));
    TRY(r.deinit());
    TRY(sigint_handle.deinit());

    return None();
}

static u8 main(Slice<char *> args) {
    if (args.len >= 2) host_dir = *args [1];

    SysRes<None> res = main_w();
    if (res.is_err()) {
        eprintln("Server failed to start with the code: ", static_cast<u8>(res.kind));
        eprintln("File: ", res.file, " Line: ", res.line);

        return 1;
    }

    return 0;
}
