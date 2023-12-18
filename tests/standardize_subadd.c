#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token* tokens[32];
	int count = mathsolver_parse("x-2+y", tokens, 32);
	count = mathsolver_standardize(tokens, count, 32, MATHSOLVER_STANDARDIZE_FLAG_NONE);
	if (count == 7 &&
		token_isopar(tokens[0]) &&
		token_isvar(tokens[1], "x") &&
		token_issub(tokens[2]) &&
		token_isnum(tokens[3], 2) &&
		token_iscpar(tokens[4]) &&
		token_isadd(tokens[5]) &&
		token_isvar(tokens[6], "y")
		)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}