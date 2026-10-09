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

    if(ch == EOF)
    {
        return token;
    }

    if(ch == '#')
    {
        while((ch = fgetc(fptr_src))!='\n' && ch !=EOF)
        {
        }
        if(ch == '\n')
        {
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
    // skip comments 
    else if(ch == '/')
    {
        char single =ch;
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
            int close = 0;
            char prev = 0;

            while ((ch = fgetc(fptr_src)) != EOF)
            {
                if (prev == '*' && ch == '/')
                {
                      close=1;
                    break;
                }
                prev = ch;
            }
            if(!close)
            {
                token.type = UNKNOWN;
                printf("Lexical Error: Unterminated comment\n");
                return token;
            }
            return getNextToken();
        }

            token.lexeme[0]='/';
            token.lexeme[1] = '\0';
            token.type = OPERATOR;
            ungetc(ch,fptr_src);
            return token;
    }
    // char littral'--'
    else if(ch == '\'')
    {
        i = 0;

        token.lexeme[i++] = ch;

        ch = fgetc(fptr_src);

        if(ch == '\'' || ch == '\n' || ch == EOF)
        {
            token.lexeme[i++] = ch;
            token.lexeme[i] = '\0';

            token.type = UNKNOWN;

            printf("Lexical Error: Invalid character constant '%s'\n",token.lexeme);

            return token;
        }

        token.lexeme[i++] = ch;

        if(ch == '\\')
        {
            ch = fgetc(fptr_src);

            if(ch == EOF || ch == '\n')
            {
                token.type = UNKNOWN;

                printf("Lexical Error: Unterminated character constant\n");

                return token;
            }

            token.lexeme[i++] = ch;
        }

        ch = fgetc(fptr_src);

        if(ch != '\'')
        {
            while(ch != '\'' && ch != '\n' && ch != EOF)
            {
                token.lexeme[i++] = ch;
                ch = fgetc(fptr_src);
            }

            token.lexeme[i] = '\0';

            token.type = UNKNOWN;

            printf("Lexical Error: Invalid character constant '%s'\n",
               token.lexeme);

            return token;
        }

        token.lexeme[i++] = ch;
        token.lexeme[i] = '\0';

        token.type = CHAR_LITERAL;

        return token;
    }
    // make token of digit
    else if(isdigit(ch))
    {
        do
        {

            token.lexeme[i++] = ch;
            ch = fgetc(fptr_src);

        } while (isdigit(ch) || ch == '.' );

        if(isalpha(ch) || ch == '_')
        {
            while(isalnum(ch) || ch == '_')
            {
                token.lexeme[i++] = ch;
                ch = fgetc(fptr_src);
            }

            token.lexeme[i] = '\0';
            if(isConstant(token.lexeme) || isfloatConstant(token.lexeme))
            {
                token.type = CONSTANT;
                return token;
            }
            token.type =UNKNOWN;
            printf("Lexical Error : Invalid identifier/constant '%s'\n",token.lexeme);
            return token;
        }
        token.lexeme[i] = '\0';

        if(ch != EOF)
        {
            ungetc(ch,fptr_src);
        }

        // categorize Token
        if(isConstant(token.lexeme) || isfloatConstant(token.lexeme))
        {
            token.type = CONSTANT;
        }
        else
        {
            printf("Lexical Error: Invalid constant '%s'\n",token.lexeme);
            token.type = UNKNOWN;
        }
        return token;
    }
    // make token of string literal
    else if(ch == '"')
    {
        ch = fgetc(fptr_src);
        do
        {  
            if(ch == EOF || ch == '\n')
            {
                token.lexeme[i] = '\0';

                token.type = UNKNOWN;

                printf("Lexical Error: Unterminated string literal '%s'\n",token.lexeme);

                return token;
            }
            
            token.lexeme[i++] = ch;
            ch = fgetc(fptr_src);

        } while (ch != '"' || ch == '\\');

        token.lexeme[i]='\0';

        token.type = STRING_LITERAL;
        return token;
    }
    else if(strchr(operators,ch)!=NULL)
    {
        token.lexeme[0]=ch;
        char next = fgetc(fptr_src);
        if(&next != NULL && strchr(operators,next)!=NULL)
        {
            token.lexeme[1]=next;
        }
        if(&next != NULL && strchr(operators,next)==NULL)
        {
            token.lexeme[1]='\0';
            ungetc(next,fptr_src);
            token.type = OPERATOR;
            return token; 
        }
        next = fgetc(fptr_src);
        if(&next != NULL && strchr(operators,next)!=NULL)
        {
            token.lexeme[2]=next;
            token.lexeme[3]='\0';
            printf("Lexical Error : Invalid identifier/operater '%s'\n",token.lexeme);
            token.type=UNKNOWN;
            return  token;
        }
        ungetc(next,fptr_src);
        token.lexeme[2]='\0';
        token.type = OPERATOR;
        return token;  
    }

        token.lexeme[0]=ch;
        token.lexeme[1]='\0';

        // categorize Token
        categorizeToken(&token);
        parentheses_valid(ch);

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

    // binary
    if(str[0]=='0' && (str[1] == 'b' || str[1] == 'B'))
    {
        if(str[2] == 0)
        {
            return 0;
        }

        i=2;
        while(str[i]!=0)
        {
            if(str[i]!='0' && str[i]!='1')
            {
                return 0;
            }
            i++;
        }
        return 1;
    }

    // hexa
    if(str[0]=='0' && (str[1] == 'x' || str[1] == 'X'))
    {
        if(str[2] == 0)
        {
            return 0;
        }

        i=2;
        while(str[i]!=0)
        {
            if(!isxdigit(str[i]))
            {
                return 0;
            }
            i++;
        }
        return 1;
    }

    // octa
    if(str[0]=='0' && str[1] !=0)
    {
        i=1;
        while(str[i]!=0)
        {
            if(str[i]<'0' || str[i]>'7')
            {
                return 0;
            }
            i++;
        }
        return 1;
    }

    // Decimal
    i=0;
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

int isfloatConstant(const char* str)
{
    int i=0, dot_count=0, digit_count=0;
    while(str[i]!=0)
    {
        if(isdigit(str[i]))
        {
            digit_count++;
        }
        else if(str[i]=='.')
        {
            dot_count++;
            if(dot_count > 1)
                return 0;
        }
        else if(str[i]=='f' || str[i]=='l')
        {
            if(str[i+1]!=0)
            {
                return 0;
            }
            break;
        }
        else
        {
            return 0;
        }
        i++;
    }
    if(dot_count!=1)
    {
        return 0;
    }
    if(digit_count == 0)
    {
        return 0;
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
        if(!(isalnum(str[i]) || str[i] =='_'))
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
        
        case CHAR_LITERAL:
            return "CHAR LITERAL";
            break; 

        case UNKNOWN:
            return "UNKNOWN";
            break;
    }
}

//-------------------------------------------------------------------------------//
char openbracket[1000];
int top=-1;

int isopenbracket(const char ch)
{
    if(ch == '{' || ch == '[' || ch == '(')
    {
        return 1;
    }
    return 0;
}

int isclosebracket(const char ch)
{
    if(ch == '}' || ch == ']' || ch == ')')
    {
        return 1;
    }
    return 0;
}

int ismatching(const char ch)
{
    if(ch == '}' && openbracket[top]!='{')
    {
        return 1;
    }
    if(ch == ']' && openbracket[top]!='[')
    {
        return 1;
    }
    if(ch == ')' && openbracket[top]!='(')
    {
        return 1;
    }

    return 0;
}

Token parentheses_valid(const char ch)
{
    if(isopenbracket(ch))
    {
        openbracket[++top]=ch;
    }
    
    if(isclosebracket(ch))
    {
        if(top == -1)
        {
            printf("Lexical Error: Unexpected closing bracket '%c'\n", ch);
            token.type=UNKNOWN;
            return token;
        }
        else if(ismatching(ch))
        {
            printf("Lexical Error: Mismatched bracket '%c', expected matching close for '%c'\n",ch, openbracket[top]);
            token.type=UNKNOWN;
            return token;
        }
        
        top--;

        token.type = SPECIAL_CHARACTER;
        return token;
    }
    
    token.type=SPECIAL_CHARACTER;
    return token;
}
