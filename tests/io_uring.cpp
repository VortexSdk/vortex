#define NO_ARGS true

#include <vortex/vortex.hpp>

static vortex::SysRes<vortex::None> main_w() {
    vortex::IoUring io  = TRY(vortex::IoUring::init(4, 0, 0, 0));

    const char hello [] = "Hi there!\n";
    io.write(0, 0, hello, sizeof(hello));

    io.submit_and_wait(1);
    io.deinit();

    return vortex::None();
}

static u8 main() {
    vortex::SysRes<vortex::None> res = main_w();
    if (res.is_err()) {
        vortex::println("Error: ", static_cast<u8>(res.kind));
        vortex::assert(false, "Failed to run the main function!");

        return 1;
    }

    return 0;
}
