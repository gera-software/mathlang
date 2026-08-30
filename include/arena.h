#pragma once
#include <stddef.h>

typedef struct Arena {
    char *buffer;   /* contiguous backing store */
    size_t capacity; /* total bytes in the buffer */
    size_t length;   /* current used bytes / bump offset */
} Arena;

/*
 * Allocate and initialize an arena with a fixed byte capacity.
 * Returns NULL on allocation failure or when capacity is zero.
 */
Arena *arena_alloc(size_t capacity);

/*
 * Release the backing buffer and destroy the arena object.
 * Safe to call with NULL.
 */
void arena_release(Arena *arena);

/*
 * Push `bytes` bytes onto the arena and return a pointer to the newly reserved
 * region. The memory is uninitialized. Returns NULL if the arena is invalid or
 * if the request would exceed the remaining capacity.
 */
void *arena_push(Arena *arena, size_t bytes);

/*
 * Like arena_push, but zeroes the newly allocated memory range before returning.
 */
void *arena_push_zero(Arena *arena, size_t bytes);

/*
 * Convenience helpers for typed allocations.
 * arena_push_array(arena, int, 8) allocates 8 ints.
 * arena_push_struct(arena, MyType) allocates a single MyType.
 */
#define arena_push_array(arena, type, count) ((type *)arena_push((arena), sizeof(type) * (count)))
#define arena_push_array_zero(arena, type, count) ((type *)arena_push_zero((arena), sizeof(type) * (count)))
#define arena_push_struct(arena, type) ((type *)arena_push((arena), sizeof(type)))
#define arena_push_struct_zero(arena, type) ((type *)arena_push_zero((arena), sizeof(type)))

/*
 * Rewind the arena by resetting its current used length to zero.
 * The backing storage remains allocated and may be reused for new allocations.
 */
void arena_clear(Arena *arena);