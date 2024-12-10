#define NO_ARGS true

#include <vortex/vortex.hpp>

template <typename T> static bool is_aligned(const T *ptr) {
    return (reinterpret_cast<usize>(ptr) % alignof(T)) == 0;
}

static vortex::SysRes<vortex::None> main_w() {
    vortex::PageAllocator p = TRY(vortex::PageAllocator::init(2));
    vortex::assert(p.len == 2 * PAGE_SIZE, "Invalid number of allocated pages!");

    vortex::Allocator<vortex::Arena> a = vortex::Allocator<vortex::Arena>::init(p.len, p.ptr);

    const vortex::Slice<u8> m1         = a.alloc<u8>(25);
    vortex::assert(m1.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<u8>(m1.ptr), "Unaligned memory!");
    vortex::assert(a.state.pos == 25 * sizeof(u8), "Invalid Arena position!");

    const vortex::Slice<vortex::PageAllocator> m2 = a.alloc<vortex::PageAllocator>(172);
    vortex::assert(m2.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<vortex::PageAllocator>(m2.ptr), "Unaligned memory!");
    vortex::assert(
        a.state.pos == vortex::alignForward(
                           (25 * sizeof(u8)) + (172 * sizeof(vortex::PageAllocator)),
                           alignof(vortex::PageAllocator)
                       ),
        "Invalid Arena position!"
    );

    const vortex::Slice<float> m3 = a.alloc<float>(172);
    vortex::assert(m3.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<float>(m3.ptr), "Unaligned memory!");

    const vortex::Slice<long double> m4 = a.alloc<long double>(172);
    vortex::assert(m4.ptr != NULL, "Failed to allocate memory from the Arena!");
    vortex::assert(is_aligned<long double>(m4.ptr), "Unaligned memory!");

    p.deinit();

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
