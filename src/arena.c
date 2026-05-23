#include <stddef.h>
#include <stdlib.h>

#include "./arena.h"

static arena *arena_inst = NULL;

static void init_arena() {
    if (arena_inst) return;
    arena *a = (arena *)malloc(sizeof *a);
    if (!a) return;
    void *start = malloc(ARENA_SIZE);
    if (!start) {
        free(a);
        return;
    }
    a->start = start;
    a->cursor = 0;
    arena_inst = a;
}

/*
 * arena_alloc(size) – Allocate memory from the global arena.
 *
 * Alignment rule:
 *   We align to an 8‑byte boundary only when doing so wastes fewer
 *   bytes than the allocation itself. If padding >= size, the block
 *   is packed at the current cursor with no alignment guarantee.
 *
 * This keeps tiny allocations dense (e.g. 1‑byte requests use exactly
 * 1 byte) while still providing natural alignment for larger objects.
 * Callers must respect the actual alignment of the returned pointer;
 * no `max_align_t` guarantee is made.
 */
void *arena_alloc(size_t size) {
    if (arena_inst == NULL) init_arena();
    if (arena_inst == NULL) return NULL;

    size_t current = arena_inst->cursor;
    size_t aligned = (current + 7) & ~7;
    size_t padding = aligned - current;

    // Skip alignment if padding is greater than or equal to the requested size.
    if (padding >= size) {
        aligned = current;
    }

    if (aligned + size > ARENA_SIZE) {
        // TODO: chain arenas
        return NULL;
    }

    arena_inst->cursor = aligned + size;
    return (char *)arena_inst->start + aligned;
}

void arena_free() {
    if (arena_inst == NULL) return;
    free(arena_inst->start);
    free(arena_inst);
    arena_inst = NULL;
}
