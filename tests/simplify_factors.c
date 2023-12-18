#include "mathsolver.h"

void printIndent(int depth)
{
	// each depth is +4 spaces
	if (depth > 0)
	{
		for (int i = 0; i < depth - 1; i++)
		{
			printf("|   ");
		}

		printf("|___");
	}
}

void printInstruction(mathsolver_instruction instr)
{
	switch (instr)
	{
	case iAdd:
	{
		printf("add");
		break;
	}
	case iSub:
	{
		printf("sub");
		break;
	}
	case iNeg:
	{
		printf("neg");
		break;
	}
	case iMul:
	{
		printf("mul");
		break;
	}
	case iDiv:
	{
		printf("div");
		break;
	}
	case iEq:
	{
		printf("eq");
		break;
	}
	case iGt:
	{
		printf("gt");
		break;
	}
	case iLt:
	{
		printf("lt");
		break;
	}
	case iGe:
	{
		printf("ge");
		break;
	}
	case iLe:
	{
		printf("le");
		break;
	}
	case iNeq:
	{
		printf("neq");
		break;
	}
	case iFactorial:
	{
		printf("fac");
		break;
	}
	case iExponent:
	{
		printf("exp");
		break;
	}
	}
}

void printExpression(mathsolver_expression* expression, int depth)
{
    if (expression->type == expNumber)
    {
        printIndent(depth);
        printf("%f\n", expression->number);
    }
    else if (expression->type == expVariable)
    {
        printIndent(depth);
        printf("%.*s\n", expression->variable_length, expression->variable);
    }
    else
    {
        printIndent(depth);
        printInstruction(expression->instruction);
        printf("\n");

        for (int i = 0; i < expression->node_count; i++)
        {
            printExpression(expression->nodes[i], depth + 1);
        }
    }
}

int main()
{
    mathsolver_expression* expression = mathsolver_parsed_expression("(x-2)(x+2)", 64);

    mathsolver_expression* eval = mathsolver_evaluate(expression, NULL, 0);
    mathsolver_expression* simplifiedRaw = mathsolver_simplify(eval);
	mathsolver_expression* deepSimplified = mathsolver_to_deep(simplifiedRaw);
	mathsolver_expression* simplified = mathsolver_evaluate(deepSimplified, NULL, 0);
	simplifiedRaw = mathsolver_simplify(simplified);
	deepSimplified = mathsolver_to_deep(simplifiedRaw);
	simplified = mathsolver_evaluate(deepSimplified, NULL, 0);

    char buf[256];
    int len = mathsolver_format_expression(buf, 256, simplified, 64);
	buf[len] = '\0';
	printf("%s\n", buf);
    //mathsolver_expression_free(&expression);
    //mathsolver_expression_free(&eval);
    //mathsolver_expression_free(&simplified);

    if (strcmp(buf, "(((x^2)+x*2)+(-x*2))-4") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}