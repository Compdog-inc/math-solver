#include "main.h"
#include <crtdbg.h>

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
		case Exponent:
		{
			printf("^");
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
	case iExponent:
	{
		printf("exp");
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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	while (1) {
		printf("Enter expression:\n");

		char str[256];
		if (fgets(str, 256, stdin) == NULL)
			return 1;

		char output[256];
		
		mathsolver_expression* expression = mathsolver_parsed_expression(str, 256);
		printf("Instruction expression:\n");
		printExpression(expression, 0);

		printf("Evaluating expression...\n");

		mathsolver_expression* xExp = mathsolver_number_expression(3);
		if (xExp != NULL)
		{
			mathsolver_variable vars[] =
			{
				{
					.indexed_value = xExp
				}
			};
			char* var_indexes[] =
			{
				"x"
			};

			mathsolver_push_variable_table(expression, var_indexes, 1);
			mathsolver_expression* eval = mathsolver_evaluate(expression, vars, 1);
			mathsolver_pop_variable_table(expression, var_indexes, 1);
			printf("Evaluated expression:\n");
			printExpression(eval, 0);

			mathsolver_inflated_tokens* inflatedOut = mathsolver_from_expression(eval);
			printf("Inflated out:\n");
			printInflatedTokens(inflatedOut);

			mathsolver_token* tokensOut[64];
			int countOut = mathsolver_deflate(inflatedOut, tokensOut, 64);
			mathsolver_format(output, 256, tokensOut, countOut);
			printf("Deflated: %s\n", output);

			countOut = mathsolver_reduce(tokensOut, countOut);
			mathsolver_format(output, 256, tokensOut, countOut);
			printf("Expression out: %s\n", output);

			if (eval->copy_of != NULL) // free eval result if it's a copy
				mathsolver_expression_free(&eval);

			mathsolver_expression_free(&expression);

			printf("Done.\n");
			break;
		}
	}
	return 0;
}