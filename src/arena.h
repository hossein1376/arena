#include <stddef.h>
#include <stdlib.h>

#define ARENA_SIZE (2 << 11)

void *arena_alloc(unsigned long size);
void arena_free();
static void init_arena();

typedef struct {
    void   *start;
    size_t cursor;
} arena;
