#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	mathsolver_token* tokens[32];
	int count = mathsolver_parse("3x*(2x+(2y-3)+5)x(2(z/2)z)3x", tokens, 32);
	mathsolver_inflated_tokens* inflated = mathsolver_inflate(tokens, count);

	int targetCount = mathsolver_count_inflated(inflated);
	mathsolver_token** outTokens = malloc(sizeof(mathsolver_token*) * targetCount);
	if (outTokens == NULL) return 2;
	int countOut = mathsolver_deflate(inflated, outTokens, targetCount);

	if (countOut != targetCount || count != countOut) return 1;

	for (int i = 0; i < countOut; i++)
	{
		if (tokens[i]->type != outTokens[i]->type || tokens[i]->size != outTokens[i]->size)
			return 1;

		switch (tokens[i]->type)
		{
		case Operator:
			if (tokens[i]->operator != outTokens[i]->operator)
				return 1;
			break;
		case Number:
			if (strcmp(tokens[i]->number, outTokens[i]->number) != 0)
				return 1;
			break;
		case Variable:
			if (strcmp(tokens[i]->variable, outTokens[i]->variable) != 0)
				return 1;
			break;
		}
	}

	mathsolver_inflated_tokens_free(&inflated);
	free(outTokens);

	return 0;
}