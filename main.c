#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[]) 
{
    Token token;
    initializeLexer(argv[1],&token);
    while ((token = getNextToken(&token)).type != UNKNOWN) {
        printf("Token: %s, Type: %d\n", token.lexeme, token.type);
    }

    return 0;
}
