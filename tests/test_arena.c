#include <criterion/criterion.h>
#include "arena.h"

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