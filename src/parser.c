#include "parser.h"

Token* parser_peek(Parser* parser, size_t offset) {
    return &(parser->tokens[parser->cursor + offset]);
}

Token* parser_consume(Parser* parser) {
    return &(parser->tokens[parser->cursor++]);
}

ASTNode* parse_number(Arena* arena, Parser* parser) {
    Token* peek = parser_peek(parser, 0);
    if(peek->type == TOKEN_NUMBER) {
        Token* token = parser_consume(parser);

        return create_int_node(arena, token->value);
    }
    return NULL;
}

// TODO
ASTNode* parse(Parser* parser) {
    return NULL;
}