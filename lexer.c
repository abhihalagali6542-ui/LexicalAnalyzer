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

static Token token;

static FILE*fptr_src;

//-------------------------------------------------------------------------------//

void initializeLexer(const char* filename)
{
    fptr_src=fopen(filename,"r");

    if(fptr_src == NULL)
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
    int i=0;
    char ch;

    token.lexeme[0]='\0';
    token.type=UNKNOWN;

    // skip if whitespace is there
    do{

        ch=fgetc(fptr_src);

    }while(ch !=EOF && isspace(ch));

    // check for End of file
    if(ch == EOF)
    {
        return token;
    }

    // skip comments 
    if(ch == '/')
    {
        ch = fgetc(fptr_src);
        if(ch == '/') // ------
        {
            while((ch = fgetc(fptr_src))!='\n' && ch !=EOF)
            {
            }
            if(ch == '\n')
            {
                return getNextToken();
            }
        }
        if(ch == '*') /*-----*/
        {
            char prev = 0;

            while ((ch = fgetc(fptr_src)) != EOF)
            {
                if (prev == '*' && ch == '/')
                {
                    break;
                }
                prev = ch;
            }
                return getNextToken();
        }
    }
    // make token of alpha
    else if(isalpha(ch) || ch == '_')
    {
        do
        {
            token.lexeme[i++] = ch;
            ch = fgetc(fptr_src);

        } while (isalnum(ch) || ch == '_');

        token.lexeme[i]='\0';

        if(ch != EOF)
        {
            ungetc(ch,fptr_src);
        }

        // categorize Token
        categorizeToken(&token);
        return token;
    }
    // make token of digit
    else if(isdigit(ch))
    {
        do
        {

            token.lexeme[i++] = ch;
            ch = fgetc(fptr_src);

        } while (isdigit(ch));

        token.lexeme[i] = '\0';

        if(ch != EOF)
        {
            ungetc(ch,fptr_src);
        }

        // categorize Token
        categorizeToken(&token);
        return token;
    }
    // make token of string literal
    else if(ch == '"')
    {
        ch = fgetc(fptr_src);
        do
        {  
            
            token.lexeme[i++] = ch;
            ch = fgetc(fptr_src);

        } while (ch != '"' || ch == '\\');

        token.lexeme[i]='\0';

        token.type = STRING_LITERAL;
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
    if(strchr(specialCharacters,ch)!=NULL)
    {
        return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------//

int isConstant(const char* str)
{
    int i=0;

    // check empty string
    if(str[i]==0)
    {
        return 0;
    }
    while(str[i]!=0)
    {
        if(!isdigit(str[i]))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

//-------------------------------------------------------------------------------//

int isIdentifier(const char* str)
{
    int i=0;

    // check first char
    if(!(isalpha(str[0]) || str[0] =='_'))
    {
        return 0;
    }

    // remaining char
    while (str[i] !=0)
    {
        if(!(isalpha(str[0]) || str[0] =='_'))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

//-------------------------------------------------------------------------------//

char* Token_Type(int type)
{
    switch(type)
    {
        case KEYWORD:
            return "KEYWORD";
            break;

        case OPERATOR:
            return "OPERATOR";
            break;

        case SPECIAL_CHARACTER:
            return "SPECIAL CHARACTER";
            break;

        case CONSTANT:
            return "CONSTANT";
            break;

        case IDENTIFIER:
            return "IDENTIFIER";
            break;

        case STRING_LITERAL:
            return "STRING LITERAL";
            break;

        case UNKNOWN:
            return "UNKNOWN";
            break;
    }
}

//-------------------------------------------------------------------------------//
