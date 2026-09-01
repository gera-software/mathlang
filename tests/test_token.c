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
    cr_expect_eq(token_list->arena, a);
     
    arena_release(a);
}

Test(token_suite, should_push_token_to_list) {
    Arena *a = arena_alloc(1024);

    TokenList* token_list = create_token_list(a, 10);

    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });
    
    cr_expect_eq(token_list->list[0].type, TOKEN_PLUS);
    cr_expect_eq(token_list->length, 1);

    arena_release(a);
}