#include <criterion/criterion.h>
#include "lexer.h"

Test(create_lexer, should_return_new_lexer) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "(3+ 4) *5");
    
    cr_expect_not_null(lexer);
    cr_expect_str_eq(lexer->source, "(3+ 4) *5");
    cr_expect_eq(lexer->cursor, 0);

    arena_release(a);
}

Test(peek, should_peek_char) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "(3+4)*5");

    char c0 = lexer_peek(lexer, 0);
    cr_assert_eq(c0, '(');

    char c1 = lexer_peek(lexer, 1);
    cr_assert_eq(c1, '3');

    cr_assert_eq(lexer->cursor, 0);

    arena_release(a);
}

Test(consume, should_consume_char) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "(3+4)*5");

    char c0 = lexer_consume(lexer);
    cr_assert_eq(c0, '(');

    char c1 = lexer_consume(lexer);
    cr_assert_eq(c1, '3');

    cr_assert_eq(lexer->cursor, 2);

    arena_release(a);
}

Test(next_token, empty_input) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "");

    Token token = lexer_next_token(lexer);

    cr_assert_eq(token.type, TOKEN_END);
    cr_assert_eq(token.value, 0);

    arena_release(a);
}

Test(next_token, operators) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "+-*/");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);
    Token token2 = lexer_next_token(lexer);
    Token token3 = lexer_next_token(lexer);
    Token token4 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_PLUS);
    cr_assert_eq(token0.value, 0);

    cr_assert_eq(token1.type, TOKEN_MINUS);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_STAR);
    cr_assert_eq(token2.value, 0);

    cr_assert_eq(token3.type, TOKEN_SLASH);
    cr_assert_eq(token3.value, 0);

    cr_assert_eq(token4.type, TOKEN_END);
    cr_assert_eq(token4.value, 0);

    arena_release(a);
}

Test(next_token, parentheses) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "()");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);
    Token token2 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_LPAREN);
    cr_assert_eq(token0.value, 0);

    cr_assert_eq(token1.type, TOKEN_RPAREN);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_END);
    cr_assert_eq(token2.value, 0);

    arena_release(a);
}

Test(next_token, single_digit_number) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "5");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 5);

    cr_assert_eq(token1.type, TOKEN_END);
    cr_assert_eq(token1.value, 0);

    arena_release(a);
}

Test(next_token, multi_digit_number) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "0123450");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 123450);

    cr_assert_eq(token1.type, TOKEN_END);
    cr_assert_eq(token1.value, 0);

    arena_release(a);
}

Test(next_token, whitespace_skipping) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, " 12 + 34 ");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);
    Token token2 = lexer_next_token(lexer);
    Token token3 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 12);

    cr_assert_eq(token1.type, TOKEN_PLUS);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_NUMBER);
    cr_assert_eq(token2.value, 34);

    cr_assert_eq(token3.type, TOKEN_END);
    cr_assert_eq(token3.value, 0);

    arena_release(a);
}

Test(next_token, invalid_character) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "2 $ 3");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);
    Token token2 = lexer_next_token(lexer);
    Token token3 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 2);

    cr_assert_eq(token1.type, TOKEN_INVALID);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_NUMBER);
    cr_assert_eq(token2.value, 3);

    cr_assert_eq(token3.type, TOKEN_END);
    cr_assert_eq(token3.value, 0);

    arena_release(a);
}

Test(next_token, mixed_expression) {
    Arena* a = arena_alloc(50);
    Lexer* lexer = create_lexer(a, "(3+4)*5");

    Token token0 = lexer_next_token(lexer);
    Token token1 = lexer_next_token(lexer);
    Token token2 = lexer_next_token(lexer);
    Token token3 = lexer_next_token(lexer);
    Token token4 = lexer_next_token(lexer);
    Token token5 = lexer_next_token(lexer);
    Token token6 = lexer_next_token(lexer);
    Token token7 = lexer_next_token(lexer);

    cr_assert_eq(token0.type, TOKEN_LPAREN);
    cr_assert_eq(token0.value, 0);

    cr_assert_eq(token1.type, TOKEN_NUMBER);
    cr_assert_eq(token1.value, 3);

    cr_assert_eq(token2.type, TOKEN_PLUS);
    cr_assert_eq(token2.value, 0);

    cr_assert_eq(token3.type, TOKEN_NUMBER);
    cr_assert_eq(token3.value, 4);

    cr_assert_eq(token4.type, TOKEN_RPAREN);
    cr_assert_eq(token4.value, 0);

    cr_assert_eq(token5.type, TOKEN_STAR);
    cr_assert_eq(token5.value, 0);

    cr_assert_eq(token6.type, TOKEN_NUMBER);
    cr_assert_eq(token6.value, 5);

    cr_assert_eq(token7.type, TOKEN_END);
    cr_assert_eq(token7.value, 0);

    arena_release(a);
}

Test(tokenize_, should_return_final_token_list) {
    Arena* a = arena_alloc(250);
    Lexer* lexer = create_lexer(a, "(3+4)*5");

    TokenList* token_list = tokenize(a, lexer);

    cr_expect_not_null(token_list);

    arena_release(a);
}
