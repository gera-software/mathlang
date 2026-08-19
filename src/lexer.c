#include "lexer.h"
#include <stdlib.h>

bool is_digit(char c) {
    switch(c) {
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
            return true;
        default:
            return false;
    }
}

// int string_to_int(const char* string, char** endptrl) {
//     // int i = 0;
//     // while(string[i] != '\0' && is_digit(string[i])) {
//     //     i++;
//     // }
//     char *endptr;

//     long val = strtol(string, &endptr, 10); // Base 10 conversion
//     printf("%d , endptrl: -%c-\n", (int)val, *endptr);

//     if (string == endptr) {
//         printf("Error: No digits found.\n");
//     }

//     // Check for partial invalid characters (e.g., "123xyz")
//     if (*endptr != '\0') {
//         printf("Warning: Partial conversion. Non-numeric character found: -%c-\n", *endptr);
//     }
//     return (int)val; // Safe to cast if within range
// }

Token lexer_next_token(Lexer* lexer) {
    char current = lexer->source[lexer->cursor];

    if(current == '\0') return (Token){ END, 0, lexer->cursor++ }; 

    // lexer->cursor++;

    switch(current) {
        case '+':
            return (Token) { PLUS, 0, lexer->cursor++ };
        case '-':
            return (Token) { MINUS, 0, lexer->cursor++ };
        case '*':
            return (Token) { STAR, 0, lexer->cursor++ };
        case '/':
            return (Token) { SLASH, 0, lexer->cursor++ };
        case '(':
            return (Token) { LPAREN, 0, lexer->cursor++ };
        case ')':
            return (Token) { RPAREN, 0, lexer->cursor++ };
    }

    if(is_digit(current)) {
        // lexer->cursor--; // Step back to read full number
        size_t start_pos = lexer->cursor;
        char* endptr;

        long val = strtol(&lexer->source[lexer->cursor], &endptr, 10); // Base 10 conversion
        printf("%p: -%c-, ", (void*) &lexer->source[lexer->cursor], lexer->source[lexer->cursor]);
        printf("%p: -%c-, ", (void*) endptr, *endptr);
        if (*endptr != '\0') { printf("Unconverted trailing text: -%s-\n", endptr); }
        lexer->cursor += (endptr - &lexer->source[lexer->cursor]);
        printf("cursor: %lu\n", lexer->cursor);
        return (Token){ NUMBER, (int)val, start_pos};
    }

    return (Token){ INVALID, 0, lexer->cursor };
}