#include "lexer.h"
#include <stdlib.h>
#include "str_buf.h"

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

bool is_space(char c) {
    switch (c) {
        case ' ':
            return true;
        case '\n':
            return true;
        default:
            return false;
    }
}

char lexer_peek(Lexer *lexer, size_t offset) {
    return lexer->source[lexer->cursor + offset];
}

char lexer_consume(Lexer *lexer) {
    return lexer->source[lexer->cursor++];
}

Token lexer_next_token(Lexer* lexer) {
    // white space skipping
    while(lexer_peek(lexer, 0) != '\0' && is_space(lexer_peek(lexer, 0))) {
        lexer_consume(lexer);
    }

    char peek = lexer_peek(lexer, 0);

    // end of string
    if(peek == '\0') {
        lexer_consume(lexer);
        return (Token){ TOKEN_END, 0 }; 
    }

    // digits
    if(is_digit(peek)) {
        StrBuf string_buf = {0};
        sb_init(&string_buf, 10);

        while(lexer_peek(lexer, 0) != '\0' && is_digit(lexer_peek(lexer, 0))) {
            sb_append_char(&string_buf, lexer_consume(lexer));
        }
        int val = (int) strtol(sb_cstr(&string_buf), NULL, 10);
    
        sb_free(&string_buf);
        return (Token){ TOKEN_NUMBER, val };
    }

    // operators
    switch(peek) {
        case '+':
            lexer_consume(lexer);
            return (Token) { TOKEN_PLUS, 0 };
        case '-':
            lexer_consume(lexer);
            return (Token) { TOKEN_MINUS, 0 };
        case '*':
            lexer_consume(lexer);
            return (Token) { TOKEN_STAR, 0 };
        case '/':
            lexer_consume(lexer);
            return (Token) { TOKEN_SLASH, 0 };
        case '(':
            lexer_consume(lexer);
            return (Token) { TOKEN_LPAREN, 0 };
        case ')':
            lexer_consume(lexer);
            return (Token) { TOKEN_RPAREN, 0 };
        default:
            lexer_consume(lexer);
            return (Token){ TOKEN_INVALID, 0 };
    }

}

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