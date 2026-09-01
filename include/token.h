#pragma once

typedef enum TokenType {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_INVALID,
} TokenType;

const char* get_token_type_name(TokenType type);

typedef struct Token {
    TokenType type;
    int value;
} Token;
