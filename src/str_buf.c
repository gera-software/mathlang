#include "str_buf.h"

#include <stdlib.h>

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

void sb_append_char(StrBuf *sb, char c) {
    if(sb == NULL) {
        return;
    }

    sb->data[sb->length] = c;
    sb->length++;
    sb->data[sb->length] = '\0';
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

