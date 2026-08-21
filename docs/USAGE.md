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

Notes and tips
--------------

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
