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

// Test(lexer_suite, lex_end_token) {
//     TokenList list = lex("+");

//     cr_assert_eq(list.count, 2);
//     cr_assert_eq(list.tokens[1].type, END);
//     cr_assert_eq(list.tokens[1].line, 1);
//     cr_assert_eq(list.tokens[1].column, 2);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_plus_token) {
//     TokenList list = lex("+");

//     cr_assert_eq(list.tokens[0].type, PLUS);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_minus_token) {
//     TokenList list = lex("-");

//     cr_assert_eq(list.tokens[0].type, MINUS);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_star_token) {
//     TokenList list = lex("*");

//     cr_assert_eq(list.tokens[0].type, STAR);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_slash_token) {
//     TokenList list = lex("/");

//     cr_assert_eq(list.tokens[0].type, SLASH);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_lparent_token) {
//     TokenList list = lex("(");

//     cr_assert_eq(list.tokens[0].type, LPAREN);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_rparent_token) {
//     TokenList list = lex(")");

//     cr_assert_eq(list.tokens[0].type, RPAREN);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_0_token) {
//     TokenList list = lex("0");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 0);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_1_token) {
//     TokenList list = lex("1");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 1);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_2_token) {
//     TokenList list = lex("2");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 2);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_3_token) {
//     TokenList list = lex("3");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 3);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_4_token) {
//     TokenList list = lex("4");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 4);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_5_token) {
//     TokenList list = lex("5");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 5);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_6_token) {
//     TokenList list = lex("6");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 6);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_7_token) {
//     TokenList list = lex("7");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 7);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_8_token) {
//     TokenList list = lex("8");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 8);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_single_digit_9_token) {
//     TokenList list = lex("9");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 9);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_multi_digit_token) {
//     TokenList list = lex("001950");
//     cr_assert_eq(list.tokens[0].type, DIGIT);
//     cr_assert_eq(list.tokens[0].value, 1950);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_skips_whitespace) {
//     TokenList list = lex(" +");

//     // cr_assert_eq(list.count, 1);
//     cr_assert_eq(list.tokens[0].type, PLUS);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 2);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_tracks_newlines_and_multiple_tokens) {
//     TokenList list = lex("+\n-");

//     // cr_assert_eq(list.count, 2);
//     cr_assert_eq(list.tokens[0].type, PLUS);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);
//     cr_assert_eq(list.tokens[1].type, MINUS);
//     cr_assert_eq(list.tokens[1].line, 2);
//     cr_assert_eq(list.tokens[1].column, 1);

//     free_tokens(&list);
// }