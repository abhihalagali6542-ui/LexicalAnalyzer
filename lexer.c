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

void initializeLexer(const char* filename,Token* token)
{
    token->fptr_src=fopen(filename,"r");

    if(token->fptr_src == NULL)
    {
        printf("\nError : unable to open source file..\n");
        return;
    }

    printf("\nFile opende successfully...\n");
    return;
}

//-------------------------------------------------------------------------------//

Token getNextToken()
{
     Token token;

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
        return token;
    }

    // get token 
    if(isalpha(ch) || ch == '_')
    {
        token.lexeme[i++]=ch;
        do{

            ch=fgetc(token.fptr_src);
            token.lexeme[i++]=ch;

        }while(isalpha(ch) || ch =='_');

        token.lexeme[i]='\0';

        if(ch != EOF)
        {
            ungetc(ch,token.fptr_src);
        }

        // categorize Token
        categorizeToken(&token);
        return token;
    }

    // for single char(: ; ...)
    token.lexeme[0]=ch;
    token.lexeme[1]='\0';

    // categorize Token
    categorizeToken(&token);
    return token;

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

//-------------------------------------------------------------------------------//

int isKeyword(const char* str)
{
    for(int i=0;i<MAX_KEYWORDS;i++)
    {
        if(!strcmp(str,keywords[i]))
        {
            return 1;
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------//

int isOperator(const char* str)
{
    if(strlen(str)!=1)
    {
        return 0;
    }

    if(strchr(operators,str[0])!=NULL)
    {
        return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------//

int isSpecialCharacter(char ch)
{

}

//-------------------------------------------------------------------------------//

int isConstant(const char* str)
{

}

//-------------------------------------------------------------------------------//

int isIdentifier(const char* str)
{

}

//-------------------------------------------------------------------------------//
