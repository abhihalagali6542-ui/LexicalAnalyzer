#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";

void initializeLexer(const char* filename)
{
    token.fptr_src=fopen(filename,"r");

    if(token.fptr_src == NULL)
    {
        printf("\nError : unable to open source file..\n");
        return;
    }

    printf("\nFile opende successfully...\n");
    return 1;
}

//-------------------------------------------------------------------------------//

Token getNextToken()
{
    int i=0;
    char ch;
    token.lexeme[0]='\0';
    token.type=UNKNOWN;

    // skip if space is there
    do{

        ch=fgetc(token.fptr_src);

    }while(isspace(ch));

    // check for End of file
    if(ch == EOF)
    {
        return Token;
    }

    // get token 
    if(isapla(ch) || ch == '_')
    {
        token.lexeme[i++]=ch;
        do{

            ch=fgetc(token.fptr_src);
            token.lexeme[i++]=ch;

        }while(isapla(ch) || ch =='_');

        token.lexeme[i]='\0';

        if(ch != EOF)
        {
            ungetc(ch,token.fptr_src);
        }

        // categorize Token
        categorizeToken(&token)
        return Token;
    }

    token.lexeme[0]=ch;
    token.lexeme[1]='\0';

    categorizeToken(&token)
    return Token;

}

//-------------------------------------------------------------------------------//

void categorizeToken(Token* token)
{
     if (isKeyword(token->lexeme))
    {
        token->type = KEYWORD;
    }
    else if (isConstant(token->lexeme))
    {
        token->type = CONSTANT;
    }
    else if (isIdentifier(token->lexeme))
    {
        token->type = IDENTIFIER;
    }
    else if (isOperator(token->lexeme))
    {
        token->type = OPERATOR;
    }
    else if (isSpecialCharacter(token->lexeme[0]))
    {
        token->type = SPECIAL_CHARACTER;
    }
    else
    {
        token->type = UNKNOWN;
    }
}