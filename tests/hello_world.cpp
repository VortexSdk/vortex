#include <asm/unistd_64.h>
#include <vortex/vortex.hpp>

static auto main() -> u8 {
    const char* msg = "Hello World!\n";

    vortex::syscall(__NR_write, 0, reinterpret_cast<usize>(msg), 13);

    return 0;
}
