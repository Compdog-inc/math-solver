#include "mathsolver.h"
#include <memory.h>
#include <stdio.h>

__uint8_t token_ismul(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Mul;
}

__uint8_t token_isdiv(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Div;
}

__uint8_t token_isadd(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Add;
}

__uint8_t token_issub(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Sub;
}

__uint8_t token_iseq(mathsolver_token *token)
{
    return token->type == Operator && token->operator== EQ;
}

__uint8_t token_isneq(mathsolver_token *token)
{
    return token->type == Operator && token->operator== NEQ;
}

__uint8_t token_isgt(mathsolver_token *token)
{
    return token->type == Operator && token->operator== GT;
}

__uint8_t token_islt(mathsolver_token *token)
{
    return token->type == Operator && token->operator== LT;
}

__uint8_t token_isge(mathsolver_token *token)
{
    return token->type == Operator && token->operator== GE;
}

__uint8_t token_isle(mathsolver_token *token)
{
    return token->type == Operator && token->operator== LE;
}

__uint8_t token_isopar(mathsolver_token *token)
{
    return token->type == Operator && token->operator== OpeningParentheses;
}

__uint8_t token_iscpar(mathsolver_token *token)
{
    return token->type == Operator && token->operator== ClosingParentheses;
}

__uint8_t token_isfac(mathsolver_token *token)
{
    return token->type == Operator && token->operator== Factorial;
}

__uint8_t token_isvar(mathsolver_token *token, char *cmp)
{
    int cl = strlen(cmp);
    if (token->size != cl)
        return 0;
    return token->type == Variable && strcmp(token->variable, cmp) == 0;
}

__uint8_t token_isnum(mathsolver_token *token, double num)
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