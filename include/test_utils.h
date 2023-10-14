#include "mathsolver.h"
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint8_t token_ismul(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Mul;
}

uint8_t token_isdiv(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Div;
}

uint8_t token_isadd(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Add;
}

uint8_t token_issub(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Sub;
}

uint8_t token_iseq(mathsolver_token *token)
{
    return token->type == Operator && token->operator== EQ;
}

uint8_t token_isneq(mathsolver_token *token)
{
    return token->type == Operator && token->operator== NEQ;
}

uint8_t token_isgt(mathsolver_token *token)
{
    return token->type == Operator && token->operator== GT;
}

uint8_t token_islt(mathsolver_token *token)
{
    return token->type == Operator && token->operator== LT;
}

uint8_t token_isge(mathsolver_token *token)
{
    return token->type == Operator && token->operator== GE;
}

uint8_t token_isle(mathsolver_token *token)
{
    return token->type == Operator && token->operator== LE;
}

uint8_t token_isopar(mathsolver_token *token)
{
    return token->type == Operator && token->operator== OpeningParentheses;
}

uint8_t token_iscpar(mathsolver_token *token)
{
    return token->type == Operator && token->operator== ClosingParentheses;
}

uint8_t token_isfac(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Factorial;
}

uint8_t token_isvar(mathsolver_token *token, char *cmp)
{
    int cl = strlen(cmp);
    if (token->size != cl)
        return 0;
    return token->type == Variable && strcmp(token->variable, cmp) == 0;
}

uint8_t token_isnum(mathsolver_token *token, double num)
{
    char buf[256];
    snprintf(buf, 256, "%f", num);
    int len = strlen(buf);
    char *ptr = buf + len - 1;
    if (strchr(buf, '.') != NULL)
    {
        while (*ptr == '0' || *ptr == '.' /* decimal */)
        {
            *ptr = '\0';
            ptr--;
            len--;
        }
    }

    if (token->size != len)
        return 0;

    return token->type == Number && strcmp(token->number, buf) == 0;
}