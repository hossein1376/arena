#include <stddef.h>
#include <stdlib.h>

#define ARENA_SIZE (2 << 11)

typedef struct arena arena;
struct arena {
    void   *start;
    size_t  cursor;
    arena  *next;
};

arena *init_arena();
void *arena_alloc(arena *a, unsigned long size);
void arena_free(arena *a);
