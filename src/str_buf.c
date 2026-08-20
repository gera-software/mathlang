#include "str_buf.h"

#include <stdlib.h>
#include <string.h>

void sb_init(StrBuf *sb, size_t initial_capacity) {
    if (sb == NULL) {
        return;
    }

    sb->length = 0;
    sb->capacity = 0;
    sb->data = NULL;

    if (initial_capacity == 0) {
        return;
    }

    sb->data = malloc(initial_capacity + 1);
    if (sb->data == NULL) {
        return;
    }

    sb->data[0] = '\0';
    sb->capacity = initial_capacity;
}

void sb_free(StrBuf *sb) {
    if (sb == NULL) {
        return;
    }

    free(sb->data);
    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;
}

void sb_reserve(StrBuf *sb, size_t needed) {
    if (sb == NULL || needed <= sb->capacity) {
        return;
    }

    char *new_data = realloc(sb->data, needed + 1);
    if (new_data == NULL) {
        return;
    }

    sb->data = new_data;
    sb->capacity = needed;

    if (sb->length < sb->capacity) {
        sb->data[sb->length] = '\0';
    }
}


/**
 * Ensures the buffer has minimal capacity.
 * 
 * Used to extend buffer capacity before appending extra characters 
 */
static void sb_ensure_capacity(StrBuf *sb, size_t extra_len) {
    if(sb == NULL) return;

    size_t required = sb->length + extra_len;
    if(required <= sb->capacity) {
        return;
    }

    size_t new_capacity = sb->capacity == 0 ? 1 : sb->capacity;
    while(new_capacity < required) {
        new_capacity *= 2;
    }

    sb_reserve(sb, new_capacity);
    if (sb->data == NULL) {
        return;
    }
}

/**
 * Append bytes to the end of string buffer data
 */
static void sb_append_bytes(StrBuf *sb, const char *data, size_t len) {
    if(sb == NULL || data == NULL || len == 0) {
        return;
    }

    sb_ensure_capacity(sb, len);

    if(sb->data == NULL) {
        return;
    }

    memcpy(sb->data + sb->length, data, len);
    sb->length += len;
    sb->data[sb->length] = '\0';
}

void sb_append_char(StrBuf *sb, char c) {
    sb_append_bytes(sb, &c, 1);
}

void sb_append(StrBuf *sb, const char *str) {
    sb_append_bytes(sb, str, strlen(str));
}

void sb_clear(StrBuf *sb) {
    if (sb == NULL) {
        return;
    }

    if (sb->data != NULL) {
        sb->data[0] = '\0';
    }

    sb->length = 0;
}

