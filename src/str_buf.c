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

int sb_reserve(StrBuf *sb, size_t needed) {
    if (sb == NULL || needed <= sb->capacity) {
        return 0;
    }

    char *new_data = realloc(sb->data, needed + 1);
    if (new_data == NULL) {
        return -1;
    }

    sb->data = new_data;
    sb->capacity = needed;

    if (sb->length < sb->capacity) {
        sb->data[sb->length] = '\0';
    }

    return 0;
}


/**
 * Ensures the buffer has minimal capacity.
 * 
 * Used to extend buffer capacity before appending extra characters
 * Returns 0 on success, -1 on allocation failure or invalid input.
 */
static int sb_ensure_capacity(StrBuf *sb, size_t extra_len) {
    if(sb == NULL) return -1;

    size_t required = sb->length + extra_len;
    if(required <= sb->capacity) {
        return 0;
    }

    size_t new_capacity = sb->capacity == 0 ? 1 : sb->capacity;
    while(new_capacity < required) {
        new_capacity *= 2;
    }

    if (sb_reserve(sb, new_capacity) != 0) {
        return -1;
    }

    if (sb->data == NULL) {
        return -1;
    }

    return 0;
}

/**
 * Append bytes to the end of string buffer data
 */
static void sb_append_bytes(StrBuf *sb, const char *data, size_t len) {
    if(sb == NULL || data == NULL || len == 0) {
        return;
    }

    if (sb_ensure_capacity(sb, len) != 0) {
        return;
    }

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

const char *sb_cstr(StrBuf *sb) {
    if (sb == NULL || sb->data == NULL) {
        return NULL;
    }

    /* If length somehow exceeds capacity, try to reserve enough space.
     * This is a defensive check; normally length should never be > capacity.
     */
    if (sb->length > sb->capacity) {
        if (sb_reserve(sb, sb->length) != 0) {
            return NULL;
        }
        /* Defensive check: if still not enough capacity, avoid OOB write */
        if (sb->length > sb->capacity) {
            return NULL;
        }
    }

    sb->data[sb->length] = '\0';
    return (const char *)sb->data;
}

char *sb_cstr_copy(const StrBuf *sb) {
    if (sb == NULL || sb->data == NULL) {
        char *empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t len = sb->length;
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, sb->data, len);
    copy[len] = '\0';
    return copy;
}

