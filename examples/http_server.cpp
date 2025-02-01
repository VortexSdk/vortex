#define VORTEX_NO_ENV       true
#define VORTEX_NO_NAMESPACE true

#include <vortex/vortex.hpp>

enum class UserDataKind : u8 {
    RECV,
    SEND_ZC,
    ACCEPT,
    CLOSE,
    CRTL_C,
    CANCEL_RECV,
};
struct UserData {
    union {
        struct {
            i32 fd: 32;
            UserDataKind kind: 8;
            i32 _gap: 24;
        } vals;
        u64 bits;
    };

    UserData(u64 _bits) : bits(_bits) {}
    UserData(Fd _fd, UserDataKind _kind) : vals({.fd = _fd, .kind = _kind, ._gap = 0}) {}
    operator u64() const {
        return bits;
    }
    Fd fd() {
        return vals.fd;
    }
    UserDataKind kind() {
        return vals.kind;
    }
};

static const char E404 []         = "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Connection: close\r\n"
                                    "Content-Length: 13\r\n"
                                    "\r\n"
                                    "404 Not Found\r\n";
static const char E405_chunked [] = "HTTP/1.1 405 Method Not Allowed\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Connection: close\r\n"
                                    "Content-Length: 33\r\n"
                                    "\r\n"
                                    "Chunked requests are not allowed.\r\n";
static const char E400 []         = "HTTP/1.1 405 Bad Request\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Connection: close\r\n"
                                    "Content-Length: 12\r\n"
                                    "\r\n"
                                    "Bad request.\r\n";
static const char C200 []         = "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Connection: close\r\n"
                                    "Content-Length: 6\r\n"
                                    "\r\n"
                                    "200 Ok\r\n";

static const char *host_dir       = ".";
static SysRes<None> thread_main_w() {
    static SignalFd sigint_handle;
    IoUring r = TRY(IoUring::init(4096, IORING_SETUP_SINGLE_ISSUER | IORING_SETUP_SQPOLL));
    TRY(r.register_sparse_files(1024));

    socklen_t addrlen      = sizeof(sockaddr_in);
    sockaddr_in sockaddrin = init_sockaddr_in(AF_INET, 8080);
    auto *sock_addr        = reinterpret_cast<sockaddr *>(&sockaddrin);
    Fd sockfd              = TRY(init_tcp_server(sock_addr));
    TRY(r.update_file(0, sockfd, 0));

    PageAllocator p1    = TRY(PageAllocator::init(512));
    PageAllocator p2    = TRY(PageAllocator::init(512));
    Allocator<Arena> pa = Allocator<Arena>::init(p1.len, p1.ptr);
    Allocator<Arena> ta = Allocator<Arena>::init(p2.len, p2.ptr);
    FsMap fm            = TRY(FsMap::init(&pa, ImOsString::init(host_dir)));
    TRY(fm.load_dir(&r, &pa, &ta));
    TRY(p2.deinit());

    StringMap files_http_resp = TRY(StringMap::init(&pa, fm.fs_map.key_value_len * 2));
    for (usize i = 0; i < fm.fs_map.ncm.entries.len; i++) {
        MapEntry<OsString, MappedFile> *entry = fm.fs_map.ncm.entries [i];
        if (entry->status != MapEntryStatus::Filled) continue;
        // files_http_resp.insert(&pa, "");
        //        fm.fs_map.ncm.entries.
        //        files_http_res.
    }

    PhrHeader *headers = pa.alloc<PhrHeader>(100).ptr;
    IoUringBufRing br  = TRY(IoUringBufRing::init(&r, 0, 1024, 14));
    TRY(SignalFd::init_and_read(&sigint_handle, 2UL, &r, UserData(0, UserDataKind::CRTL_C)));
    IO_TRY_SUBMIT(
        r, r.accept_multishot(UserData(sockfd, UserDataKind::ACCEPT), 0, sock_addr, &addrlen)
    );
    for (;;) {
        CqeRes sub  = TRY(r.copy_cqe());
        UserData ud = sub.user_data;

        if (sub.err_kind() == SysResKind::CANCELED) continue;
        if ((sub.err_kind() == SysResKind::BADF || sub.err_kind() == SysResKind::NOENT) &&
            (ud.kind() == UserDataKind::CANCEL_RECV || ud.kind() == UserDataKind::CLOSE ||
             ud.kind() == UserDataKind::SEND_ZC))
            continue;

        sub = IR_TRY(sub);
        if (ud.kind() == UserDataKind::CRTL_C) {
            println("CRTL received");
            break;
        }
        if (ud.kind() == UserDataKind::ACCEPT) {
            IO_TRY_SUBMIT(
                r, r.recv_multishot(UserData(Fd(sub.res), UserDataKind::RECV), Fd(sub.res), 0)
            );
            continue;
        }
        if (ud.kind() == UserDataKind::SEND_ZC) {
            if ((sub.flags & IORING_CQE_F_NOTIF) ||
                (!(sub.flags & IORING_CQE_F_NOTIF) && !(sub.flags & IORING_CQE_F_MORE))) {
                IO_TRY_ADD(r.hard_link(r.cancel(
                    UserData(sub.user_data, UserDataKind::CANCEL_RECV),
                    UserData(ud.fd(), UserDataKind::RECV), 0
                )));
                IO_TRY_SUBMIT(
                    r, r.close_direct(UserData(sub.user_data, UserDataKind::CLOSE), ud.fd())
                );
            }
            continue;
        }

        Slice<char> buf = br.get_buf(sub);
        if (!(sub.flags & IORING_CQE_F_MORE)) {
            IO_TRY_SUBMIT(
                r,
                r.skip_success(r.close_direct(UserData(sub.user_data, UserDataKind::CLOSE), ud.fd())
                )
            );
            br.recycle_buf(sub);
            continue;
        }

        int http_minor_version;
        Slice<const char> http_path;
        Slice<const char> http_method;
        Slice<PhrHeader> headers_s = Slice<PhrHeader>::init(100, headers);
        println("Buf len is: ", buf.len);
        int pr =
            phr_parse_request(buf, &http_method, &http_path, &http_minor_version, &headers_s, 0);
        if (pr == -1) {
            IO_TRY_SUBMIT(
                r, r.send_zc(UserData(ud.fd(), UserDataKind::SEND_ZC), ud.fd(), E400, sizeof(E400))
            );
        } else if (pr == -2) {
            IO_TRY_SUBMIT(
                r, r.send_zc(
                       UserData(ud.fd(), UserDataKind::SEND_ZC), ud.fd(), E405_chunked,
                       sizeof(E405_chunked)
                   )
            );
        } else if (http_method.len < 3 || memcmp(http_method.ptr, "GET", 3) != 0) {
            IO_TRY_SUBMIT(
                r, r.send_zc(UserData(ud.fd(), UserDataKind::SEND_ZC), ud.fd(), E404, sizeof(E404))
            );
        } else {
            IO_TRY_SUBMIT(
                r, r.send_zc(UserData(ud.fd(), UserDataKind::SEND_ZC), ud.fd(), C200, sizeof(C200))
            );
        }
        br.recycle_buf(sub);
    }

    TRY(fm.deinit(&pa));
    TRY(br.deinit(&r));
    TRY(r.deinit());
    TRY(sigint_handle.deinit());

    return None();
}
static u8 thread_main(void *) {
    SysRes<None> res = thread_main_w();
    if (res.is_err()) {
        eprintln("Server failed to start with the code: ", static_cast<u8>(res.kind));
        eprintln("File: ", res.file, " Line: ", res.line);

        return 1;
    }

    return 0;
}

static u8 main(Slice<char *> args) {
    if (args.len >= 2) host_dir = *args [1];

    // SysRes<Thread> tr = Thread::init(
    //        thread_main, null, (1_usize << 12_usize) * 1024_usize * 1024_usize,
    //        CLONE_SIGHAND | CLONE_VM | CLONE_IO
    //    );
    //    if (tr.is_err()) return 1;

    u8 main = thread_main(null);

    return main;
}
