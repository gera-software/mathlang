StrBuf usage and examples
=========================

This document shows common usage patterns and lifetime rules for the StrBuf
API, including sb_cstr (non-owning view) and sb_cstr_copy (owned copy).

Building and running tests
--------------------------

- Build and run tests:

  make test

Examples
--------

1) Short-lived view (non-owning)

```c
StrBuf sb = {0};
sb_init(&sb, 16);
sb_append(&sb, "hello");

/* sb_cstr returns a pointer to the internal buffer. It is valid only until
 * the next mutating operation on `sb`.
 */
const char *view = sb_cstr(&sb);
puts(view); // safe

/* After a mutating operation the pointer may be invalidated: */
sb_append(&sb, " world");
/* view may now be dangling; do not use it. */

sb_free(&sb);
```

2) Owned copy (persist beyond buffer mutations)

```c
StrBuf sb = {0};
sb_init(&sb, 8);
sb_append(&sb, "persistent");

char *owned = sb_cstr_copy(&sb);
if (owned == NULL) {
    /* allocation failed */
}
else {
    /* use and then free when done */
    puts(owned);
    free(owned);
}

sb_free(&sb);
```

3) Append one StrBuf into another using the shared internal append helper

```c
StrBuf dst = {0};
StrBuf src = {0};

sb_init(&dst, 8);
sb_init(&src, 8);

sb_append(&dst, "hello");
sb_append(&src, " world");

sb_append_buf(&dst, &src);
puts(dst.data);  /* prints: hello world */

sb_free(&src);
sb_free(&dst);
```

Arena usage and examples
------------------------

The arena is a simple bump allocator. It stores a contiguous byte buffer and
tracks the current write offset with `Arena.length`. New allocations always
append at the current end of the buffer; calling `arena_clear()` rewinds the
write position back to zero without freeing the backing storage.

Example: allocate and write a string

```c
Arena *arena = arena_alloc(256);
if (arena == NULL) {
    /* allocation failed */
}

char *message = arena_push(arena, 6);
if (message == NULL) {
    /* not enough capacity */
}

memcpy(message, "hello\0", 6);
puts(message);

arena_clear(arena);
arena_release(arena);
```

Example: zero-initialized array or struct

```c
Arena *arena = arena_alloc(512);
if (arena == NULL) {
    /* allocation failed */
}

int *numbers = arena_push_array_zero(arena, int, 8);
if (numbers == NULL) {
    /* insufficient space */
}

for (int i = 0; i < 8; ++i) {
    printf("%d\n", numbers[i]);
}

typedef struct Point {
    int x;
    int y;
} Point;

Point *p = arena_push_struct_zero(arena, Point);
if (p != NULL) {
    p->x = 10;
    p->y = 20;
}

arena_release(arena);
```

Notes and tips
--------------

- `arena_alloc` returns NULL if capacity is zero or if memory allocation fails.

- `arena_push` and `arena_push_zero` return NULL when the arena is NULL, the
  backing buffer is NULL, or the requested size would exceed remaining capacity.

- `arena_clear` resets the write offset (`Arena.length`) to zero; it does not
  free the buffer or shrink the capacity. This makes it cheap to reuse the same
  arena for a new pass of allocations.

- `arena_release` frees the arena buffer and then frees the Arena object itself.
  It is safe to call with NULL.

- `arena_push_array` and `arena_push_struct` are convenience helpers for typed
  allocations. Their zeroed variants (`arena_push_array_zero` and
  `arena_push_struct_zero`) zero the allocated memory before returning it.

- The arena is not thread-safe. If multiple threads access the same arena,
  protect it with a mutex or other synchronization.

- sb_cstr returns a pointer to the internal buffer (non-owning). The caller
  must not free the returned pointer.

- sb_cstr may attempt to allocate or reallocate in order to correct an
  inconsistent buffer state; it returns an empty string literal "" on error.

- sb_append_buf reuses the same internal growth/copy logic as sb_append,
  so it keeps the same capacity, NUL-termination, and error behavior.

- sb_reserve returns 0 on success and -1 on allocation failure; callers
  should check its return value when they require guaranteed capacity.

- The API is not thread-safe. If multiple threads access the same StrBuf
  instance, protect it with a mutex or other synchronization.

- If you want sb_init to report allocation failure reliably, consider
  changing its signature to return an int. Currently sb_init leaves sb->data
  == NULL on allocation failure and does not return an error code.
