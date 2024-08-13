#ifndef ARENA_H_
#define ARENA_H_

#include <stddef.h>
#include <stdint.h>

#define REGION_CAPACITY 4096

typedef struct Region Region;

struct Region {
    size_t count;
    Region *next;
    uint8_t memory[REGION_CAPACITY];
};

typedef struct {
    Region *first;
    size_t region_count;
} Arena;

void *arena_allocate(Arena *arena, size_t size);
void arena_free(Arena *arena);

#endif // ARENA_H_
