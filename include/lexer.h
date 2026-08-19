#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef enum TokenType {
    DIGIT,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LPAREN,
    RPAREN,
    END,
    INVALID,
} TokenType;

/**
 * Representa um token lexical
 * 
 * `value` holds the integer value for NUMBER tokens. For other tokens, it can be 0. 
 * `line` and `column` store the position of the token start for error messages.
 */
typedef struct Token {
    TokenType type;
    int value;
    int line;
    int column;
} Token;

/**
 * A dynamic array to hold all tokens
 * 
 * The TokenList struct represents a dynamic array designed to store tokens, 
 * featuring a pointer to the token array, a count of the current number of tokens, 
 * and the total capacity of the array. This structure facilitates efficient management 
 * of token storage in applications such as parsers or interpreters.
 */
typedef struct TokenList {
    Token *tokens;
    size_t count;
    size_t capacity;
    int error; 
    char error_message[256];
} TokenList;

bool pushToken(TokenList *list, Token token);

/**
 * Tokenizes the input string. Returns a TokenList.
 * The caller is responsible for freeing the token array (free_tokens).
 */
TokenList lex(const char *input);

/**
 * Frees the tokens inside the list.
 */
void free_tokens(TokenList *list);