#include <criterion/criterion.h>
#include "token.h"
#include "arena.h"

Test(token_suite, should_create_token_list_with_requested_capacity) {
    Arena *a = arena_alloc(1024);

    TokenList* token_list = create_token_list(a, 10);

    cr_expect_not_null(token_list);
    cr_expect_eq(token_list->capacity, 10);
    cr_expect_eq(token_list->length, 0);
    cr_expect_not_null(token_list->list);
     
    arena_release(a);
}