#define NO_ARGS true

#include <vortex/vortex.hpp>

static u8 main() {
    const char *msg = "Hello World!\n";
    vortex::write(0, msg, 13);

    return 0;
}
