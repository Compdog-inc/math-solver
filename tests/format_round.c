#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token *tokens[32];

	char* str = "3a*3!*x_{sub}+n!=z(5x+3)/(9-2)";
	int count = mathsolver_parse(str, tokens, 32);

	char output[256];
	mathsolver_format(output, 256, tokens, count);

	if (strcmp(str, output) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}