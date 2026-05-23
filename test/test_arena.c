#include <criterion/criterion.h>
#include <stdint.h>
#include "../src/arena.h"

void setup(void) {}

void teardown(void) {
    // Reset the arena so tests don’t interfere.
    arena_free();
}

// --- Test cases ---

Test(arena, alloc_returns_non_null) {
    void *ptr = arena_alloc(64);
    cr_assert_not_null(ptr, "Expected a non‑NULL pointer for the arena");
}

Test(arena, small_allocs_pack_tightly) {
    void *prev = NULL;
    for (int i = 0; i < 10; i++) {
        void *p = arena_alloc(1);
        cr_assert_not_null(p);
        if (prev != NULL) {
            cr_assert_eq((char*)p, (char*)prev + 1,
            "Consecutive addresses should be exactly 1 byte apart");
        }
        prev = p;
    }
}

Test(arena, large_alloc_is_8byte_aligned) {
    // Create a known misaligned state
    arena_alloc(1);
    void *p = arena_alloc(16);
    cr_assert(
        (uintptr_t)p % 8 == 0,
        "16‑byte allocation must be 8‑byte aligned");
}

Test(arena, returns_null_when_exhausted) {
    // ARENA_SIZE assumed small; request more than the whole arena.
    void *ptr = arena_alloc(ARENA_SIZE*2);
    cr_assert_null(ptr, "Should return NULL when arena is full");
}
