#include <asm/unistd_64.h>
#include <vortex/vortex.hpp>

static u8 main() {
    auto p   = vortex::PageAllocator(8);
    auto p_r = p.init();
    vortex::assert(p_r.is_ok(), "Failed to allocated pages!");
    vortex::assert(p.len == 8 * PAGE_SIZE, "Invalid number of allocated pages!");

    auto a        = vortex::Allocator<vortex::Arena>(p.ptr, p.len);

    const auto m1 = a.alloc<u8>(25);
    vortex::assert(m1.ptr != NULL, "Failed to allocated memory from the Arena!");
    vortex::assert(a.state.pos == 25 * sizeof(u8), "Invalid Arena position!");

    const auto m2 = a.alloc<vortex::PageAllocator>(172);
    vortex::assert(m2.ptr != NULL, "Failed to allocated memory from the Arena!");
    vortex::assert(
        a.state.pos == (25 * sizeof(u8)) + (172 * sizeof(vortex::PageAllocator)),
        "Invalid Arena position!"
    );

    return 0;
}
