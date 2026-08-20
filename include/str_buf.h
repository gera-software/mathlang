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
 * Reset length but keep capacity
 */
void sb_clear(StrBuf *sb);
void sb_append_char(StrBuf *sb, char c);