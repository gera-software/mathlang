#include "arena.h"
#include <stdlib.h>

Arena *arena_alloc(size_t capacity) {
    Arena *arena = malloc(sizeof(Arena));
    if(arena == NULL) {
        return NULL;
    }
    if(capacity == 0) {
        return NULL;
    }

    arena->buffer = malloc(sizeof(char) * capacity);
    if(arena->buffer == NULL) {
        return NULL;
    }
    arena->capacity = capacity;
    arena->length = 0;
    
    return arena;
}

void arena_release(Arena *arena) {
    if(arena == NULL){
        return;
    }
    if(arena->buffer != NULL) {
        free(arena->buffer);
    }
    arena->capacity = 0;
    arena->length = 0;
    free(arena);
}

void *arena_push(Arena *arena, size_t bytes) {
    char *pos = (arena->buffer + arena->length);
    arena->length += bytes;
    return pos;
}