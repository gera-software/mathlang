#include <stdio.h>
#include "repl.h"
#include "lexer.h"

void repl_start(void) {
    Arena *a = arena_alloc(1024);

    char input[100];
    printf("MathLang REPL\n");
    // printf("Type 'quit' to exit.\n");
    fgets(input, sizeof(input), stdin);
    // Actual loop comes later.

    Lexer lexer = {
        .source = input,
        .cursor = 0,
    };

    TokenList* token_list = create_token_list(a, 10);

    // Lexer step
    while(true) {
        Token token = lexer_next_token(&lexer);
        printf("%s: <%d>\n", get_token_type_name(token.type), token.value);

        token_list_push(token_list, token);

        if(token.type == TOKEN_END) {
            break;
        }
    }

    printf("found %ld tokens\n", token_list->length);
}