#include <stdio.h>
#include "repl.h"
#include "lexer.h"
#include "parser.h"

void repl_start(void) {
    Arena *a = arena_alloc(1024);

    char input[100];
    printf("MathLang REPL\n");
    printf("Enter a math expression:\n");
    // printf("Type 'quit' to exit.\n");
    fgets(input, sizeof(input), stdin);
    // Actual loop comes later.

    Lexer* lexer = create_lexer(a, input);

    TokenList* token_list = tokenize(a, lexer);

    Parser* parser = create_parser(a, token_list);

    ASTNode* ast = parse(a, parser);

    printf("found %ld tokens\n", token_list->length);

    StrBuf string_buf = {0};
    sb_init(&string_buf, 250);
    ast_to_string(&string_buf, ast);
    printf("expression: %s\n", sb_cstr(&string_buf));

}