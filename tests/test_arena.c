#include <criterion/criterion.h>
#include "arena.h"
#include <stdio.h>

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

    *item = 2;
    cr_expect_eq(*item, 2);
    cr_expect_eq(arena->capacity, 10);
    cr_expect_eq(arena->length, sizeof(int));

    arena_release(arena);
}

Test(arena_push_, should_push_string_to_stack) {
    Arena *arena = arena_alloc(10);

    char string[] = "Hello World!";
    int string_size = strlen(string) + 1; // includes null terminator character
    char *item = arena_push(arena, string_size);
    strcpy(item, string);

    cr_expect_eq(arena->length, string_size);

    arena_release(arena);
}

Test(arena_push_, should_push_multiple_items_to_stack) {
    Arena *arena = arena_alloc(32);

    int *item1_p = arena_push(arena, sizeof(int));
    *item1_p = 100;
    int *item2_p = arena_push(arena, sizeof(int));
    *item2_p = 200;
    int *item3_p = arena_push(arena, sizeof(int));
    *item3_p = 300;

    cr_expect_eq(*item1_p, 100);
    cr_expect_eq(*item2_p, 200);
    cr_expect_eq(*item3_p, 300);

    arena_release(arena);
}