#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool pushToken(TokenList *list, Token token) {
    if (list == NULL) {
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

    // if previous token is a number, concat them in a single token
    if(token.type == DIGIT) {
        if(list->count == 0) {
            list->tokens[list->count] = token;
            list->count++;
        } else {
            Token *previous = &list->tokens[list->count - 1];
            
            char previous_digit[40]; // Certifique-se de que o tamanho é suficiente para o número e o '\0'
            // Converte o inteiro para string com segurança
            snprintf(previous_digit, sizeof(previous_digit), "%d", previous->value);
            
            char current_digit[20]; // Certifique-se de que o tamanho é suficiente para o número e o '\0'
            // Converte o inteiro para string com segurança
            snprintf(current_digit, sizeof(current_digit), "%d", token.value);
            
            printf("previous digit %s\n", previous_digit);
            printf("current digit %s\n", current_digit);
            
            // Concatena strings
            strcat(previous_digit, current_digit);
            printf("new string %s\n", previous_digit);
            
            // convert string para numero novamente
            int new_value = atoi(previous_digit);
            printf("new int %d\n", new_value);
            previous->value = new_value;
        }
    } else {
        list->tokens[list->count] = token;
        list->count++;
    }
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
    TokenList list;
    list.tokens = NULL;
    list.count = 0;
    list.capacity = 0;
    list.error = 0;
    strcpy(list.error_message, "");

    int start_capacity = 16;
    Token *tokens;

    tokens = (Token *) malloc(sizeof(Token) * start_capacity);
    if(tokens == NULL) {
        list.error = 1;
        strcpy(list.error_message, "Memória insuficiente!");
        return list;
    }
    list.tokens = tokens;
    list.capacity = start_capacity;

    int index = 0;
    int line = 1;
    int column = 1;
    Token t = {
        .type = INVALID,
        .value = 0,
        .line = line,
        .column = column,
    };
    while(input[index] != '\0') {
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
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            case '-':
                t.type = MINUS;
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            case '*':
                t.type = STAR;
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            case '/':
                t.type = SLASH;
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            case '(':
                t.type = LPAREN;
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            case ')':
                t.type = RPAREN;
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                t.type = DIGIT;
                t.value = input[index] - '0'; // convert a single character digit to its actual mathematical integer value
                t.line = line;
                t.column = column;
                pushToken(&list, t);
                break;
            default:
                t.type = INVALID;
                t.value = 0;
                t.line = line;
                t.column = column;
                pushToken(&list, t);
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
    pushToken(&list, end_token);

    return list;
}