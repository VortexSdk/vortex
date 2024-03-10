#include <vortex/vortex.h>

void test_arena(void) {
    Allocator allocator;
    PageAllocator page   = page_init(20);
    ArenaAllocator arena = arena_init(page.mem, page.len);

    void* ptr1;
    void* ptr2;
    void* ptr3;
    void* ptr4;
    usize expected_arena_pos;

    assert(arena.mem != nullptr, "Failed to initiate an arena. Null pointer received.\n");

    allocator = arena_allocator(&arena);
    ptr1      = mem_alloc(allocator, 200);
    ptr2      = mem_alloc(allocator, 50);
    mem_free(allocator, ptr1, 200);

    ptr3               = mem_alloc(allocator, 540);
    ptr4               = mem_resize(allocator, ptr3, 540, 900);
    expected_arena_pos = mem_align_len(200) + mem_align_len(50) + mem_align_len(900);

    assert(arena.pos == expected_arena_pos, "Invalid arena length management.\n");

    page_deinit(&page);
}

__attribute__((noreturn)) extern void _start(void) {
    test_arena();

    syscall(SYS_exit_group, 1, 0);

    __builtin_unreachable();
}
