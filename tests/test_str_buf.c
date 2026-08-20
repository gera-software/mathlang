#include <criterion/criterion.h>
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