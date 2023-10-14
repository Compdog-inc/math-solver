#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token* tokens[32];
	int count = mathsolver_parse("3x(2x2y+5)x(2zz)3x", tokens, 32);
	count = mathsolver_standardize(tokens, count, 32);
	if (count == 29 &&
		token_isnum(tokens[0], 3) &&
		token_ismul(tokens[1]) &&
		token_isvar(tokens[2], "x") &&
		token_ismul(tokens[3]) &&
		token_isopar(tokens[4]) &&
		token_isnum(tokens[5], 2) &&
		token_ismul(tokens[6]) &&
		token_isvar(tokens[7], "x") &&
		token_ismul(tokens[8]) &&
		token_isnum(tokens[9], 2) &&
		token_ismul(tokens[10]) &&
		token_isvar(tokens[11], "y") &&
		token_isadd(tokens[12]) &&
		token_isnum(tokens[13], 5) &&
		token_iscpar(tokens[14]) &&
		token_ismul(tokens[15]) &&
		token_isvar(tokens[16], "x") &&
		token_ismul(tokens[17]) &&
		token_isopar(tokens[18]) &&
		token_isnum(tokens[19], 2) &&
		token_ismul(tokens[20]) &&
		token_isvar(tokens[21], "z") &&
		token_ismul(tokens[22]) &&
		token_isvar(tokens[23], "z") &&
		token_iscpar(tokens[24]) &&
		token_ismul(tokens[25]) &&
		token_isnum(tokens[26], 3) &&
		token_ismul(tokens[27]) &&
		token_isvar(tokens[28], "x")
		)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}