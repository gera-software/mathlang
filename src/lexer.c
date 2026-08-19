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
        Token t = {
            .type = INVALID,
            .value = 0,
            .line = line,
            .column = column,
        };
        switch (input[index]) {
            // increment new line
            case '\n':
            case '\r':
                line++;
                column = 0;
                break;
            // ignore whitespace
            case ' ':
            case '\t':
                break;
            case '+':
                t.type = PLUS;
                pushToken(&list, &t);
                break;
            case '-':
                t.type = MINUS;
                pushToken(&list, &t);
                break;
            case '*':
                t.type = STAR;
                pushToken(&list, &t);
                break;
            case '/':
                t.type = SLASH;
                pushToken(&list, &t);
                break;
            case '(':
                t.type = LPAREN;
                pushToken(&list, &t);
                break;
            case ')':
                t.type = RPAREN;
                pushToken(&list, &t);
                break;
            case '0':
                t.type = DIGIT;
                t.value = 0;
                pushToken(&list, &t);
                break;
            default:
                t.type = INVALID;
                pushToken(&list, &t);
                break;
        }

        column++;
        index++;
    }
    
    Token end_token = {
        .type = END,
        .value = 0,
        .line = line,
        .column = column,
    };
    pushToken(&list, &end_token);

    return list;
}