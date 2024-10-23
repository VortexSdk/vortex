#define NO_ARGS true

#include <vortex/vortex.hpp>

static u8 main() {
    auto p_r = vortex::PageAllocator::init(2);
    vortex::assert(p_r.is_ok(), "Failed to allocate pages!");
    auto p       = p_r.unwrap();
    auto a       = vortex::Allocator<vortex::Arena>::init(p.len, p.ptr);

    // Test initialization
    auto map_res = vortex::Map<int, long>::init(&a, 32);
    vortex::assert(map_res.is_ok(), "Map initialization should succeed!");
    auto map = map_res.unwrap();

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

    return 0;
}
