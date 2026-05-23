#include <stddef.h>
#include <stdlib.h>

#include "./arena.h"

/*
 * arena_alloc(arena *, size) – Returns a pointer to a memory location with at
 * least the requested size allocated. If the requested size is bigger than a
 * single block (ARENA_SIZE), it returns NULL. It will allocate new blocks if
 * needed.
 */
void *arena_alloc(arena *a, size_t size) {
    if (a == NULL || size == 0 || size > ARENA_SIZE)
        return NULL;

    while (1) {
        size_t current = a->cursor;
        // align to ARENA_ALIGN bytes
        size_t aligned = (current + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);

        if (aligned + size <= ARENA_SIZE) {
            a->cursor = aligned + size;
            return (char *)a->start + aligned;
        }

        // Not enough room, try next arena
        if (!a->next) {
            a->next = init_arena();
            if (!a->next) return NULL;    // out of memory
        }
        a = a->next;
    }
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
