#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char *keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"};

static const char *operators = "+-*/%=!<>|&";
static const char *specialCharacters = ",;:{}()[]";

/* ----------------- IDENTIFIER CHECK ----------------- */
int isIdentifier(const char *str)
{
    if (!str || !*str)
        return 0;

    if (!isalpha((unsigned char)str[0]) && str[0] != '_')
        return 0;

    for (int i = 1; str[i]; i++)
    {
        if (!isalnum((unsigned char)str[i]) && str[i] != '_')
            return 0;
    }
    return 1;
}

/* ------------------ CONSTANT CHECK ------------------ */
int isConstant(const char *str)
{
    int i = 0, hasDigit = 0, hasDot = 0;

    if (!str || !*str)
        return 0;

    if (str[i] == '+' || str[i] == '-')
        i++;

    for (; str[i]; i++)
    {
        if (isdigit(str[i]))
            hasDigit = 1;

        else if (str[i] == '.' && !hasDot)
            hasDot = 1;

        else
            break;
    }

    if (str[i] == 'f' || str[i] == 'F' || str[i] == 'l' || str[i] == 'L')
        i++;

    return str[i] == '\0';
}

/* -------- SPECIAL CHARACTER CHECK (CORRECTED) -------- */
int isSpecialCharacter(char ch)
{
    return strchr(specialCharacters, ch) != NULL;
}

/* ----------------- OPERATOR CHECK -------------------- */
int isOperator(const char *str)
{
    if (!str || !*str)
        return 0;

    if (strlen(str) == 1)
        return strchr(operators, str[0]) != NULL;

    static const char *multi_ops[] = {
        "==", "!=", "<=", ">=", "&&", "||",
        "+=", "-=", "*=", "/=", "%=", ">>", "<<",
        NULL};

    for (int i = 0; multi_ops[i]; i++)
        if (strcmp(str, multi_ops[i]) == 0)
            return 1;

    return 0;
}

/* ---------------- KEYWORD CHECK ---------------- */
int isKeyword(const char *str)
{
    for (int i = 0; keywords[i]; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;

    return 0;
}

/* ---------------- TOKEN CATEGORIZE FUNCTION ---------------- */
void categorizeToken(Token *token)
{
    if (isKeyword(token->lexeme))
        token->type = KEYWORD;
    else if (isIdentifier(token->lexeme))
        token->type = IDENTIFIER;
    else if (isConstant(token->lexeme))
        token->type = CONSTANT;
    else if (isOperator(token->lexeme))
        token->type = OPERATOR;
    else if (isSpecialCharacter(token->lexeme[0]))
        token->type = SPECIAL_CHARACTER;
    else
        token->type = UNKNOWN;
}

/* ---------------- LEXER CORE ---------------- */
FILE *sourceFile = NULL;

void initializeLexer(const char *filename)
{
    sourceFile = fopen(filename, "r");
    if (!sourceFile)
    {
        perror("File error");
    }
}

/* ----------------- MAIN TOKENIZER ------------------ */
Token getNextToken()
{
    Token token;
    int i = 0;
    char ch;

    memset(token.lexeme, 0, MAX_TOKEN_SIZE);
    token.type = UNKNOWN;

    while ((ch = fgetc(sourceFile)) != EOF)
    {
        if (isspace(ch))
            continue;

        /* ----- IDENTIFIER / KEYWORD ----- */
        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            do
            {
                token.lexeme[i++] = ch;
                ch = fgetc(sourceFile);
            } while (isalnum(ch) || ch == '_');

            ungetc(ch, sourceFile);
            categorizeToken(&token);
            return token;
        }

        /* ------------ NUMBER ------------ */
        if (isdigit(ch))
        {
            i = 0;
            do
            {
                token.lexeme[i++] = ch;
                ch = fgetc(sourceFile);
            } while (isdigit(ch) || ch == '.');

            ungetc(ch, sourceFile);
            token.type = CONSTANT;
            return token;
        }

        /* ------------ COMMENTS FIXED ------------ */
        if (ch == '/')
        {
            char next = fgetc(sourceFile);

            if (next == '/') 
            {
                while ((ch = fgetc(sourceFile)) != '\n' && ch != EOF)
                    ;
                continue;
            }

            if (next == '*')
            {
                while (1)
                {
                    ch = fgetc(sourceFile);
                    if (ch == EOF)
                        break;

                    if (ch == '*')
                    {
                        char end = fgetc(sourceFile);
                        if (end == '/')
                            break;
                        ungetc(end, sourceFile);
                    }
                }
                continue;
            }

            ungetc(next, sourceFile);
        }

        /* ------------ OPERATOR ------------ */
        if (isOperator((char[]){ch, '\0'}))
        {
            char next = fgetc(sourceFile);
            char op2[3] = {ch, next, '\0'};

            if (isOperator(op2))
            {
                strcpy(token.lexeme, op2);
            }
            else
            {
                token.lexeme[0] = ch;
                token.lexeme[1] = '\0';
                ungetc(next, sourceFile);
            }

            token.type = OPERATOR;
            return token;
        }

        /* --------- SPECIAL CHARACTER --------- */
        if (isSpecialCharacter(ch))
        {
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            token.type = SPECIAL_CHARACTER;
            return token;
        }

        /* ---------- UNKNOWN CHAR ---------- */
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        return token;
    }

    token.lexeme[0] = '\0';
    token.type = UNKNOWN;
    return token;
}
