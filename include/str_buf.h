/**
 * @file str_buf.h
 * @brief Growable string buffer (StrBuf) API.
 *
 * StrBuf provides a small, efficient, heap-backed growable string buffer
 * with a lightweight API for appending and obtaining C-string views.
 */

#pragma once
#include <stddef.h>

/**
 * @brief StrBuf -- a simple growable string buffer.
 *
 * @invariant sb->length <= sb->capacity
 * @invariant sb->data is either NULL (uninitialized/empty) or points to a
 *            heap allocation of size sb->capacity + 1 (space for terminating NUL).
 *
 * @note The API is not thread-safe. Concurrent access requires external
 *       synchronization by the caller.
 */

typedef struct StrBuf {
    char *data;     /**< Heap buffer holding the characters (may be NULL). */
    size_t length;  /**< Number of valid characters in the buffer (not including NUL). */
    size_t capacity;/**< Allocated capacity (number of characters, not counting NUL). */
} StrBuf;
/**
 * @brief Initialize a StrBuf.
 *
 * @param sb Pointer to StrBuf to initialize.
 * @param initial_capacity Initial allocated capacity (not counting NUL). If
 *        zero, no allocation is performed and sb->data will be NULL.
 *
 * @note This function does not report allocation failure. If allocation
 *       fails sb->data will remain NULL and sb->capacity will be 0. Callers
 *       that need to observe allocation failures should check sb->data.
 */
void sb_init(StrBuf *sb, size_t initial_capacity);

/**
 * @brief Free the buffer and reset fields.
 *
 * @param sb StrBuf pointer (may be NULL).
 *
 * After return: sb->data == NULL, sb->length == 0, sb->capacity == 0.
 */
void sb_free(StrBuf *sb);

/**
 * @brief Ensure the buffer has at least `needed` bytes of capacity (not
 * counting the terminating NUL).
 *
 * Behavior:
 *  - Grows the buffer capacity preserving existing contents.
 *  - Never shrinks the capacity.
 *  - Ensures there is room for a terminating '\0' byte at index capacity.
 *
 * @param sb StrBuf pointer (must not be NULL).
 * @param needed Requested capacity (number of characters, not counting NUL).
 * @return 0 on success, -1 on allocation failure or invalid input.
 *
 * @note On failure the StrBuf state is left as it was prior to the call (no
 * partial shrink/resize). Callers relying on growth must check the return
 * value.
 */
int sb_reserve(StrBuf *sb, size_t needed);

/**
 * @brief Reset the buffer length to zero while retaining the allocated
 * capacity.
 *
 * @param sb StrBuf pointer (may be NULL).
 *
 * After this call:
 *  - sb->length == 0
 *  - sb->data is unchanged (except the first byte is set to '\0' if data != NULL)
 *  - sb->capacity is unchanged
 */
void sb_clear(StrBuf *sb);

/**
 * @brief Append a character to the existing buffer.
 *
 * @param sb StrBuf pointer (may be NULL).
 * @param c Character to append.
 *
 * Behavior:
 *  - Attempts to grow the buffer as needed. If allocation fails during
 *    growth the append is aborted and the buffer is left unchanged.
 *  - The function is a no-op if sb == NULL.
 */
void sb_append_char(StrBuf *sb, char c);

/**
 * @brief Append a NUL-terminated C string to the existing buffer.
 *
 * @param sb StrBuf pointer (may be NULL).
 * @param str NUL-terminated C string to append; passing NULL is a no-op.
 *
 * Behavior and failure modes are the same as sb_append_char.
 */
void sb_append(StrBuf *sb, const char *str);

/**
 * @brief Append the contents of one StrBuf to another.
 *
 * This function appends the raw bytes stored in `src->data` to `dst` without
 * modifying `src`. It reuses the internal `sb_append_bytes()` logic, so it
 * follows the same growth, copy, and terminating-NUL behavior as the other
 * append functions.
 *
 * @param dst Destination StrBuf pointer (may be NULL).
 * @param src Source StrBuf pointer (may be NULL). Empty or uninitialized
 *            buffers are treated as a no-op.
 */
void sb_append_buf(StrBuf *dst, const StrBuf *src);

// void  sb_append_n(StrBuf *sb, const char *str, size_t n);
// void  sb_appendf(StrBuf *sb, const char *fmt, ...);

/**
 * @brief Return a pointer to a NUL-terminated C string view of the buffer.
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
 *
 * @param sb StrBuf pointer.
 * @return Pointer to internal NUL-terminated string, or NULL on error.
 */
const char *sb_cstr(StrBuf *sb);

/**
 * @brief Return a newly-allocated NUL-terminated copy of the buffer contents.
 *
 * Ownership:
 *  - The returned pointer is owned by the caller and must be freed with free().
 *
 * @param sb StrBuf pointer (may be NULL).
 * @return malloc()'d copy on success, NULL on allocation failure. If `sb`
 *         is NULL or uninitialized (sb->data == NULL) this returns an
 *         allocated empty string (equivalent to strdup("")) on success.
 */
char *sb_cstr_copy(const StrBuf *sb);

/**
 * @brief Return a deep copy of the given StrBuf.
 *
 * Ownership:
 *  - The returned pointer is owned by the caller and must be freed with free()
 *    for the StrBuf itself.
 *  - If copy->data != NULL, that heap buffer must also be freed with free().
 *
 * Example cleanup:
 *  - StrBuf *copy = sb_dup(&sb);
 *  - if (copy != NULL) {
 *      free(copy->data);
 *      free(copy);
 *    }
 *
 * @param sb StrBuf pointer (may be NULL).
 * @return A newly allocated duplicate of `sb`, or NULL if `sb` is NULL or an
 *         allocation fails.
 */
StrBuf *sb_dup(StrBuf *sb);

/**
 * @brief Print the contents of the buffer to standard output (for debug).
 *
 * The function writes the current buffer contents as a C string without adding
 * a trailing newline.
 *
 * @param sb StrBuf pointer (may be NULL).
 *
 * @note If `sb` is NULL or `sb->data` is NULL, the function is a no-op.
 */
void sb_print(StrBuf *sb);
 