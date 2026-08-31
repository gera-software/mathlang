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
    ASTNode* factor_node_left = parse_factor(arena, parser);
    if(factor_node_left == NULL) {
        printf("Expected FACTOR\n");
        return NULL;
    }

    Token* peek = NULL;
    while(true) {
        // TODO safe guard for out of bound access
        peek = parser_peek(parser, 0);
        if(peek->type == TOKEN_END) {
            break;
        }

        if(peek->type == TOKEN_STAR || peek->type == TOKEN_SLASH) {
            Token *token = parser_consume(parser);

            ASTNode* factor_node_right = parse_factor(arena, parser);
            if(factor_node_right == NULL) {
                printf("Expected FACTOR\n");
                return NULL;
            }

            factor_node_left = create_binary_op_node(arena, token->type == TOKEN_STAR ? NODE_MUL : NODE_DIV, factor_node_left, factor_node_right);
        } else {
            break;
        }

    }
    return factor_node_left;
}

ASTNode* parse_expression(Arena* arena, Parser* parser) {
    ASTNode* term_node_left = parse_term(arena, parser);
    if(term_node_left == NULL) {
        printf("Expected TERM\n");
        return NULL;
    }

    Token* peek = NULL;
    while(true) {
        // TODO safe guard for out of bound access
        peek = parser_peek(parser, 0);
        if(peek->type == TOKEN_END) {
            break;
        }

        if(peek->type == TOKEN_PLUS || peek->type == TOKEN_MINUS) {
            Token *token = parser_consume(parser);

            ASTNode* term_node_right = parse_term(arena, parser);
            if(term_node_right == NULL) {
                printf("Expected TERM\n");
                return NULL;
            }

            term_node_left = create_binary_op_node(arena, token->type == TOKEN_PLUS ? NODE_ADD : NODE_SUB, term_node_left, term_node_right);
        } else {
            break;
        }
    }
    return term_node_left;
}

// TODO
ASTNode* parse(Parser* parser) {
    return NULL;
}