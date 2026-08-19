#include <criterion/criterion.h>
#include "lexer.h"

Test(lexer_suite, empty_input) {
    Lexer lexer = {
        .source = "",
        .cursor = 0,
    };

    Token token = lexer_next_token(&lexer);

    cr_assert_eq(token.type, END);
    cr_assert_eq(token.value, 0);
    cr_assert_eq(token.column, 0);
}

Test(lexer_suite, operators) {
    Lexer lexer = {
        .source = "+-*/",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);
    Token token4 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, PLUS);
    cr_assert_eq(token0.value, 0);
    cr_assert_eq(token0.column, 0);

    cr_assert_eq(token1.type, MINUS);
    cr_assert_eq(token1.value, 0);
    cr_assert_eq(token1.column, 1);

    cr_assert_eq(token2.type, STAR);
    cr_assert_eq(token2.value, 0);
    cr_assert_eq(token2.column, 2);

    cr_assert_eq(token3.type, SLASH);
    cr_assert_eq(token3.value, 0);
    cr_assert_eq(token3.column, 3);

    cr_assert_eq(token4.type, END);
    cr_assert_eq(token4.value, 0);
    cr_assert_eq(token4.column, 4);
}

Test(lexer_suite, parentheses) {
    Lexer lexer = {
        .source = "()",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, LPAREN);
    cr_assert_eq(token0.value, 0);
    cr_assert_eq(token0.column, 0);

    cr_assert_eq(token1.type, RPAREN);
    cr_assert_eq(token1.value, 0);
    cr_assert_eq(token1.column, 1);

    cr_assert_eq(token2.type, END);
    cr_assert_eq(token2.value, 0);
    cr_assert_eq(token2.column, 2);
}

Test(lexer_suite, single_digit_number) {
    Lexer lexer = {
        .source = "5",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, NUMBER);
    cr_assert_eq(token0.value, 5);
    cr_assert_eq(token0.column, 0);

    cr_assert_eq(token1.type, END);
    cr_assert_eq(token1.value, 0);
    cr_assert_eq(token1.column, 1);
}

Test(lexer_suite, multi_digit_number) {
    Lexer lexer = {
        .source = "0123450",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, NUMBER);
    cr_assert_eq(token0.value, 123450);
    cr_assert_eq(token0.column, 0);

    cr_assert_eq(token1.type, END);
    cr_assert_eq(token1.value, 0);
    cr_assert_eq(token1.column, 7);
}

Test(lexer_suite, whitespace_skipping) {
    Lexer lexer = {
        .source = " 12 + 34 ",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, NUMBER);
    cr_assert_eq(token0.value, 12);
    cr_assert_eq(token0.column, 1);

    cr_assert_eq(token1.type, PLUS);
    cr_assert_eq(token1.value, 0);
    cr_assert_eq(token1.column, 4);

    cr_assert_eq(token2.type, NUMBER);
    cr_assert_eq(token2.value, 34);
    cr_assert_eq(token2.column, 6);

    cr_assert_eq(token3.type, END);
    cr_assert_eq(token3.value, 0);
    cr_assert_eq(token3.column, 9);
}

Test(lexer_suite, invalid_character) {
    Lexer lexer = {
        .source = "2 $ 3",
        .cursor = 0,
    };

    Token token0 = lexer_next_token(&lexer);
    Token token1 = lexer_next_token(&lexer);
    Token token2 = lexer_next_token(&lexer);
    Token token3 = lexer_next_token(&lexer);

    cr_assert_eq(token0.type, NUMBER);
    cr_assert_eq(token0.value, 2);
    cr_assert_eq(token0.column, 0);

    cr_assert_eq(token1.type, INVALID);
    cr_assert_eq(token1.value, 0);
    cr_assert_eq(token1.column, 2);

    cr_assert_eq(token2.type, NUMBER);
    cr_assert_eq(token2.value, 3);
    cr_assert_eq(token2.column, 4);

    cr_assert_eq(token3.type, END);
    cr_assert_eq(token3.value, 0);
    cr_assert_eq(token3.column, 5);
}

Test(lexer_suite, mixed_expression) {
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

    cr_assert_eq(token0.type, LPAREN);
    cr_assert_eq(token0.value, 0);
    cr_assert_eq(token0.column, 0);

    cr_assert_eq(token1.type, NUMBER);
    cr_assert_eq(token1.value, 3);
    cr_assert_eq(token1.column, 1);

    cr_assert_eq(token2.type, PLUS);
    cr_assert_eq(token2.value, 0);
    cr_assert_eq(token2.column, 2);

    cr_assert_eq(token3.type, NUMBER);
    cr_assert_eq(token3.value, 4);
    cr_assert_eq(token3.column, 3);

    cr_assert_eq(token4.type, RPAREN);
    cr_assert_eq(token4.value, 0);
    cr_assert_eq(token4.column, 4);

    cr_assert_eq(token5.type, STAR);
    cr_assert_eq(token5.value, 0);
    cr_assert_eq(token5.column, 5);

    cr_assert_eq(token6.type, NUMBER);
    cr_assert_eq(token6.value, 5);
    cr_assert_eq(token6.column, 6);

    cr_assert_eq(token7.type, END);
    cr_assert_eq(token7.value, 0);
    cr_assert_eq(token7.column, 7);
}