#include <criterion/criterion.h>
#include "arena.h"
#include <stdio.h>
#include <string.h>

Test(arena_suite, should_create_arena_with_requested_capacity) {
    Arena *arena = arena_alloc(10);

    cr_assert_not_null(arena);
    cr_assert_not_null(arena->buffer);
    cr_assert_eq(arena->capacity, 10);
    cr_assert_eq(arena->length, 0);

    arena_release(arena);
}

Test(arena_suite, should_release_arena_without_crashing) {
    Arena *arena = arena_alloc(32);

    cr_assert_not_null(arena);
    cr_assert_not_null(arena->buffer);

    arena_release(arena);
    cr_assert(true);
}

Test(arena_suite, should_ignore_null_arena_release) {
    arena_release(NULL);
    cr_assert(true);
}

Test(arena_push_, should_push_number_to_stack) {
    Arena *arena = arena_alloc(10);

    int *item = arena_push(arena, sizeof(int));

    cr_assert_not_null(item);
    *item = 2;
    cr_expect_eq(*item, 2);
    cr_expect_eq(arena->capacity, 10);
    cr_expect_eq(arena->length, sizeof(int));

    arena_release(arena);
}

Test(arena_push_, should_push_string_to_stack) {
    Arena *arena = arena_alloc(32);

    char string[] = "Hello World!";
    size_t string_size = strlen(string) + 1;
    char *item = arena_push(arena, string_size);

    cr_assert_not_null(item);
    strcpy(item, string);

    cr_expect_str_eq(item, "Hello World!");
    cr_expect_eq(arena->length, string_size);

    arena_release(arena);
}

Test(arena_push_, should_push_multiple_items_to_stack) {
    Arena *arena = arena_alloc(32);

    int *item1_p = arena_push(arena, sizeof(int));
    cr_assert_not_null(item1_p);
    *item1_p = 100;

    int *item2_p = arena_push(arena, sizeof(int));
    cr_assert_not_null(item2_p);
    *item2_p = 200;

    int *item3_p = arena_push(arena, sizeof(int));
    cr_assert_not_null(item3_p);
    *item3_p = 300;

    cr_expect_eq(*item1_p, 100);
    cr_expect_eq(*item2_p, 200);
    cr_expect_eq(*item3_p, 300);

    arena_release(arena);
}

Test(arena_push_, should_fail_when_capacity_is_exceeded) {
    Arena *arena = arena_alloc(4);

    void *item = arena_push(arena, 8);

    cr_assert_null(item);
    cr_expect_eq(arena->length, 0);

    arena_release(arena);
}

Test(arena_push_, should_ignore_null_arena) {
    cr_assert_null(arena_push(NULL, sizeof(int)));
}

Test(arena_push_zero, should_zero_new_memory) {
    Arena *arena = arena_alloc(16);
    int *item = arena_push_zero(arena, sizeof(int));

    cr_assert_not_null(item);
    cr_expect_eq(arena->length, sizeof(int));
    cr_expect_eq(*item, 0);

    arena_release(arena);
}

Test(arena_clear, should_reset_used_length_to_zero) {
    Arena *arena = arena_alloc(32);
    int *value = arena_push(arena, sizeof(int));

    cr_assert_not_null(value);
    *value = 42;
    cr_expect_eq(arena->length, sizeof(int));

    arena_clear(arena);

    cr_expect_eq(arena->length, 0);
    cr_expect_not_null(arena->buffer);
    cr_expect_eq(arena->capacity, 32);

    arena_release(arena);
}

Test(arena_clear, should_allow_reusing_the_arena_after_reset) {
    Arena *arena = arena_alloc(16);
    int *first = arena_push(arena, sizeof(int));
    cr_assert_not_null(first);
    *first = 123;

    arena_clear(arena);

    int *second = arena_push(arena, sizeof(int));
    cr_assert_not_null(second);
    *second = 456;

    cr_expect_eq(*second, 456);
    cr_expect_eq(arena->length, sizeof(int));

    arena_release(arena);
}

Test(arena_clear, should_ignore_null_pointer) {
    arena_clear(NULL);
    cr_assert(true);
}