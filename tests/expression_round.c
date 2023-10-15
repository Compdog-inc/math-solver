#include "mathsolver.h"

int main()
{
    mathsolver_token* tokens[64];
    int count = mathsolver_parse("3x*(2x+(2y-3)+5)x(2(z/2)z)3x", tokens, 64);
    count = mathsolver_standardize(tokens, count, 64, 0);
    mathsolver_inflated_tokens* inflated = mathsolver_inflate(tokens, count);
    mathsolver_expression* expression = mathsolver_to_expression(inflated);



    mathsolver_expression_free(&expression);
    mathsolver_inflated_tokens_free(&inflated);
    for (int i = 0; i < count; i++)
    {
        mathsolver_token_free(&tokens[i]);
    }
}