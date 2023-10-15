#include "main.h"

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

void printToken(mathsolver_token* token)
{
	switch (token->type)
	{
	case Operator:
	{
		switch (token->operator)
		{
		case Add:
		{
			printf("+");
			break;
		}
		case Sub:
		{
			printf("-");
			break;
		}
		case Mul:
		{
			printf("*");
			break;
		}
		case Div:
		{
			printf("/");
			break;
		}
		case EQ:
		{
			printf("=");
			break;
		}
		case GT:
		{
			printf(">");
			break;
		}
		case LT:
		{
			printf("<");
			break;
		}
		case GE:
		{
			printf(">=");
			break;
		}
		case LE:
		{
			printf("<=");
			break;
		}
		case NEQ:
		{
			printf("!=");
			break;
		}
		case OpeningParentheses:
		{
			printf("OpeningParentheses");
			break;
		}
		case ClosingParentheses:
		{
			printf("ClosingParentheses");
			break;
		}
		case Factorial:
		{
			printf("!");
			break;
		}
		}
		break;
	}
	case Number:
		printf("%.*s", token->size, token->number);
		break;
	case Variable:
		printf("%.*s", token->size, token->variable);
		break;
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
	}
}

void printInflatedTokens(mathsolver_inflated_tokens* tokens)
{
	if (tokens->type == Token)
	{
		printIndent(tokens->depth);
		printToken(tokens->token);
		printf("\n");
	}
	else
	{
		for (int i = 0; i < tokens->children_count; i++)
		{
			printInflatedTokens(tokens->children[i]);
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
	char* str = "3x*(2x+(2y-3)+5)x(2(z/2)z)3x";
	printf("Expression: %s\n", str);

	mathsolver_token* tokens[64];
	int count = mathsolver_parse(str, tokens, 64);
	count = mathsolver_standardize(tokens, count, 64, 0);

	char output[256];
	mathsolver_format(output, 256, tokens, count);
	printf("Standardized: %s\n", output);

	mathsolver_inflated_tokens* inflated = mathsolver_inflate(tokens, count);
	printInflatedTokens(inflated);

	mathsolver_expression* expression = mathsolver_to_expression(inflated);
	printf("Instruction expression:\n");
	printExpression(expression, 0);

	mathsolver_inflated_tokens* inflatedOut = mathsolver_from_expression(expression);
	printf("Inflated out:\n");
	printInflatedTokens(inflatedOut);

	mathsolver_token* tokensOut[64];
	int countOut = mathsolver_deflate(inflatedOut, tokensOut, 64);
	mathsolver_format(output, 256, tokensOut, countOut);
	printf("Deflated: %s\n", output);

	countOut = mathsolver_reduce(tokensOut, countOut);
	mathsolver_format(output, 256, tokensOut, countOut);
	printf("Expression out: %s\n", output);

	mathsolver_expression_free(&expression);
	mathsolver_inflated_tokens_free(&inflated);
	for (int i = 0; i < count; i++)
	{
		mathsolver_token_free(&tokens[i]);
	}

	return 0;
}