#include <criterion/criterion.h>
#include "parser.h"
#include "arena.h"
#include "ast.h"

Test(parser, should_create_parser) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    Parser* parser = create_parser(arena, token_list);

    cr_expect_not_null(parser);
    cr_expect_eq(parser->cursor, 0);
    cr_expect_eq(parser->tokens, token_list);

    arena_release(arena);
}

Test(peek, should_peek_token) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 200 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 300 });

    Parser* parser = create_parser(arena, token_list);

    Token* expected0 = parser_peek(parser, 0);
    cr_expect_eq(expected0->type, TOKEN_NUMBER);
    cr_expect_eq(expected0->value, 100);

    Token* expected1 = parser_peek(parser, 1);
    cr_expect_eq(expected1->type, TOKEN_NUMBER);
    cr_expect_eq(expected1->value, 200);

    cr_assert_eq(parser->cursor, 0);

    arena_release(arena);
}

Test(consume, should_consume_token) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 200 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 300 });

    Parser* parser = create_parser(arena, token_list);

    Token* expected0 = parser_consume(parser);
    cr_expect_eq(expected0->type, TOKEN_NUMBER);
    cr_expect_eq(expected0->value, 100);

    Token* expected1 = parser_consume(parser);
    cr_expect_eq(expected1->type, TOKEN_NUMBER);
    cr_expect_eq(expected1->value, 200);

    cr_assert_eq(parser->cursor, 2);

    arena_release(arena);
}

Test(_parse_number, should_return_number_node) {
    Arena *arena = arena_alloc(1024);
 
    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_number(arena, parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser->cursor, 1);
    arena_release(arena);
}

Test(_parse_number, should_return_null) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_number(arena, parser);
    cr_expect_eq(expected0, NULL);

    cr_assert_eq(parser->cursor, 0);
    arena_release(arena);
}

Test(_parse_unary_minus, should_return_negative_number_op) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_unary_minus(arena, parser);
    cr_expect_eq(expected0->type, NODE_NEG);
    cr_expect_eq(expected0->data.unary.operand->type, NODE_INT);
    cr_expect_eq(expected0->data.unary.operand->data.value, 100);

    cr_assert_eq(parser->cursor, 2);
    arena_release(arena);
}

Test(_parse_unary_minus, should_return_null) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_unary_minus(arena, parser);
    cr_expect_eq(expected0, NULL);

    cr_assert_eq(parser->cursor, 0);
    arena_release(arena);
}

Test(_parse_factor, should_return_number_node) {
    Arena *arena = arena_alloc(1024);


    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    
    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_factor(arena, parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser->cursor, 1);
    arena_release(arena);
}

Test(_parse_factor, should_return_negative_number_op) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_factor(arena, parser);
    cr_expect_eq(expected0->type, NODE_NEG);
    cr_expect_eq(expected0->data.unary.operand->type, NODE_INT);
    cr_expect_eq(expected0->data.unary.operand->data.value, 100);

    cr_assert_eq(parser->cursor, 2);
    arena_release(arena);
}

Test(_parse_term, should_return_number) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_term(arena, parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser->cursor, 1);
    arena_release(arena);
}

Test(_parse_term, should_return_multiplication_op) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_STAR, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_term(arena, parser);
    cr_expect_eq(expected0->type, NODE_MUL);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser->cursor, 3);
    arena_release(arena);
}

Test(_parse_term, should_return_division_op) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_SLASH, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_term(arena, parser);
    cr_expect_eq(expected0->type, NODE_DIV);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser->cursor, 3);
    arena_release(arena);
}

Test(_parse_term, should_return_associativity_tree) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 10 });
    token_list_push(token_list, (Token) { TOKEN_STAR, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_SLASH, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 2 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_term(arena, parser);

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

    cr_assert_eq(parser->cursor, 5);

    sb_free(&sb);
    arena_release(arena);
}

Test(_parse_expression, should_return_number) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_expression(arena, parser);
    cr_expect_eq(expected0->type, NODE_INT);
    cr_expect_eq(expected0->data.value, 100);

    cr_assert_eq(parser->cursor, 1);
    arena_release(arena);
}

Test(_parse_expression, should_return_addition_op) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_expression(arena, parser);
    cr_expect_eq(expected0->type, NODE_ADD);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser->cursor, 3);
    arena_release(arena);
}

Test(_parse_expression, should_return_subtraction_op) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 100 });
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_expression(arena, parser);
    cr_expect_eq(expected0->type, NODE_SUB);
    cr_expect_eq(expected0->data.op.left->data.value, 100);
    cr_expect_eq(expected0->data.op.right->data.value, 5);

    cr_assert_eq(parser->cursor, 3);
    arena_release(arena);
}

Test(_parse_expression, should_return_associativity_tree) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 10 });
    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 2 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected0 = parse_expression(arena, parser);

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

    cr_assert_eq(parser->cursor, 5);

    sb_free(&sb);
    arena_release(arena);
}

Test(math, simple_number) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 42 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected = parse(arena, parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "42");

    sb_free(&sb);
    arena_release(arena);
}

Test(math, addition_left_associativity) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 10 });
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 3 });
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 2 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected = parse(arena, parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "((10 SUB 3) SUB 2)");

    sb_free(&sb);
    arena_release(arena);
}

Test(math, term_precedence) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 2 });
    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 3 });
    token_list_push(token_list, (Token) { TOKEN_STAR, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 4 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected = parse(arena, parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "(2 ADD (3 MUL 4))");

    sb_free(&sb);
    arena_release(arena);
}

Test(math, unary_minus) {
    Arena *arena = arena_alloc(1024);
    
    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 3 });
    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 2 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });
    
    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected = parse(arena, parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "(( NEG 3) ADD 2)");

    sb_free(&sb);
    arena_release(arena);
}

Test(math, nested_unary_minus) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_MINUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 5 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected = parse(arena, parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "( NEG ( NEG 5))");

    sb_free(&sb);
    arena_release(arena);
}

Test(math, parentheses) {
    Arena *arena = arena_alloc(1024);

    TokenList* token_list = create_token_list(arena, 10);
    token_list_push(token_list, (Token) { TOKEN_LPAREN, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 2 });
    token_list_push(token_list, (Token) { TOKEN_PLUS, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 3 });
    token_list_push(token_list, (Token) { TOKEN_RPAREN, 0 });
    token_list_push(token_list, (Token) { TOKEN_STAR, 0 });
    token_list_push(token_list, (Token) { TOKEN_NUMBER, 4 });
    token_list_push(token_list, (Token) { TOKEN_END, 0 });

    Parser* parser = create_parser(arena, token_list);

    ASTNode* expected = parse(arena, parser);

    StrBuf sb = {0};
    sb_init(&sb, 250);
    ast_to_string(&sb, expected);

    const char *cstr = sb_cstr(&sb);
    cr_expect_str_eq(cstr, "((2 ADD 3) MUL 4)");

    sb_free(&sb);
    arena_release(arena);
}

// Test(math, empty_input) {
//     Arena *arena = arena_alloc(1024);

//      Parser* parser = create_parser(arena, token_list);

//     ASTNode* root = parse(arena, parser);

//     cr_assert_eq(root, NULL);

//     arena_release(arena);
// }