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
    printf("Expected NUMBER\n");
    return NULL;
}

ASTNode* parse_unary_minus(Arena* arena, Parser* parser) {
    Token* peek = parser_peek(parser, 0);
    if(peek->type == TOKEN_MINUS) {
        parser_consume(parser);
        ASTNode* factor_node = parse_factor(arena, parser);
        if(factor_node == NULL) {
            printf("Expected FACTOR\n");
            return NULL;
        }

        return create_unary_op_node(arena, NODE_NEG, factor_node);
    }
    printf("Expected '-'\n");
    return NULL;
}

ASTNode* parse_factor(Arena* arena, Parser* parser) {
    ASTNode* number = parse_number(arena, parser);
    if(number != NULL) {
        return number;
    }

    // TODO parse expression

    ASTNode* unary_minus = parse_unary_minus(arena, parser);
    return unary_minus;
}

ASTNode* parse_term(Arena* arena, Parser* parser) {
    ASTNode* factor_node1 = parse_factor(arena, parser);
    if(factor_node1 == NULL) {
        printf("Expected FACTOR\n");
        return NULL;
    }
    
    Token* peek = parser_peek(parser, 0);
    if(peek->type == TOKEN_STAR || peek->type == TOKEN_SLASH) {
        Token *token = parser_consume(parser);

        ASTNode* factor_node2 = parse_factor(arena, parser);
        if(factor_node2 == NULL) {
            printf("Expected FACTOR\n");
            return NULL;
        }

        return create_binary_op_node(arena, token->type == TOKEN_STAR ? NODE_MUL : NODE_DIV, factor_node1, factor_node2);
    } else {
        return factor_node1;
    }
}

// TODO
ASTNode* parse(Parser* parser) {
    return NULL;
}