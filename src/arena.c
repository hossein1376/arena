#include <stddef.h>
#include <stdlib.h>

#include "./arena.h"

/*
 * arena_alloc(arena *, size) – Returns a pointer to a memory location with at
 * least the requested size allocated. If the requested size is bigger than a
 * single block (ARENA_SIZE), it returns NULL. It will allocate new blocks if
 * needed.
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
void *arena_alloc(arena *a, size_t size) {
    if (a == NULL) return NULL;
    if (size > ARENA_SIZE) return NULL;   // too large for any single arena

    size_t current = a->cursor;
    size_t aligned = (current + 7) & ~7;
    size_t padding = aligned - current;

    // Skip alignment if padding is greater than or equal to the requested size.
    if (padding >= size) {
        aligned = current;
    }

    if (aligned + size > ARENA_SIZE) {
        if (!a->next) a->next = init_arena();
        return arena_alloc(a->next, size);
    }

    a->cursor = aligned + size;
    return (char *)a->start + aligned;
}

void arena_free(arena *a) {
    while (a) {
        arena *next = a->next;
        free(a->start);
        free(a);
        a = next;
    }
}

arena *init_arena() {
    arena *a = (arena *)malloc(sizeof *a);
    if (!a) return NULL;
    void *start = malloc(ARENA_SIZE);
    if (!start) {
        free(a);
        return NULL;
    }
    a->start = start;
    a->cursor = 0;
    a->next= NULL;
    return a;
}
