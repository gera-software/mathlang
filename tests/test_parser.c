#include <criterion/criterion.h>
#include "parser.h"
#include "arena.h"
#include "ast.h"

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

Test(_parse_number, should_return_number_node) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_number(arena, &parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser.cursor, 1);
    arena_release(arena);
}

Test(_parse_number, should_return_null) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_PLUS,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_number(arena, &parser);
    cr_expect_eq(expected0, NULL);

    cr_assert_eq(parser.cursor, 0);
    arena_release(arena);
}

Test(_parse_unary_minus, should_return_negative_number_op) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_MINUS,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_unary_minus(arena, &parser);
    cr_expect_eq(expected0->type, NODE_NEG);
    cr_expect_eq(expected0->data.unary.operand->type, NODE_INT);
    cr_expect_eq(expected0->data.unary.operand->data.value, 100);

    cr_assert_eq(parser.cursor, 2);
    arena_release(arena);
}

Test(_parse_unary_minus, should_return_null) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_unary_minus(arena, &parser);
    cr_expect_eq(expected0, NULL);

    cr_assert_eq(parser.cursor, 0);
    arena_release(arena);
}

Test(_parse_factor, should_return_number_node) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_factor(arena, &parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser.cursor, 1);
    arena_release(arena);
}

Test(_parse_factor, should_return_negative_number_op) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_MINUS,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_factor(arena, &parser);
    cr_expect_eq(expected0->type, NODE_NEG);
    cr_expect_eq(expected0->data.unary.operand->type, NODE_INT);
    cr_expect_eq(expected0->data.unary.operand->data.value, 100);

    cr_assert_eq(parser.cursor, 2);
    arena_release(arena);
}

Test(_parse_term, should_return_number) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_END,
        }
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_term(arena, &parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser.cursor, 1);
    arena_release(arena);
}

Test(_parse_term, should_return_multiplication_op) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_STAR,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 5,
        },
        {
            .type = TOKEN_END,
        }
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_term(arena, &parser);
    cr_expect_eq(expected0->type, NODE_MUL);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser.cursor, 3);
    arena_release(arena);
}

Test(_parse_term, should_return_division_op) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_SLASH,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 5,
        },
        {
            .type = TOKEN_END,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_term(arena, &parser);
    cr_expect_eq(expected0->type, NODE_DIV);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser.cursor, 3);
    arena_release(arena);
}

Test(_parse_term, should_return_associativity_tree) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 10,
        },
        {
            .type = TOKEN_STAR,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 5,
        },
        {
            .type = TOKEN_SLASH,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 2,
        },
        {
            .type = TOKEN_END,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_term(arena, &parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected0);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "((10 MUL 5) DIV 2)");

    cr_expect_eq(expected0->type, NODE_DIV);
    cr_expect_eq(expected0->data.op.left->type, NODE_MUL);
    cr_expect_eq(expected0->data.op.left->data.op.left->data.value, 10);
    cr_expect_eq(expected0->data.op.left->data.op.right->data.value, 5);
    cr_expect_eq(expected0->data.op.right->data.value, 2);

    cr_assert_eq(parser.cursor, 5);

    sb_free(&sb);
    arena_release(arena);
}

Test(_parse_expression, should_return_number) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_END,
        }
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_expression(arena, &parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser.cursor, 1);
    arena_release(arena);
}

Test(_parse_expression, should_return_addition_op) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_PLUS
        },
        {
            .type = TOKEN_NUMBER,
            .value = 5,
        },
        {
            .type = TOKEN_END,
        }
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_expression(arena, &parser);
    cr_expect_eq(expected0->type, NODE_ADD);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser.cursor, 3);
    arena_release(arena);
}

Test(_parse_expression, should_return_subtraction_op) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 100,
        },
        {
            .type = TOKEN_MINUS
        },
        {
            .type = TOKEN_NUMBER,
            .value = 5,
        },
        {
            .type = TOKEN_END,
        }
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_expression(arena, &parser);
    cr_expect_eq(expected0->type, NODE_SUB);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser.cursor, 3);
    arena_release(arena);
}

Test(_parse_expression, should_return_associativity_tree) {
    Arena *arena = arena_alloc(1024);

    Token array[] = { 
        {
            .type = TOKEN_NUMBER,
            .value = 10,
        },
        {
            .type = TOKEN_PLUS,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 5,
        },
        {
            .type = TOKEN_MINUS,
        },
        {
            .type = TOKEN_NUMBER,
            .value = 2,
        },
        {
            .type = TOKEN_END,
        },
    }; 
    Parser parser = {
        .tokens = array,
        .cursor = 0,
    };

    ASTNode* expected0 = parse_expression(arena, &parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected0);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "((10 ADD 5) SUB 2)");

    cr_expect_eq(expected0->type, NODE_SUB);
    cr_expect_eq(expected0->data.op.left->type, NODE_ADD);
    cr_expect_eq(expected0->data.op.left->data.op.left->data.value, 10);
    cr_expect_eq(expected0->data.op.left->data.op.right->data.value, 5);
    cr_expect_eq(expected0->data.op.right->data.value, 2);

    cr_assert_eq(parser.cursor, 5);

    sb_free(&sb);
    arena_release(arena);
}