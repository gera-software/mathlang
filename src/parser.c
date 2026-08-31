#include "parser.h"

Token* parser_peek(Parser* parser, size_t offset) {
    return &(parser->tokens[parser->cursor + offset]);
}

Token* parser_consume(Parser* parser) {
    return &(parser->tokens[parser->cursor++]);
}

ASTNode* parse(Parser* parser) {
    return NULL;
}