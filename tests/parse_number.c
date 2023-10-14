#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token *tokens[16];
	int count = mathsolver_parse("13+5.23-98/(2.3+5)", tokens);
	if (
		count == 11 &&
		token_isnum(tokens[0], 13) &&
		token_isadd(tokens[1]) &&
		token_isnum(tokens[2], 5.23) &&
		token_issub(tokens[3]) &&
		token_isnum(tokens[4], 98) &&
		token_isdiv(tokens[5]) &&
		token_isopar(tokens[6]) &&
		token_isnum(tokens[7], 2.3) &&
		token_isadd(tokens[8]) &&
		token_isnum(tokens[9], 5) &&
		token_iscpar(tokens[10]))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}