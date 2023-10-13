#include "mathsolver.h"

__uint8_t is_numeric(char ch, char hintPrev, char hintNext)
{
	/*
		!NOTE! Does not count (-) as numeric because the lexer parses that as [0][sub][numeric]
	*/
	if (ch >= '0' && ch <= '9')
	{
		return 1;
	}
	else if (
		ch == '.' &&						 // decimal point
		(is_numeric(hintPrev, '\0', '\0') || // has number before or after
		 is_numeric(hintNext, '\0', '\0')))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

__uint8_t is_whitespace(char ch)
{
	if (ch == ' ' || ch == '\n' || ch == '\r')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

__uint8_t is_alphabetic(char ch)
{
	// A-Z
	if (ch >= 'A' && ch <= 'Z')
	{
		return 1;
	}
	// a-z
	else if (ch >= 'a' && ch <= 'z')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int mathsolver_parse(char *str, mathsolver_token **tokens)
{
	int tokenCount = 0;
	__uint8_t strPos = 0;

	char *currentValueStart = NULL;
	__uint8_t inSubscript = 0;
	mathsolver_token_type currentTokenType;

	while (*str) // read characters until null
	{
		__uint8_t isOperator = 1;
		mathsolver_token *queuedToken = NULL;

		switch (*str)
		{
		case '*':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= Mul;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '/':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= Div;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '+':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= Add;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '-':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= Sub;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '=':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= EQ;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '>':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				if (*(str + 1) == '=')
				{
					str++;
					token->operator= GE;
				}
				else
				{
					token->operator= GT;
				}
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '<':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				if (*(str + 1) == '=')
				{
					str++;
					token->operator= LE;
				}
				else
				{
					token->operator= LT;
				}
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '(':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= OpeningParentheses;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case ')':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= ClosingParentheses;
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		case '!':
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				if (*(str + 1) == '=')
				{
					str++;
					token->operator= NEQ;
				}
				else
				{
					token->operator= Factorial;
				}
				token->size = size;
				queuedToken = token;
			}
			break;
		}
		default:
		{
			if (!inSubscript && *str == '_' && *(str + 1) == '{')
			{
				inSubscript = 1;
			}

			if (inSubscript && *str == '}')
			{
				inSubscript = 0;
			}

			isOperator = 0;
			if (currentValueStart == NULL && !is_whitespace(*str))
			{
				// detect value type
				if (is_numeric(*str, strPos ? *(str - 1) : '\0', (char)*(str + 1)))
				{
					currentValueStart = str;
					currentTokenType = Number;
				}
				else if (is_alphabetic(*str))
				{
					currentValueStart = str;
					currentTokenType = Variable;
				}
			}
			else if (currentValueStart != NULL)
			{
				if (currentTokenType == Number && !is_numeric(*str, strPos ? *(str - 1) : '\0', (char)*(str + 1)))
				{
					isOperator = 1;
				}
				else if (currentTokenType == Variable)
				{
					// check if in not in subscript and detect invalid chars
					// (anything after first letter that is not subscript is illegal)
					if (!inSubscript)
					{
						return -1;
					}
				}
			}
			break;
		}
		}

		if (isOperator && currentValueStart != NULL)
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(char *) + sizeof(int);
			mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = currentTokenType;
				int strLen = (int)(str - currentValueStart);
				if (currentTokenType == Number)
				{
					token->number = strndup(currentValueStart, strLen);
				}
				else if (currentTokenType == Variable)
				{
					token->number = strndup(currentValueStart, strLen);
				}
				token->size = strLen;
				tokens[tokenCount++] = token;
			}
			currentValueStart = NULL;
		}

		if (queuedToken != NULL)
		{
			tokens[tokenCount++] = queuedToken;
		}

		str++;		// move start of string to next character
		strPos = 1; // only care about non-zero
	}

	// apply token after EOL
	if (currentValueStart != NULL)
	{
		// get size of token
		int size = sizeof(mathsolver_token_type) + sizeof(char *) + sizeof(int);
		mathsolver_token *token = (mathsolver_token *)malloc(size); // allocate
		if (token != NULL)
		{
			// create token
			token->type = currentTokenType;
			int strLen = (int)(str - currentValueStart);
			if (currentTokenType == Number)
			{
				token->number = strndup(currentValueStart, strLen);
			}
			else if (currentTokenType == Variable)
			{
				token->number = strndup(currentValueStart, strLen);
			}
			token->size = strLen;
			tokens[tokenCount++] = token;
		}
		currentValueStart = NULL;
	}

	return tokenCount;
}