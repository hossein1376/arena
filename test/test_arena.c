#include <criterion/criterion.h>
#include <stdint.h>
#include "../src/arena.h"

// --- Test cases ---

Test(arena, alloc_returns_non_null) {
    arena *a = init_arena();
    void *ptr = arena_alloc(a, 64);
    cr_assert_not_null(ptr, "Expected a non‑NULL pointer for the arena");
}

/*
 * Since a fresh arena instance is created at the start of this test, it's
 * assumed that it has enough free space to allocate all 10 bytes consecutively.
 */
Test(arena, small_allocs_pack_tightly) {
    arena *a = init_arena();
    void *prev = NULL;
    for (int i = 0; i < 10; i++) {
        void *p = arena_alloc(a, 1);
        cr_assert_not_null(p);
        if (prev != NULL) {
            cr_assert_eq((char*)p, (char*)prev + 1,
            "Consecutive addresses should be exactly 1 byte apart");
        }
        prev = p;
    }
}

Test(arena, large_alloc_is_8byte_aligned) {
    arena *a = init_arena();
    // Create a known misaligned state
    arena_alloc(a, 1);
    void *p = arena_alloc(a, 16);
    cr_assert(
        (uintptr_t)p % 8 == 0,
        "16‑byte allocation must be 8‑byte aligned");
}

Test(arena, returns_null_when_size_is_too_large) {
    arena *a = init_arena();
    void *ptr = arena_alloc(a, ARENA_SIZE*2);
    cr_assert_null(ptr, "Should return NULL when size is too large");
}

Test(arena, allocates_new_block_if_not_enough_space) {
    arena *a = init_arena();
    void *ptr1 = arena_alloc(a, ARENA_SIZE);
    cr_assert_not_null(ptr1, "Should not return NULL for first allocation");
    void *ptr2 = arena_alloc(a, ARENA_SIZE);
    cr_assert_not_null(ptr2, "Should not return NULL for second allocation");
}


Test(arena, arena_free_should_release_all_resources) {
    arena *a = init_arena();
    void *ptr1 = arena_alloc(a, ARENA_SIZE/2);
    cr_assert_not_null(ptr1, "Should not return NULL for first allocation");
    void *ptr2 = arena_alloc(a, ARENA_SIZE/2);
    cr_assert_not_null(ptr2, "Should not return NULL for second allocation");
    void *ptr3 = arena_alloc(a, ARENA_SIZE);
    cr_assert_not_null(ptr3, "Should not return NULL for third allocation");

    arena_free(a);
}
