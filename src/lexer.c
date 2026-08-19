#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>

bool pushToken(TokenList *list, Token *token) {
    if (list == NULL || token == NULL) {
        return false;
    }

    // if (list->count >= list->capacity) {
    //     size_t new_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
    //     Token *resized_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
    //     if (resized_tokens == NULL) {
    //         return false;
    //     }
    //     list->tokens = resized_tokens;
    //     list->capacity = new_capacity;
    // }

    list->tokens[list->count] = *token;
    list->count++;
    return true;
}

void free_tokens(TokenList *list) {
    if (list == NULL) {
        return;
    }

    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
    list->error = 0;
    list->error_message[0] = '\0';
}

TokenList lex(const char *input) {
    int capacity = 16;
    Token *tokens;

    tokens = (Token *) malloc(sizeof(Token) * capacity);
    if(tokens == NULL) {
        return (TokenList) {
            .tokens = NULL,
            .count = 0,
            .capacity = 0,
            .error = 1,
            .error_message = "Memória insuficiente!"
        };
    }

    TokenList list = {
        .tokens = tokens,
        .count = 0,
        .capacity = capacity,
        .error = 0,
        .error_message = ""
    };

    int index = 0;
    int line = 1;
    int column = 1;
    while(input[index] != '\0') {
        if(input[index] == ' ' || input[index] == '\t') {
            // ignore whitespace
        } else if(input[index] == '\n' || input[index] == '\r') {
            line++;
            column = 0;
        } else if(input[index] == '+') {
            Token t = {
                .type = PLUS,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else if(input[index] == '-') {
            Token t = {
                .type = MINUS,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else if(input[index] == '*') {
            Token t = {
                .type = STAR,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else if(input[index] == '/') {
            Token t = {
                .type = SLASH,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else if(input[index] == '(') {
            Token t = {
                .type = LPAREN,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else if(input[index] == ')') {
            Token t = {
                .type = RPAREN,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else if(input[index] == '0') {
            Token t = {
                .type = DIGIT,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        } else {
            Token t = {
                .type = INVALID,
                .value = 0,
                .line = line,
                .column = column,
            };
            pushToken(&list, &t);
        }

        column++;
        index++;
    }

    return list;
}