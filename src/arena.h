#include <stddef.h>
#include <stdlib.h>

#define ARENA_SIZE (2 << 11)
#define ARENA_ALIGN _Alignof(max_align_t)

typedef struct arena arena;
struct arena {
    void   *start;
    size_t  cursor;
    arena  *next;
};

arena *init_arena();
void *arena_alloc(arena *a, unsigned long size);
void arena_free(arena *a);
