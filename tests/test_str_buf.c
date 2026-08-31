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

Test(str_buf__free, idempotent_double_free_safe) {
    StrBuf sb = {0};
    sb_init(&sb, 8);
    sb_free(&sb);
    /* second free should be safe */
    sb_free(&sb);
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

Test(str_buf__reserve, returns_codes_for_success_and_invalid_input) {
    StrBuf sb = {0};
    sb_init(&sb, 4);

    int rc = sb_reserve(&sb, 16);
    cr_expect_eq(rc, 0);
    cr_expect_eq(sb.capacity, 16);

    /* Null pointer should return -1 and not crash */
    int rc_null = sb_reserve(NULL, 8);
    cr_expect_eq(rc_null, -1);

    sb_free(&sb);
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

Test(str_buf__append_buf, should_append_contents_of_another_buffer) {
    StrBuf dst = {0};
    StrBuf src = {0};

    sb_init(&dst, 8);
    sb_init(&src, 8);

    sb_append(&dst, "hello");
    sb_append(&src, " world");

    sb_append_buf(&dst, &src);

    cr_expect_eq(dst.length, 11);
    cr_expect_geq(dst.capacity, 11);
    cr_expect_str_eq(dst.data, "hello world");

    sb_free(&src);
    sb_free(&dst);
}

Test(str_buf__append_buf, should_ignore_null_or_empty_source) {
    StrBuf dst = {0};
    StrBuf empty = {0};
    sb_init(&dst, 8);
    sb_init(&empty, 8);

    sb_append(&dst, "hello");
    sb_append_buf(&dst, NULL);
    sb_append_buf(&dst, &empty);

    cr_expect_eq(dst.length, 5);
    cr_expect_str_eq(dst.data, "hello");

    sb_free(&empty);
    sb_free(&dst);
}

Test(str_buf__append_int, should_append_signed_decimal_values) {
    StrBuf string = {0};
    sb_init(&string, 8);

    sb_append_int(&string, 0);
    sb_append_int(&string, 42);
    sb_append_int(&string, -7);

    cr_expect_eq(string.length, 5);
    cr_expect_geq(string.capacity, 5);
    cr_expect_str_eq(string.data, "042-7");

    sb_free(&string);
}

Test(str_buf__append_int, should_ignore_null_pointer) {
    sb_append_int(NULL, 123);
    cr_assert(true);
}

Test(str_buf__clear, should_reset_length_and_keep_capacity) {
    StrBuf string = {0};
    sb_init(&string, 8);

    sb_append(&string, "hello");

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

Test(str_buf__cstr, view_is_invalidated_by_mutation) {
    StrBuf sb = {0};
    sb_init(&sb, 4);

    sb_append(&sb, "ab");
    const char *view = sb_cstr(&sb);
    cr_expect_str_eq(view, "ab");

    /* mutate: cause reallocation */
    sb_append(&sb, "cdef");
    /* view may now be dangling; obtain a new view for checking the current contents */
    const char *new_view = sb_cstr(&sb);
    cr_expect_str_eq(new_view, "abcdef");

    sb_free(&sb);
}

Test(str_buf__cstr, defends_against_corrupted_length) {
    StrBuf sb = {0};
    sb_init(&sb, 4);

    sb_append(&sb, "hi"); /* length = 2 */
    /* artificially corrupt the state to simulate a bug: length > capacity */
    sb.length = sb.capacity + 10;

    /* sb_cstr should not crash; it either fixes the state or returns NULL */
    (void)sb_cstr(&sb);
    /* At minimum the call must complete without crashing. No strict expectation. */
    cr_assert(true);
    sb_free(&sb);
}

Test(str_buf__cstr_copy, returns_owned_copy_and_must_be_freed) {
    StrBuf sb = {0};
    sb_init(&sb, 8);

    sb_append(&sb, "copyme");
    char *copy = sb_cstr_copy(&sb);

    cr_assert_not_null(copy);
    cr_expect_str_eq(copy, "copyme");
    /* copy is distinct from internal pointer */
    cr_expect_neq((void *)copy, (void *)sb.data);

    /* mutate sb and ensure copy is unchanged */
    sb_append(&sb, "x");
    cr_expect_str_eq(copy, "copyme");

    free(copy);
    sb_free(&sb);
}

Test(str_buf__cstr_copy, returns_allocated_empty_for_null_or_uninitialized) {
    /* NULL StrBuf pointer returns allocated empty string */
    char *from_null = sb_cstr_copy(NULL);
    cr_expect_not_null(from_null);
    cr_expect_str_empty(from_null);
    free(from_null);

    /* Uninitialized StrBuf (sb_init with 0) */
    StrBuf sb = {0};
    sb_init(&sb, 0);
    char *from_uninit = sb_cstr_copy(&sb);
    cr_expect_not_null(from_uninit);
    cr_expect_str_empty(from_uninit);
    free(from_uninit);
}

Test(str_buf__dup, should_duplicate_non_empty_buffer) {
    StrBuf original = {0};
    sb_init(&original, 8);
    sb_append(&original, "hello");

    StrBuf *copy = sb_dup(&original);
    cr_assert_not_null(copy);
    cr_expect_eq(copy->length, original.length);
    cr_expect_eq(copy->capacity, original.capacity);
    cr_expect_not_null(copy->data);
    cr_expect_str_eq(copy->data, original.data);
    cr_expect_neq((void *)copy->data, (void *)original.data);

    /* Mutating the original should not affect the duplicate */
    sb_append(&original, " world");
    cr_expect_str_eq(copy->data, "hello");

    free(copy->data);
    free(copy);
    sb_free(&original);
}

Test(str_buf__dup, should_return_null_for_null_pointer) {
    cr_expect_null(sb_dup(NULL));
}

Test(str_buf__dup, should_duplicate_empty_buffer) {
    StrBuf original = {0};
    sb_init(&original, 0);

    StrBuf *copy = sb_dup(&original);
    cr_assert_not_null(copy);
    cr_expect_eq(copy->length, 0);
    cr_expect_eq(copy->capacity, 0);
    cr_expect_null(copy->data);

    free(copy);
    sb_free(&original);
}
