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

/**
 * The Lexer tokenizes a source string
 * 
 * Iterator / Stream Pattern (Pull-Based): The parser acts as the driver, explicitly asking the lexer for the "next token" only when needed via lexer_next_token().
 * This minimizes memory overhead because tokens are processed lazily.
 * 
 * Lexers map transitions character-by-character using deterministic finite automata (state machines).
 * The lexer buffers characters to look one or more steps ahead before deciding on a token type.
 * 
 */
Token lexer_next_token(Lexer* lexer);

bool is_digit(char c);

bool is_space(char c);