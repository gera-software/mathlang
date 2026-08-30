#pragma once
#include <stddef.h>

typedef struct Arena {
    char *buffer;
    size_t capacity;
    size_t length;
} Arena;

// create or destroy a 'stack' - an "arena"
Arena *arena_alloc(size_t capacity);
void arena_release(Arena *arena);

// push some bytes onto the 'stack' - the way to allocate
void *arena_push(Arena *arena, size_t bytes);
void *arena_push_zero(Arena *arena, size_t bytes);

// some macro helpers that I've found nice:
#define arena_push_array(arena, type, count) ((type *)arena_push((arena), sizeof(type) * (count)))
#define arena_push_array_zero(arena, type, count) ((type *)arena_push_zero((arena), sizeof(type) * (count)))
#define arena_push_struct(arena, type) ((type *)arena_push((arena), sizeof(type)))
#define arena_push_struct_zero(arena, type) ((type *)arena_push_zero((arena), sizeof(type)))

// pop some bytes off the 'stack' - the way to free
// void arena_pop(Arena *arena, size_t bytes);

// get the # of bytes currently allocated.
// size_t arena_get_pos(Arena *arena);

// also some useful popping helpers:
// void arena_set_pos_back(Arena *arena, size_t pos);
void arena_clear(Arena *arena);