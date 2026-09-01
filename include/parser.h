#pragma once
#include "ast.h"
#include "lexer.h"

typedef struct Parser {
    TokenList *tokens;
    size_t cursor;
} Parser;


Token* parser_peek(Parser* parser, size_t offset);
Token* parser_consume(Parser* parser);

ASTNode* parse_expression(Arena* arena, Parser* parser);
ASTNode* parse_term(Arena* arena, Parser* parser);
ASTNode* parse_factor(Arena* arena, Parser* parser);
ASTNode* parse_number(Arena* arena, Parser* parser);
ASTNode* parse_unary_minus(Arena* arena, Parser* parser);

ASTNode* parse(Arena* arena, Parser* parser);
