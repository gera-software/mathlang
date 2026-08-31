#include <criterion/criterion.h>
#include "lexer.h"

Test(peek, should_peek_char) {
    Lexer lexer = {
        .source = "(3+4)*5",
        .cursor = 0,
    };

    char c0 = lexer_peek(&lexer, 0);
    cr_assert_eq(c0, '(');

    char c1 = lexer_peek(&lexer, 1);
    cr_assert_eq(c1, '3');

    cr_assert_eq(lexer.cursor, 0);
}

Test(consume, should_consume_char) {
    Lexer lexer = {
        .source = "(3+4)*5",
        .cursor = 0,
    };

    char c0 = lexer_consume(&lexer);
    cr_assert_eq(c0, '(');

    char c1 = lexer_consume(&lexer);
    cr_assert_eq(c1, '3');

    cr_assert_eq(lexer.cursor, 2);
}

Test(next_token, empty_input) {
    Lexer lexer = {
        .source = "",
        .cursor = 0,
    };

    Token token = lexer_next_token(&lexer);

    cr_assert_eq(token.type, TOKEN_END);
    cr_assert_eq(token.value, 0);
}

Test(next_token, operators) {
    Lexer lexer = {
        .source = "+-*/",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);
    Token token4 = lexer_next_token(&lexer);

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
}

Test(next_token, parentheses) {
    Lexer lexer = {
        .source = "()",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, TOKEN_LPAREN);
    cr_assert_eq(token0.value, 0);

    cr_assert_eq(token1.type, TOKEN_RPAREN);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_END);
    cr_assert_eq(token2.value, 0);
}

Test(next_token, single_digit_number) {
    Lexer lexer = {
        .source = "5",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 5);

    cr_assert_eq(token1.type, TOKEN_END);
    cr_assert_eq(token1.value, 0);
}

Test(next_token, multi_digit_number) {
    Lexer lexer = {
        .source = "0123450",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 123450);

    cr_assert_eq(token1.type, TOKEN_END);
    cr_assert_eq(token1.value, 0);
}

Test(next_token, whitespace_skipping) {
    Lexer lexer = {
        .source = " 12 + 34 ",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 12);

    cr_assert_eq(token1.type, TOKEN_PLUS);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_NUMBER);
    cr_assert_eq(token2.value, 34);

    cr_assert_eq(token3.type, TOKEN_END);
    cr_assert_eq(token3.value, 0);
}

Test(next_token, invalid_character) {
    Lexer lexer = {
        .source = "2 $ 3",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, TOKEN_NUMBER);
    cr_assert_eq(token0.value, 2);

    cr_assert_eq(token1.type, TOKEN_INVALID);
    cr_assert_eq(token1.value, 0);

    cr_assert_eq(token2.type, TOKEN_NUMBER);
    cr_assert_eq(token2.value, 3);

    cr_assert_eq(token3.type, TOKEN_END);
    cr_assert_eq(token3.value, 0);
}

Test(next_token, mixed_expression) {
    Lexer lexer = {
        .source = "(3+4)*5",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);
    Token token4 = lexer_next_token(&lexer);
    Token token5 = lexer_next_token(&lexer);
    Token token6 = lexer_next_token(&lexer);
    Token token7 = lexer_next_token(&lexer);

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
}
