#define NO_ARGS true

#include <vortex/vortex.hpp>

static constexpr const char *TEST_MSG   = "hi there";
static bool server_should_exit          = false;
static bool server_received_correct_msg = false;

static void server_conn_handler(vortex::IoUring *, vortex::Slice<const u8> data) {
    server_should_exit = true;
    if (data.len == vortex::strlen(TEST_MSG) && vortex::memcmp(data.ptr, TEST_MSG, data.len) == 0) {
        server_received_correct_msg = true;
    }
}

static u8 run_server(void *) {
    auto res = []() {
        auto r = TRY(vortex::IoUring::init(16, IORING_SETUP_SINGLE_ISSUER, 0, 0), vortex::None);
        auto s = TRY(vortex::Socket::init_server(&r, vortex::SocketConfig{}), vortex::None);

        auto *buffer   = ZEROED_ARR(u8, 1024);
        vortex::FdI fd = static_cast<vortex::FdI>(TRY(s.accept(&r), vortex::None));
        auto slice     = vortex::Slice<u8>::init(sizeof(buffer), buffer);
        auto recv_len  = TRY(s.recv(&r, fd, &slice), vortex::None);
        server_conn_handler(&r, vortex::Slice<const u8>::init(recv_len, buffer));

        s.deinit(&r);
        r.deinit();
        return vortex::SysRes<vortex::None>::from_successful(vortex::None());
    }();
    if (res.is_err()) {
        vortex::println("Server error: ", static_cast<u8>(res.kind));
        vortex::assert(false, "Failed to run the server!");
    }

    return 0;
}

static vortex::SysRes<u8> main_w() {
    auto r      = TRY(vortex::IoUring::init(16, IORING_SETUP_SINGLE_ISSUER, 0, 0), u8);
    auto thread = TRY(vortex::Thread::init(run_server), u8);

    auto ts     = vortex::timespec{.tv_sec = 1, .tv_nsec = 0};
    vortex::nanosleep(&ts, vortex::null);

    auto conf = vortex::SocketConfig{};
    conf.addr = "127.0.0.1";
    auto c    = TRY(vortex::Socket::init_client(&r, conf), u8);

    TRY(c.send(&r, TEST_MSG, vortex::strlen(TEST_MSG)), u8);

    c.deinit(&r);
    TRY(thread.join(), u8);
    thread.deinit();
    r.deinit();

    return vortex::SysRes<u8>::from_successful(server_received_correct_msg ? 0 : 1);
}

static u8 main() {
    auto res = main_w();
    if (res.is_err()) {
        vortex::println("Cliend error: ", static_cast<u8>(res.kind));
        vortex::assert(false, "Failed to run the client!");
    }

    return res.unwrap();
}
