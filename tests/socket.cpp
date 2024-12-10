#define NO_ARGS true

#include <vortex/vortex.hpp>

static bool server_received_correct_msg = false;
static constexpr const char *TEST_MSG   = "hi there 439203849230";

static vortex::SysRes<vortex::None> run_server() {
    vortex::IoUring r = TRY(vortex::IoUring::init(16, IORING_SETUP_SINGLE_ISSUER, 0, 0));
    vortex::Socket s  = TRY(vortex::Socket::init_server(&r, vortex::SocketConfig{}));

    u8 buffer [1024];
    vortex::memset(reinterpret_cast<char *>(&buffer), 0, 1024);
    vortex::Slice<u8> slice = vortex::Slice<u8>::init(1024, buffer);

    vortex::FdI fd          = static_cast<vortex::FdI>(TRY(s.accept(&r)));
    usize recv_len          = TRY(s.recv(&r, fd, &slice));

    auto test_len           = vortex::strlen(TEST_MSG);
    auto cmp                = vortex::memcmp(buffer, TEST_MSG, recv_len);
    if (recv_len == test_len && cmp == 0) {
        server_received_correct_msg = true;
    }

    s.deinit(&r);
    r.deinit();

    return vortex::None();
}
static u8 run_server_w(void *) {
    vortex::SysRes<vortex::None> res = run_server();
    if (res.is_err()) {
        vortex::println("Server error: ", static_cast<u8>(res.kind));
        vortex::assert(false, "Failed to run the server!");
    }

    return 0;
}

static vortex::SysRes<u8> main_w() {
    vortex::IoUring r     = TRY(vortex::IoUring::init(16, IORING_SETUP_SINGLE_ISSUER, 0, 0), u8);
    vortex::Thread thread = TRY(vortex::Thread::init(run_server_w), u8);

    vortex::timespec ts   = vortex::timespec{.tv_sec = 1, .tv_nsec = 0};
    vortex::nanosleep(&ts, vortex::null);

    vortex::SocketConfig conf = vortex::SocketConfig{};
    conf.addr                 = "127.0.0.1";
    vortex::Socket c          = TRY(vortex::Socket::init_client(&r, conf), u8);

    TRY(c.send(&r, TEST_MSG, vortex::strlen(TEST_MSG)), u8);

    c.deinit(&r);
    TRY(thread.join(), u8);
    thread.deinit();
    r.deinit();

    return server_received_correct_msg ? 0 : 1;
}

static u8 main() {
    vortex::SysRes<u8> res = main_w();
    if (res.is_err()) {
        vortex::println("Cliend error: ", static_cast<u8>(res.kind));
        vortex::assert(false, "Failed to run the client!");
    }

    return res.unwrap();
}
