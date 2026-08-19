#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LPAREN,
    RPAREN,
    END,
    INVALID,
} TokenType;

typedef struct Token {
    TokenType type;
    int value;
    int column; // start from 0 index
} Token;

typedef struct Lexer {
    const char* source;
    size_t cursor;
} Lexer;

Token lexer_next_token(Lexer* lexer);

bool is_digit(char c);

bool is_space(char c);