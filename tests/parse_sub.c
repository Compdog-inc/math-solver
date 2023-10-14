#include "mathsolver.h"

int main()
{
	mathsolver_token* tokens[16];
	int count = mathsolver_parse("5*3", tokens);
	if (
		count == 3 &&
		tokens[1]->type == Operator &&
		tokens[1]->operator == Mul)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}