#define NO_ARGS true

#include <vortex/vortex.hpp>

static u8 main() {
    const char *msg = "Hello World!\n";

    vortex::syscall(__NR_write, 0, reinterpret_cast<usize>(msg), 13);

    return 0;
}
