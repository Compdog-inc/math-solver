#include "mathsolver.h"

int main()
{
	mathsolver_token* tokens[16];
	int count = mathsolver_parse("3+x+y+4", tokens);
	if (
		count == 7 &&
		tokens[1]->type == Operator &&
		tokens[1]->operator == Add &&
		tokens[3]->type == Operator &&
		tokens[3]->operator == Add&&
		tokens[5]->type == Operator &&
		tokens[5]->operator == Add
		)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}