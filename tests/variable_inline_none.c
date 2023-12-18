#include "mathsolver.h"

int main()
{
	mathsolver_expression* expression = mathsolver_parsed_expression("3y^2+4y+1", 256);

	mathsolver_expression* xExp = mathsolver_number_expression(3);
	if (xExp != NULL)
	{
		mathsolver_variable vars[] =
		{
			{
				.name = "x",
				.simple_value = xExp
			}
		};

		mathsolver_expression* eval = mathsolver_evaluate(expression, vars, 1);

		mathsolver_inflated_tokens* inflatedOut = mathsolver_from_expression(eval);
		mathsolver_token* tokensOut[64];

		int countOut = mathsolver_deflate(inflatedOut, tokensOut, 64);
		countOut = mathsolver_reduce(tokensOut, countOut);

		char buf[256];
		mathsolver_format(buf, 256, tokensOut, countOut);

		if (eval->copy_of != NULL) // free eval result if it's a copy
			mathsolver_expression_free(&eval);

		mathsolver_expression_free(&expression);
		mathsolver_inflated_tokens_free(&inflatedOut);

		for (int i = 0; i < countOut; i++)
		{
			free(tokensOut[i]);
		}

		if (strcmp(buf, "3(y^2)+(4y+1)") == 0) // reduce workaround (TODO: fix)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}