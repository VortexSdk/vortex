#define NO_ARGS true

#include <vortex/vortex.hpp>

template <typename T> static bool is_aligned(const T *ptr) {
    return (reinterpret_cast<usize>(ptr) % alignof(T)) == 0;
}

static u8 main() {
    auto p_r = vortex::PageAllocator::init(2);
    vortex::assert(p_r.is_ok(), "Failed to allocate pages!");

    auto p = p_r.unwrap();
    vortex::assert(p.len == 2 * PAGE_SIZE, "Invalid number of allocated pages!");

    auto a        = vortex::Allocator<vortex::Arena>::init(p.len, p.ptr);

    const auto m1 = a.alloc<u8>(25);
    vortex::assert(m1.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<u8>(m1.ptr), "Unaligned memory!");
    vortex::assert(a.state.pos == 25 * sizeof(u8), "Invalid Arena position!");

    const auto m2 = a.alloc<vortex::PageAllocator>(172);
    vortex::assert(m2.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<vortex::PageAllocator>(m2.ptr), "Unaligned memory!");
    vortex::assert(
        a.state.pos == vortex::alignForward(
                           (25 * sizeof(u8)) + (172 * sizeof(vortex::PageAllocator)),
                           alignof(vortex::PageAllocator)
                       ),
        "Invalid Arena position!"
    );

    const auto m3 = a.alloc<float>(172);
    vortex::assert(m3.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<float>(m3.ptr), "Unaligned memory!");

    const auto m4 = a.alloc<long double>(172);
    vortex::assert(m4.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<long double>(m4.ptr), "Unaligned memory!");

    p.deinit();
    return 0;
}
