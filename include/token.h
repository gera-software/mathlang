#pragma once
#include "arena.h"

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


typedef struct TokenList {
    Token* list; /* contiguous backing store */
    size_t capacity; /* total Tokens in the buffer */
    size_t length; /* current used Tokens / bump offset */
    Arena* arena; /* arena onde a lista está alocada */
} TokenList;

TokenList* create_token_list(Arena* a, size_t capacity);

void token_list_push(TokenList* token_list, Token token);