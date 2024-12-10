#define NO_ARGS true

#include <vortex/vortex.hpp>

static vortex::SysRes<vortex::None> main_w() {
    vortex::PageAllocator p            = TRY(vortex::PageAllocator::init(2));
    vortex::Allocator<vortex::Arena> a = vortex::Allocator<vortex::Arena>::init(p.len, p.ptr);

    // Test initialization
    vortex::Map<int, long> map         = TRY((vortex::Map<int, long>::init(&a, 32)));

    // Triggering Rehash
    for (usize i = 1; i < 100; i++) {
        const int key    = static_cast<int>(i);
        const long value = -static_cast<long>(i);
        vortex::assert(
            map.insert(&a, key, value) == vortex::MapInsertRes::Success, "Failed to insert."
        );
        vortex::assert(map.get(key) != NULL, "A NULL value received from Map.");
        vortex::assert(*map.get(key) == value, "Invalid value received from Map inside the loop.");
    }

    vortex::assert(map.insert(&a, 6, -9) == vortex::MapInsertRes::Success, "Failed to insert.");
    vortex::assert(map.get(6) != NULL, "A NULL value received from Map.");
    vortex::assert(*map.get(6) == -9, "Invalid value received from Map.");
    vortex::assert(map.insert(&a, 6, -95) == vortex::MapInsertRes::Success, "Failed to update.");
    vortex::assert(map.get(6) != NULL, "A NULL value received from Map after updating.");
    vortex::assert(*map.get(6) == -95, "Invalid value received from Map after updating.");
    vortex::assert(map.remove(6) == false, "Failed to remove a value from the map.");
    vortex::assert(
        map.remove(100000) == true, "Map somehow successfully deleted an entry that doesn't exist."
    );
    vortex::assert(map.get(6) == NULL, "Map doesn't return null for a deleted entry.");

    // Clean up
    map.deinit(&a);
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
