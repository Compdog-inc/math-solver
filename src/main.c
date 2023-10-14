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

int main()
{
	char* str = "3x*(2x+(2y-3)+5)x(2(z/2)z)3x";
	printf("Expression: %s\n", str);

	mathsolver_token* tokens[64];
	int count = mathsolver_parse(str, tokens, 64);
	mathsolver_inflated_tokens* inflated = mathsolver_inflate(tokens, count);
	printInflatedTokens(inflated);
	mathsolver_inflated_tokens_free(&inflated);

	return 0;
}