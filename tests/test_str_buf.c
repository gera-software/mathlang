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

Test(str_buf__reserve, should_increase_capacity_and_keep_existing_data) {
    StrBuf string = {0};
    sb_init(&string, 8);

    snprintf(string.data, string.capacity + 1, "hi");
    string.length = 2;

    sb_reserve(&string, 16);

    cr_expect_eq(string.capacity, 16);
    cr_expect_eq(string.length, 2);
    cr_expect_not_null(string.data);
    cr_expect_str_eq(string.data, "hi");
}

Test(str_buf__reserve, should_not_shrink_capacity) {
    StrBuf string = {0};
    sb_init(&string, 8);

    snprintf(string.data, string.capacity + 1, "hi");
    string.length = 2;

    sb_reserve(&string, 7);

    cr_expect_eq(string.capacity, 8);
    cr_expect_eq(string.length, 2);
    cr_expect_not_null(string.data);
    cr_expect_str_eq(string.data, "hi");
}

Test(str_buf__reserve, should_ignore_zero_size) {
    StrBuf string = {0};
    sb_init(&string, 8);

    size_t previous_capacity = string.capacity;
    char *previous_data = string.data;

    sb_reserve(&string, 0);

    cr_expect_eq(string.capacity, previous_capacity);
    cr_expect_eq((void *)string.data, (void *)previous_data);
}

Test(str_buf__reserve, should_ignore_null_pointer) {
    sb_reserve(NULL, 4);
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

Test(str_buf__append_char, should_grow_buffer_when_capacity_is_exhausted) {
    StrBuf string = {0};
    sb_init(&string, 1);

    sb_append_char(&string, 'a');
    sb_append_char(&string, 'b');
    sb_append_char(&string, 'c');

    cr_expect_eq(string.length, 3);
    cr_expect_geq(string.capacity, 3);
    cr_expect_str_eq(string.data, "abc");
}

Test(str_buf__append_char, should_grow_before_appending_when_new_length_less_then_capacity) {
    StrBuf string = {0};
    sb_init(&string, 1);

    sb_append_char(&string, 'a');
    sb_append_char(&string, 'b');

    cr_expect_eq(string.length, 2);
    cr_expect_geq(string.capacity, 2);
    cr_expect_str_eq(string.data, "ab");
}

Test(str_buf__append_char, should_ignore_null_pointer) {
    sb_append_char(NULL, 'x');
    cr_assert(true);
}

Test(str_buf__append, should_append_a_string_to_empty_buffer) {
    StrBuf string = {0};
    sb_init(&string, 2);

    sb_append(&string, "hello");

    cr_expect_eq(string.length, 5);
    cr_expect_geq(string.capacity, 5);
    cr_expect_str_eq(string.data, "hello");
}

Test(str_buf__append, should_append_to_existing_content) {
    StrBuf string = {0};
    sb_init(&string, 4);

    sb_append(&string, "hi");
    sb_append(&string, " there");

    cr_expect_eq(string.length, 8);
    cr_expect_geq(string.capacity, 8);
    cr_expect_str_eq(string.data, "hi there");
}

Test(str_buf__append, should_ignore_empty_string) {
    StrBuf string = {0};
    sb_init(&string, 8);

    sb_append(&string, "");

    cr_expect_eq(string.length, 0);
    cr_expect_eq(string.capacity, 8);
    cr_expect_str_empty(string.data);
}

Test(str_buf__append, should_reallocate_when_additional_text_exceeds_capacity) {
    StrBuf string = {0};
    sb_init(&string, 2);

    sb_append(&string, "hi");
    sb_append(&string, " there");

    cr_expect_eq(string.length, 8);
    cr_expect_gt(string.capacity, 2);
    cr_expect_geq(string.capacity, 8);
    cr_expect_str_eq(string.data, "hi there");
}

Test(str_buf__append, should_ignore_null_pointer) {
    sb_append(NULL, "hello");
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


Test(str_buf__cstr, should_return_cstring_and_point_to_internal_data) {
    StrBuf string = {0};
    sb_init(&string, 8);

    sb_append(&string, "hello");

    const char *cstr = sb_cstr(&string);

    cr_expect_not_null(cstr);
    cr_expect_str_eq(cstr, "hello");
    /* returned pointer should be the internal buffer */
    cr_expect_eq((void *)cstr, (void *)string.data);
}

Test(str_buf__cstr, should_return_null_for_null_or_uninitialized_buffer) {
    cr_expect_null(sb_cstr(NULL));

    StrBuf string = {0};
    sb_init(&string, 0); /* leaves data == NULL */
    cr_expect_null(sb_cstr(&string));
}

Test(str_buf__cstr, should_ensure_null_termination_when_length_equals_capacity) {
    StrBuf string = {0};
    sb_init(&string, 3);

    /* Fill buffer exactly to capacity (without using sb_append to simulate edge) */
    if (string.data != NULL) {
        snprintf(string.data, string.capacity + 1, "abc");
        string.length = 3;
    }

    const char *cstr = sb_cstr(&string);
    cr_expect_not_null(cstr);
    cr_expect_str_eq(cstr, "abc");
    /* explicit null terminator at sb->length must be present */
    if (string.data != NULL) {
        cr_expect_eq(string.data[string.length], '\0');
    }
}

