#pragma once
#include "ast.h"
#include "lexer.h"

typedef struct Parser {
    Token *tokens;
    size_t cursor;
} Parser;


Token* parser_peek(Parser* parser, size_t offset);
Token* parser_consume(Parser* parser);

ASTNode* parse(Parser* parser);
