#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token* tokens[32];
	int count = mathsolver_parse("3*x*(2*x*2*y+5)*x*(2*z*z)*3*x", tokens, 32);
	count = mathsolver_reduce(tokens, count);
	if (count == 21 &&
		token_isnum(tokens[0], 3) &&
		token_isvar(tokens[1], "x") &&
		token_isopar(tokens[2]) &&
		token_isnum(tokens[3], 2) &&
		token_isvar(tokens[4], "x") &&
		token_ismul(tokens[5]) &&
		token_isnum(tokens[6], 2) &&
		token_isvar(tokens[7], "y") &&
		token_isadd(tokens[8]) &&
		token_isnum(tokens[9], 5) &&
		token_iscpar(tokens[10]) &&
		token_ismul(tokens[11]) &&
		token_isvar(tokens[12], "x") &&
		token_isopar(tokens[13]) &&
		token_isnum(tokens[14], 2) &&
		token_isvar(tokens[15], "z") &&
		token_isvar(tokens[16], "z") &&
		token_iscpar(tokens[17]) &&
		token_ismul(tokens[18]) &&
		token_isnum(tokens[19], 3) &&
		token_isvar(tokens[20], "x")
		)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}