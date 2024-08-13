#include "arena.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static Region *create_region_(void) {
   Region *region = malloc(sizeof(Region));
   if(!region) {
       fprintf(stderr, "[ARENA ERROR]: Failed to allocate Region memory.\n");
       return NULL;
   }
   region->count = 0;
   region->next = NULL;
   return region;
}

void *arena_allocate(Arena *arena, size_t size) {
    assert(size != 0);
    assert(size <= REGION_CAPACITY);
    if(arena->region_count == 0) {
        arena->first = create_region_();
        arena->region_count++;
    }

    void *ret = NULL;
    Region *current_region = arena->first;
    Region *last_valid_region = current_region;
    do {
        if(sizeof(current_region->memory) - current_region->count >= size) {
            ret = current_region->memory + current_region->count;
            current_region->count += size;
            break;
        }
        if(current_region->next != NULL) {
            last_valid_region = current_region->next;
        }
        current_region = current_region->next;
    } while(current_region != NULL);

    if(!ret) {
        last_valid_region->next = create_region_();
        arena->region_count++;
        current_region = last_valid_region->next;
        ret = current_region->memory + current_region->count;
        current_region->count += size;
    }
    return ret;
}

void arena_free(Arena *arena) {
    Region *current = arena->first;
    while(current) {
        Region *region_to_free = current;
        current = current->next;
        free(region_to_free);
    }
    arena->first = NULL;
}
