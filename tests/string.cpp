#define NO_ARGS true

#include <vortex/vortex.hpp>

static vortex::SysRes<vortex::None> main_w() {
    vortex::PageAllocator p            = TRY(vortex::PageAllocator::init(2));
    vortex::Allocator<vortex::Arena> a = vortex::Allocator<vortex::Arena>::init(p.len, p.ptr);

    vortex::BasicString<u8> string     = TRY(vortex::BasicString<u8>::init(&a));
    string.deinit(&a);

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
