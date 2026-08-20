#include <criterion/criterion.h>
#include <stdio.h>
#include "str_buf.h"

Test(str_buf__init, should_init) {
    size_t string_size = 8;
    StrBuf string = {0};
    sb_init(&string, string_size);

    cr_expect_eq(string.capacity, string_size);
    cr_expect_eq(string.length, 0);
    cr_expect_not_null(string.data);
    cr_expect_str_empty(string.data);
}

Test(str_buf__init, capacity_should_be_zero_when_uninitialized) {
    size_t string_size = 0;
    StrBuf string = {0};
    sb_init(&string, string_size);

    cr_expect_eq(string.capacity, 0);
    cr_expect_eq(string.length, 0);
    cr_expect_null(string.data);
}

Test(str_buf__free, should_free_buffer_and_reset_state) {
    StrBuf string = {0};
    sb_init(&string, 8);

    cr_expect_not_null(string.data);
    cr_expect_eq(string.capacity, 8);
    cr_expect_eq(string.length, 0);

    sb_free(&string);

    cr_expect_null(string.data);
    cr_expect_eq(string.capacity, 0);
    cr_expect_eq(string.length, 0);
}

Test(str_buf__free, should_ignore_null_pointer) {
    sb_free(NULL);
    cr_assert(true);
}

Test(str_buf__append_char, should_append_characters_in_order) {
    StrBuf string = {0};
    sb_init(&string, 8);

    sb_append_char(&string, 'h');
    sb_append_char(&string, 'i');
    sb_append_char(&string, '!');

    cr_expect_eq(string.length, 3);
    cr_expect_eq(string.capacity, 8);
    cr_expect_str_eq(string.data, "hi!");
}

Test(str_buf__append_char, should_ignore_null_pointer) {
    sb_append_char(NULL, 'x');
    cr_assert(true);
}

Test(str_buf__clear, should_reset_length_and_keep_capacity) {
    StrBuf string = {0};
    sb_init(&string, 8);

    // TODO replace to append char
    snprintf(string.data, string.capacity + 1, "hello");
    string.length = 5;

    sb_clear(&string);

    cr_expect_eq(string.length, 0);
    cr_expect_eq(string.capacity, 8);
    cr_expect_not_null(string.data);
    cr_expect_str_empty(string.data);
}

Test(str_buf__clear, should_ignore_null_pointer) {
    sb_clear(NULL);
    cr_assert(true);
}
