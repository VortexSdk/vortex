#define NO_ARGS true

#include <vortex/vortex.hpp>

static u8 main() {
    auto p_r = vortex::PageAllocator::init(2);
    vortex::assert(p_r.is_ok(), "Failed to allocate pages!");
    auto p        = p_r.unwrap();
    auto a        = vortex::Allocator<vortex::Arena>::init(p.len, p.ptr);

    auto string_r = vortex::BasicString<u8>::init(&a);
    vortex::assert(string_r.is_ok(), "Failed to allocate string!");
    auto string = string_r.unwrap();

    string.deinit(&a);

    return 0;
}
