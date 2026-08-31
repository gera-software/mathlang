#include <stdio.h>
#include "repl.h"
#include "lexer.h"

void repl_start(void) {
    char input[100];
    printf("MathLang REPL\n");
    // printf("Type 'quit' to exit.\n");
    fgets(input, sizeof(input), stdin);
    // Actual loop comes later.

    Lexer lexer = {
        .source = input,
        .cursor = 0,
    };

    while(true) {
        Token token = lexer_next_token(&lexer);
        printf("%s: <%d>\n", get_token_type_name(token.type), token.value);

        // TODO push to array
        if(token.type == TOKEN_END) {
            break;
        }
    }
}