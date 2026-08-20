#pragma once
#include <stddef.h>

typedef struct StrBuf {
    char *data;
    size_t length;
    size_t capacity;
} StrBuf;

void sb_init(StrBuf *sb, size_t initial_capacity);
void sb_free(StrBuf *sb);
/**
 * Resize buffer com a capacidade desejada
 * 
 * - Reserve grows capacity while preserving the current string
 * - Reserve should not shrink
 * - Handles '\0' implicitly
 */
void sb_reserve(StrBuf *sb, size_t needed);
/**
 * Append a character to the existing buffer
 */
void sb_append_char(StrBuf *sb, char c);
/**
 * Append a C string to the existing buffer
 */
void sb_append(StrBuf *sb, const char *str);
/**
 * Reset length but keep capacity
 */
void sb_clear(StrBuf *sb);