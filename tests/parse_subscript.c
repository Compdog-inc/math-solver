#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token *tokens[16];
	int count = mathsolver_parse("3*x_{0}=5+3/y_{in}", &tokens);
	if (
		count == 9 &&
		token_isnum(tokens[0], 3) &&
		token_ismul(tokens[1]) &&
		token_isvar(tokens[2], "x_{0}") &&
		token_iseq(tokens[3]) &&
		token_isnum(tokens[4], 5) &&
		token_isadd(tokens[5]) &&
		token_isnum(tokens[6], 3) &&
		token_isdiv(tokens[7]) &&
		token_isvar(tokens[8], "y_{in}"))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}