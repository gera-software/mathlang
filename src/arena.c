#include "arena.h"
#include <stdlib.h>
#include <string.h>

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
    if (arena == NULL || arena->buffer == NULL) {
        return NULL;
    }

    if (bytes == 0) {
        return arena->buffer + arena->length;
    }

    if (arena->length > arena->capacity || bytes > (arena->capacity - arena->length)) {
        return NULL;
    }

    char *pos = arena->buffer + arena->length;
    arena->length += bytes;
    return pos;
}

void *arena_push_zero(Arena *arena, size_t bytes) {
    char *ptr = arena_push(arena, bytes);
    if (ptr == NULL) {
        return NULL;
    }

    memset(ptr, 0, bytes);
    return ptr;
}