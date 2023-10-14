#include "mathsolver.h"

void mathsolver_token_free(mathsolver_token** token)
{
	switch((*token)->type)
	{
	case Variable:
		free((*token)->variable);
		break;
	case Number:
		free((*token)->number);
		break;
	}

	free(*token);
	*token = NULL;
}

uint8_t is_numeric(char ch, char hintPrev, char hintNext)
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

uint8_t is_whitespace(char ch)
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

uint8_t is_alphabetic(char ch)
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

void mathsolver_addtoken(mathsolver_token_type* currentTokenType, char** str, char** currentValueStart, mathsolver_token*** tokens, int* tokenCount)
{
	// get size of token
	int size = sizeof(mathsolver_token_type) + sizeof(char*) + sizeof(int);
	mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
	if (token != NULL)
	{
		// create token
		token->type = *currentTokenType;
		int strLen = (int)(*str - *currentValueStart);
#if _DEBUG
		char* dup = malloc(sizeof(char) * ((size_t)strLen + 1)); // create space for null terminator
#else
		char* dup = malloc(sizeof(char) * strLen);
#endif
		if (dup != NULL)
		{
			memcpy(dup, *currentValueStart, strLen);
#if _DEBUG
			* (dup + strLen) = '\0'; // force null-termination for debugger print
#endif
			if (*currentTokenType == Number)
			{
				token->number = dup;
			}
			else if (*currentTokenType == Variable)
			{
				token->number = dup;
			}
		}
		token->size = strLen;
		(*tokens)[(*tokenCount)++] = token;
	}
	*currentValueStart = NULL;
}

int mathsolver_parse(char *str, mathsolver_token **tokens)
{
	int tokenCount = 0;
	uint8_t strPos = 0;

	char *currentValueStart = NULL;
	uint8_t inSubscript = 0;
	mathsolver_token_type currentTokenType;

	while (*str) // read characters until null
	{
		uint8_t isOperator = 1;
		uint8_t unknownOp = 0;
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
					unknownOp = 1;
					isOperator = 1;
				}
				else if (currentTokenType == Variable)
				{
					// check if in not in subscript and detect invalid chars
					// (anything after first letter that is not subscript is new token)
					if (!inSubscript)
					{
						unknownOp = 1;
						isOperator = 1;
					}
				}
			}

			if (inSubscript && *str == '}')
			{
				inSubscript = 0;
			}
			break;
		}
		}

		if (isOperator && currentValueStart != NULL)
		{
			mathsolver_addtoken(&currentTokenType, &str, &currentValueStart, &tokens, &tokenCount);
		}

		if (queuedToken != NULL)
		{
			tokens[tokenCount++] = queuedToken;
			if (queuedToken->type == Operator)
			{
				// Skip any necessary chars depending on operator
				switch (queuedToken->operator)
				{
				case GE:
				case LE:
				case NEQ:
					str++;
					break;
				}
			}
		}

		if (!unknownOp) {
			str++;		// move start of string to next character
			strPos = 1; // only care about non-zero
		}
	}

	// apply token after EOL
	if (currentValueStart != NULL)
	{
		mathsolver_addtoken(&currentTokenType, &str, &currentValueStart, &tokens, &tokenCount);
	}

	return tokenCount;
}

int mathsolver_format(char* output, int sOutput, mathsolver_token** tokens, int nTokens)
{
	int ind = 0;
	for (int i = 0; i < nTokens; i++)
	{
		if (tokens[i]->type == Operator)
		{
			switch (tokens[i]->operator)
			{
			case Add:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '+';
				break;
			case Sub:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '-';
				break;
			case Mul:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '*';
				break;
			case Div:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '/';
				break;
			case EQ:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '=';
				break;
			case GT:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '>';
				break;
			case LT:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '<';
				break;
			case GE:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '>';
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '=';
				break;
			case LE:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '<';
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '=';
				break;
			case NEQ:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '!';
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '=';
				break;
			case OpeningParentheses:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '(';
				break;
			case ClosingParentheses:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = ')';
				break;
			case Factorial:
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = '!';
				break;
			}
		}
		else if(tokens[i]->type == Variable)
		{
			for (int j = 0; j < tokens[i]->size; j++)
			{
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = tokens[i]->variable[j];
			}
		}
		else if (tokens[i]->type == Number)
		{
			for (int j = 0; j < tokens[i]->size; j++)
			{
				if (ind < sOutput - 2) // null terminator + one less
					output[ind++] = tokens[i]->number[j];
			}
		}
	}
	output[ind++] = '\0';
	return ind;
}

int mathsolver_insert_token(mathsolver_token* token, int index, mathsolver_token** tokens, int nTokens, int limitTokens)
{
	nTokens++;
	if (nTokens <= limitTokens) {
		for (int i = nTokens; i > index; i--)
		{
			tokens[i] = tokens[i - 1];
		}
		tokens[index] = token;
	}
	return nTokens;
}

int mathsolver_standardize(mathsolver_token** tokens, int nTokens, int limitTokens)
{
	for (int i = 0; i < nTokens; i++)
	{
		// check for implicit multiplication
		// [not-op][var] or [var][not-op] or [not-op][opar] or [cpar][not-op]
		// for example: 3x, yx, x3[x], xy[x], 3(, x(, )3[x], )x
		if (
			(i > 0 && tokens[i]->type == Variable && tokens[i - 1]->type != Operator) ||
			(i > 0 && tokens[i]->type != Operator && tokens[i - 1]->type == Variable) ||
			(i > 0 && tokens[i]->type == Operator && tokens[i]->operator == OpeningParentheses && tokens[i - 1]->type != Operator) ||
			(i > 0 && tokens[i]->type != Operator && tokens[i - 1]->type == Operator && tokens[i - 1]->operator == ClosingParentheses)
			)
		{
			// get size of token
			int size = sizeof(mathsolver_token_type) + sizeof(mathsolver_operator) + sizeof(int);
			mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
			if (token != NULL)
			{
				// create token
				token->type = Operator;
				token->operator= Mul;
				token->size = size;

				// insert Mul op at current index and shift tokens
				nTokens = mathsolver_insert_token(token, i, tokens, nTokens, limitTokens);
			}
		}
	}

	return nTokens;
}