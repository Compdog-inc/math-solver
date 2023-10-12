#include "mathsolver.h"

int mathsolver_parse(char* str, mathsolver_token** tokens)
{
	int tokenCount = 0;

	while (*str)		// read characters until null
	{		
		switch (*str)
		{
		case '*':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
			if (token != NULL) {
				// create token
				token->type = Operator;
				token->operator = Mul;
				token->size = size;
				tokens[tokenCount++] = token;
			}
			break;
		}
		case '/':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
			if (token != NULL) {
				// create token
				token->type = Operator;
				token->operator = Div;
				token->size = size;
				tokens[tokenCount++] = token;
			}
			break;
		}
		case '+':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
			if (token != NULL) {
				// create token
				token->type = Operator;
				token->operator = Add;
				token->size = size;
				tokens[tokenCount++] = token;
			}
			break;
		}
		case '-':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
			if (token != NULL) {
				// create token
				token->type = Operator;
				token->operator = Sub;
				token->size = size;
				tokens[tokenCount++] = token;
			}
			break;
		}
		default:
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(char*) + sizeof(int);
			mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
			if (token != NULL) {
				// create token
				token->type = Variable;
				token->variable = str;
				token->size = size;
				tokens[tokenCount++] = token;
			}
			break;
		}
		}

		str++;			// move start of string to next character
	}

	return tokenCount;
}