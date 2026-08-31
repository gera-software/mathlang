#include <criterion/criterion.h>
#include "parser.h"

Test(peek, should_peek_token) {
    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 200,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 300,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    Token* expected0 = parser_peek(&parser, 0);
    cr_expect_eq(expected0->type, TOKEN_NUMBER);
    cr_expect_eq(expected0->value, 100);

    Token* expected1 = parser_peek(&parser, 1);
    cr_expect_eq(expected1->type, TOKEN_NUMBER);
    cr_expect_eq(expected1->value, 200);

    cr_assert_eq(parser.cursor, 0);
}

Test(consume, should_consume_token) {
    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 200,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 300,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    Token* expected0 = parser_consume(&parser);
    cr_expect_eq(expected0->type, TOKEN_NUMBER);
    cr_expect_eq(expected0->value, 100);

    Token* expected1 = parser_consume(&parser);
    cr_expect_eq(expected1->type, TOKEN_NUMBER);
    cr_expect_eq(expected1->value, 200);

    cr_assert_eq(parser.cursor, 2);
}

Test(parse_suite, empty_input) {
    Token array[] = {0};
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* root = parse(&parser);

    cr_assert_eq(root, NULL);
}