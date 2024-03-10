#include "ring/ring.h"
#include "vortex/vortex.h"

void test_ring(void) {
    void* thread = allocate_ring_thread(1);
    assert(thread != nullptr, "Failed to initiate a ring thread. Null pointer received.\n");
}

__attribute__((noreturn)) extern void _start(void) {
    test_ring();

    SYSCALL(SYS_exit_group, 1, 0);

    __builtin_unreachable();
}
