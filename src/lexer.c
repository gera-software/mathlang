#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void newMathSymbol(TokenList *list, int line, int column, TokenType type) {
    Token t = {
        .type = type,
        .value = 0,
        .line = line,
        .column = column,
    };

    pushToken(list, t);
}

void newDigit(TokenList *list, int line, int column, char char_digit) {
    int int_digit = char_digit - '0'; // convert a single character digit to its actual mathematical integer value
    Token token = {
        .type = DIGIT,
        .value = int_digit,
        .line = line,
        .column = column,
    }; 

    // if previous token is a number, concat them in a single token
    if(list->count == 0) {
        pushToken(list, token);
    } else {
        Token *previous = &list->tokens[list->count - 1];
        
        if(previous->type == DIGIT) {
            char previous_digits[40]; // Certifique-se de que o tamanho é suficiente para o número e o '\0'
            // Converte o inteiro para string com segurança
            snprintf(previous_digits, sizeof(previous_digits), "%d", previous->value);
            
            // Une a string e o caractere com segurança no novo array
            char resultado[50];
            snprintf(resultado, sizeof(resultado), "%s%c", previous_digits, char_digit);
           
            // convert string para numero novamente
            int new_value = atoi(resultado);
            printf("new int %d\n", new_value);
            previous->value = new_value;
        } else {
            pushToken(list, token);
        }
    } 
}

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

    list->tokens[list->count] = token;
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
                newMathSymbol(&list, line, column, PLUS);
                break;
            case '-':
                newMathSymbol(&list, line, column, MINUS);
                break;
            case '*':
                newMathSymbol(&list, line, column, STAR);
                break;
            case '/':
                newMathSymbol(&list, line, column, SLASH);
                break;
            case '(':
                newMathSymbol(&list, line, column, LPAREN);
                break;
            case ')':
                newMathSymbol(&list, line, column, RPAREN);
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
                newDigit(&list, line, column, input[index]);
                break;
            default:
                newMathSymbol(&list, line, column, INVALID);
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