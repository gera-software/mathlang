#include "token.h"

const char* get_token_type_name(TokenType type) {
    switch(type) {
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_STAR: return "TOKEN_STAR";
        case TOKEN_SLASH: return "TOKEN_SLASH";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_END: return "TOKEN_END";
        case TOKEN_INVALID: return "TOKEN_INVALID";
        default: return "TOKEN_UNKNOWN";
    }
}

TokenList* create_token_list(Arena* a, size_t capacity) {
    TokenList* token_list = arena_push(a, sizeof(TokenList));
    if(token_list == NULL) {
        return NULL;
    }
    token_list->list = arena_push_array(a, Token, capacity);
    if(token_list->list == NULL) {
        return NULL;
    }
    token_list->capacity = capacity;
    token_list->length = 0;

    return token_list;
}