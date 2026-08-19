#include <criterion/criterion.h>
#include "lexer.h"

Test(lexer_suite, lex_invalid_token) {
    TokenList list = lex("a");

    cr_assert_eq(list.tokens[0].type, INVALID);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_end_token) {
    TokenList list = lex("+");

    cr_assert_eq(list.count, 2);
    cr_assert_eq(list.tokens[1].type, END);
    cr_assert_eq(list.tokens[1].line, 1);
    cr_assert_eq(list.tokens[1].column, 2);

    free_tokens(&list);
}

Test(lexer_suite, lex_plus_token) {
    TokenList list = lex("+");

    cr_assert_eq(list.tokens[0].type, PLUS);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_minus_token) {
    TokenList list = lex("-");

    cr_assert_eq(list.tokens[0].type, MINUS);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_star_token) {
    TokenList list = lex("*");

    cr_assert_eq(list.tokens[0].type, STAR);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_slash_token) {
    TokenList list = lex("/");

    cr_assert_eq(list.tokens[0].type, SLASH);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_lparent_token) {
    TokenList list = lex("(");

    cr_assert_eq(list.tokens[0].type, LPAREN);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_rparent_token) {
    TokenList list = lex(")");

    cr_assert_eq(list.tokens[0].type, RPAREN);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_0_token) {
    TokenList list = lex("0");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 0);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_1_token) {
    TokenList list = lex("1");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 1);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_2_token) {
    TokenList list = lex("2");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 2);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_3_token) {
    TokenList list = lex("3");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 3);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_4_token) {
    TokenList list = lex("4");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 4);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_5_token) {
    TokenList list = lex("5");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 5);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_6_token) {
    TokenList list = lex("6");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 6);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_7_token) {
    TokenList list = lex("7");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 7);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_8_token) {
    TokenList list = lex("8");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 8);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_single_digit_9_token) {
    TokenList list = lex("9");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 9);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

Test(lexer_suite, lex_multi_digit_token) {
    TokenList list = lex("001950");
    cr_assert_eq(list.tokens[0].type, DIGIT);
    cr_assert_eq(list.tokens[0].value, 1950);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);

    free_tokens(&list);
}

// Test(lexer_suite, lex_end_token) {
//     TokenList list = lex("0");

//     cr_assert_eq(list.count, 1);
//     cr_assert_eq(list.tokens[0].type, END);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }

Test(lexer_suite, lex_skips_whitespace) {
    TokenList list = lex(" +");

    // cr_assert_eq(list.count, 1);
    cr_assert_eq(list.tokens[0].type, PLUS);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 2);

    free_tokens(&list);
}

Test(lexer_suite, lex_tracks_newlines_and_multiple_tokens) {
    TokenList list = lex("+\n-");

    // cr_assert_eq(list.count, 2);
    cr_assert_eq(list.tokens[0].type, PLUS);
    cr_assert_eq(list.tokens[0].line, 1);
    cr_assert_eq(list.tokens[0].column, 1);
    cr_assert_eq(list.tokens[1].type, MINUS);
    cr_assert_eq(list.tokens[1].line, 2);
    cr_assert_eq(list.tokens[1].column, 1);

    free_tokens(&list);
}

// Test(lexer_suite, lex_parses_digit_and_parentheses) {
//     TokenList list = lex("(0)");

//     // cr_assert_eq(list.count, 3);
//     cr_assert_eq(list.tokens[0].type, LPAREN);
//     cr_assert_eq(list.tokens[1].type, DIGIT);
//     cr_assert_eq(list.tokens[2].type, RPAREN);

//     free_tokens(&list);
// }

// Test(lexer_suite, lex_marks_unknown_characters_as_invalid) {
//     TokenList list = lex("x");

//     cr_assert_eq(list.count, 1);
//     cr_assert_eq(list.tokens[0].type, INVALID);
//     cr_assert_eq(list.tokens[0].line, 1);
//     cr_assert_eq(list.tokens[0].column, 1);

//     free_tokens(&list);
// }