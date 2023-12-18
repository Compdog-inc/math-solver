#include "mathsolver.h"

int main()
{
    mathsolver_token* tokens[64];
    int count = mathsolver_parse("3x*(2x+(2y-3)+5)x(2(z/2)z)3x", tokens, 64);
    count = mathsolver_standardize(tokens, count, 64, 0);
    mathsolver_inflated_tokens* inflated = mathsolver_inflate(tokens, count);
    mathsolver_expression* expression = mathsolver_to_expression(inflated, 0);

    mathsolver_inflated_tokens* inflatedOut = mathsolver_from_expression(expression);
    mathsolver_token* tokensOut[64];
    
    int countOut = mathsolver_deflate(inflatedOut, tokensOut, 64);
    countOut = mathsolver_reduce(tokensOut, countOut);

    char buf[256];
    mathsolver_format(buf, 256, tokensOut, countOut);

    mathsolver_expression_free(&expression);
    mathsolver_inflated_tokens_free(&inflated);
    mathsolver_inflated_tokens_free(&inflatedOut);
    for (int i = 0; i < count; i++)
    {
        mathsolver_token_free(&tokens[i]);
    }
    for (int i = 0; i < countOut; i++)
    {
        free(tokensOut[i]);
    }

    if (strcmp(buf, "3x(2x+((2y-3)+5))*x(2z/2)*z*3x") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}