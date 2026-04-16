#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }

    initializeLexer(argv[1]);

    Token token;

    while (1)
    {
        token = getNextToken();

        if (token.type == UNKNOWN && token.lexeme[0] == '\0')
            break;

        switch (token.type)
        {
        case KEYWORD:
            printf("Token: %-10s Type: KEYWORD\n", token.lexeme);
            break;

        case OPERATOR:
            printf("Token: %-10s Type: OPERATOR\n", token.lexeme);
            break;

        case SPECIAL_CHARACTER:
            printf("Token: %-10s Type: SPECIAL_CHARACTER\n", token.lexeme);
            break;

        case CONSTANT:
            printf("Token: %-10s Type: CONSTANT\n", token.lexeme);
            break;

        case IDENTIFIER:
            printf("Token: %-10s Type: IDENTIFIER\n", token.lexeme);
            break;

        default:
            printf("Token: %-10s Type: UNKNOWN\n", token.lexeme);
        }
    }

    return 0;
}
