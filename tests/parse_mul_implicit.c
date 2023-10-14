#include "mathsolver.h"
#include "test_utils.h"

int main()
{
	mathsolver_token* tokens[16];
	int count = mathsolver_parse("3x+xy+x(3yx)", tokens, 16);
	if (
		count == 12 &&
		token_isnum(tokens[0], 3) &&
		token_isvar(tokens[1], "x") &&
		token_isadd(tokens[2]) &&
		token_isvar(tokens[3], "x") &&
		token_isvar(tokens[4], "y") &&
		token_isadd(tokens[5]) &&
		token_isvar(tokens[6], "x") &&
		token_isopar(tokens[7]) &&
		token_isnum(tokens[8], 3) &&
		token_isvar(tokens[9], "y") &&
		token_isvar(tokens[10], "x") &&
		token_iscpar(tokens[11])
		)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}