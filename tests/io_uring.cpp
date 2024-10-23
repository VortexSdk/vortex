#define NO_ARGS true

#include <vortex/vortex.hpp>

static u8 main() {
    auto io_r = vortex::IoUring::init(4, 0, 0, 0);
    vortex::assert(io_r.is_ok(), "Failed to initialize IoUring.");
    auto io             = io_r.unwrap();

    const char hello [] = "Hi there!\n";
    io.write(0, 0, hello, sizeof(hello));

    io.submit_and_wait(1);

    io.deinit();

    return 0;
}
