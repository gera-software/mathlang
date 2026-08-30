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

Test(arena_push_array, should_allocate_typed_array) {
    Arena *arena = arena_alloc(64);
    int *numbers = arena_push_array(arena, int, 4);

    cr_assert_not_null(numbers);

    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    numbers[3] = 40;

    cr_expect_eq(numbers[0], 10);
    cr_expect_eq(numbers[1], 20);
    cr_expect_eq(numbers[2], 30);
    cr_expect_eq(numbers[3], 40);
    cr_expect_eq(arena->length, sizeof(int) * 4);

    arena_release(arena);
}

Test(arena_push_array, should_fail_when_array_does_not_fit) {
    Arena *arena = arena_alloc(8);
    int *numbers = arena_push_array(arena, int, 4);

    cr_assert_null(numbers);
    cr_expect_eq(arena->length, 0);

    arena_release(arena);
}

Test(arena_push_array, should_ignore_null_arena) {
    int *numbers = arena_push_array(NULL, int, 4);
    cr_assert_null(numbers);
}

Test(arena_push_array_zero, should_allocate_zeroed_typed_array) {
    Arena *arena = arena_alloc(64);
    int *numbers = arena_push_array_zero(arena, int, 4);

    cr_assert_not_null(numbers);
    cr_expect_eq(arena->length, sizeof(int) * 4);
    cr_expect_eq(numbers[0], 0);
    cr_expect_eq(numbers[1], 0);
    cr_expect_eq(numbers[2], 0);
    cr_expect_eq(numbers[3], 0);

    arena_release(arena);
}

Test(arena_push_array_zero, should_fail_when_array_does_not_fit) {
    Arena *arena = arena_alloc(8);
    int *numbers = arena_push_array_zero(arena, int, 4);

    cr_assert_null(numbers);
    cr_expect_eq(arena->length, 0);

    arena_release(arena);
}

Test(arena_push_array_zero, should_ignore_null_arena) {
    int *numbers = arena_push_array_zero(NULL, int, 4);
    cr_assert_null(numbers);
}

Test(arena_suite, should_release_arena_without_leak_on_valgrind) {
    Arena *arena = arena_alloc(64);
    cr_assert_not_null(arena);

    int *numbers = arena_push_array(arena, int, 4);
    cr_assert_not_null(numbers);
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;
    numbers[3] = 4;

    arena_release(arena);
    cr_assert(true);
}

Test(arena_suite, should_clear_and_release_arena_without_leak_on_valgrind) {
    Arena *arena = arena_alloc(64);
    cr_assert_not_null(arena);

    char *text = arena_push(arena, 16);
    cr_assert_not_null(text);
    memcpy(text, "hello world", 11);

    arena_clear(arena);
    cr_assert_eq(arena->length, 0);

    arena_release(arena);
    cr_assert(true);
}

Test(arena_push_struct, should_allocate_struct) {
    typedef struct TestPoint {
        int x;
        int y;
    } TestPoint;

    Arena *arena = arena_alloc(64);
    TestPoint *point = arena_push_struct(arena, TestPoint);

    cr_assert_not_null(point);
    point->x = 7;
    point->y = 9;

    cr_expect_eq(point->x, 7);
    cr_expect_eq(point->y, 9);
    cr_expect_eq(arena->length, sizeof(TestPoint));

    arena_release(arena);
}

Test(arena_push_struct_zero, should_allocate_zeroed_struct) {
    typedef struct TestPoint {
        int x;
        int y;
    } TestPoint;

    Arena *arena = arena_alloc(64);
    TestPoint *point = arena_push_struct_zero(arena, TestPoint);

    cr_assert_not_null(point);
    cr_expect_eq(arena->length, sizeof(TestPoint));
    cr_expect_eq(point->x, 0);
    cr_expect_eq(point->y, 0);

    arena_release(arena);
}

Test(arena_push_struct_zero, should_ignore_null_arena) {
    typedef struct TestPoint {
        int x;
        int y;
    } TestPoint;

    TestPoint *point = arena_push_struct_zero(NULL, TestPoint);
    cr_assert_null(point);
}