#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "token.h"

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

char lexer_peek(Lexer *lexer, size_t offset);
char lexer_consume(Lexer *lexer);

Lexer* create_lexer(Arena *a, char *source);

bool is_digit(char c);

bool is_space(char c);