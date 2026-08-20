#pragma once
#include <stddef.h>

/*
 * StrBuf -- a simple growable string buffer.
 *
 * Invariants:
 *  - sb->length <= sb->capacity
 *  - sb->data is either NULL (uninitialized / empty) or points to a
 *    heap allocation of size sb->capacity + 1 (space for terminating NUL).
 *
 * Threading:
 *  - The API is not thread-safe. Concurrent access requires external
 *    synchronization by the caller.
 */

typedef struct StrBuf {
    char *data;
    size_t length;
    size_t capacity;
} StrBuf;

/*
 * Initialize a StrBuf.
 *
 * - `initial_capacity` specifies the initial allocated capacity (not
 *   counting the terminating NUL). If 0, no allocation is performed and
 *   sb->data will be NULL.
 * - This function does not report allocation failure; if allocation fails
 *   sb->data will remain NULL and sb->capacity will be 0. Callers that need
 *   to observe allocation failures should check sb->data after calling.
 */
void sb_init(StrBuf *sb, size_t initial_capacity);

/*
 * Free the buffer and reset fields. Safe to call with sb == NULL.
 *
 * After return: sb->data == NULL, sb->length == 0, sb->capacity == 0.
 */
void sb_free(StrBuf *sb);

/**
 * Ensure the buffer has at least `needed` bytes of capacity (not counting
 * the terminating NUL).
 *
 * Behavior:
 *  - Grows the buffer capacity preserving existing contents.
 *  - Never shrinks the capacity.
 *  - Ensures there is room for a terminating '\0' byte at index capacity.
 *
 * Return:
 *  - 0 on success.
 *  - -1 on allocation failure or if `sb` is NULL.
 *
 * On failure the StrBuf state is left as it was prior to the call (no
 * partial shrink/resize). Callers relying on growth must check the
 * return value.
 */
int sb_reserve(StrBuf *sb, size_t needed);

/**
 * Reset the buffer length to zero while retaining the allocated capacity.
 *
 * After this call:
 *  - sb->length == 0
 *  - sb->data is unchanged (except the first byte is set to '\0' if data != NULL)
 *  - sb->capacity is unchanged
 *
 * Safe to call with sb == NULL (no-op).
 */
void sb_clear(StrBuf *sb);

/**
 * Append a character to the existing buffer.
 *
 * Behavior:
 *  - Attempts to grow the buffer as needed. If allocation fails during
 *    growth the append is aborted and the buffer is left unchanged.
 *  - The function is a no-op if sb == NULL.
 */
void sb_append_char(StrBuf *sb, char c);

/**
 * Append a NUL-terminated C string to the existing buffer.
 *
 * Behavior and failure modes are the same as sb_append_char. Passing a
 * NULL `str` is treated as a no-op.
 */
void sb_append(StrBuf *sb, const char *str);

// void  sb_append_n(StrBuf *sb, const char *str, size_t n);
// void  sb_appendf(StrBuf *sb, const char *fmt, ...);

/**
 * Return a pointer to a NUL-terminated C string view of the buffer.
 *
 * Contract and lifetime:
 *  - Returns a pointer to the internal buffer (sb->data). The pointer is
 *    non-owning: the caller must not free it.
 *  - The pointer is valid only until the next mutating operation on `sb`
 *    (append, reserve, clear, free, etc.), or until `sb` is freed.
 *
 * Allocation behavior and error handling:
 *  - sb_cstr may attempt to allocate (or reallocate) to correct an
 *    inconsistent state where sb->length > sb->capacity. For that reason
 *    the parameter is non-const.
 *  - If allocation fails or sb->data is NULL, the function returns NULL.
 *
 * Thread safety:
 *  - Not thread-safe. Concurrent mutation and read may cause data races and
 *    undefined behavior.
 */
const char *sb_cstr(StrBuf *sb);

/**
 * Return a newly-allocated NUL-terminated copy of the buffer contents.
 *
 * Ownership:
 *  - The returned pointer is owned by the caller and must be freed with free().
 *
 * Return:
 *  - On success returns a malloc()'d buffer containing the string.
 *  - Returns NULL if allocation fails.
 *  - If `sb` is NULL or uninitialized (sb->data == NULL) this returns an
 *    allocated empty string (i.e., equivalent to strdup("")) on success,
 *    or NULL on allocation failure.
 */
char *sb_cstr_copy(const StrBuf *sb);
 