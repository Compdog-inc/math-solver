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

void mathsolver_inflated_tokens_free(mathsolver_inflated_tokens** tokens)
{
	if ((*tokens)->type == InflatedList)
	{
		// free children list and children
		for (int i = 0; i < (*tokens)->children_count; i++)
		{
			mathsolver_inflated_tokens_free(&((*tokens)->children[i]));
		}
		free((*tokens)->children);
	}

	free(*tokens);
	*tokens = NULL;
}

void mathsolver_expression_free(mathsolver_expression** expression)
{
	if ((*expression)->type == expInstruction)
	{
		for (int i = 0; i < (*expression)->node_count; i++)
		{
			mathsolver_expression_free(&((*expression)->nodes[i]));
		}
		free((*expression)->nodes);
	}

	free(*expression);
	*expression = NULL;
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

uint8_t is_integer(double value)
{
	if (fabs(round(value) - value) <= 0.00000001)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void mathsolver_addtoken(mathsolver_token_type* currentTokenType, char** str, char** currentValueStart, mathsolver_token*** tokens, int* tokenCount, int nTokens)
{
	if ((*tokenCount) < nTokens - 1) {
		// get size of token
		int size = sizeof(mathsolver_token);
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
	}
	*currentValueStart = NULL;
}

int mathsolver_parse(char *str, mathsolver_token **tokens, int nTokens)
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			int size = sizeof(mathsolver_token);
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
			mathsolver_addtoken(&currentTokenType, &str, &currentValueStart, &tokens, &tokenCount, nTokens);
		}

		if (queuedToken != NULL)
		{
			if (tokenCount < nTokens - 1) {
				tokens[tokenCount++] = queuedToken;
			}
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
		mathsolver_addtoken(&currentTokenType, &str, &currentValueStart, &tokens, &tokenCount, nTokens);
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
	if (nTokens < limitTokens) {
		nTokens++;
		for (int i = nTokens; i > index; i--)
		{
			tokens[i] = tokens[i - 1];
		}
		tokens[index] = token;
	}
	return nTokens;
}

int mathsolver_remove_token(int index, mathsolver_token** tokens, int nTokens)
{
	nTokens--;
	for (int i = index; i < nTokens; i++)
	{
		tokens[i] = tokens[i + 1];
	}
	return nTokens;
}

int mathsolver_standardize(mathsolver_token** tokens, int nTokens, int limitTokens, uint8_t flags)
{
	// single token pass
	for (int i = 0; i < nTokens && i < limitTokens; i++)
	{
		// check for implicit multiplication
		// [not-op][var] or [var][not-op] or [not-op][opar] or [cpar][not-op] or [cpar][opar]
		// for example: 3x, yx, x3[x], xy[x], 3(, x(, )3[x], )x, )(
		if (
			(i > 0 && tokens[i]->type == Variable && tokens[i - 1]->type != Operator) ||
			(i > 0 && tokens[i]->type != Operator && tokens[i - 1]->type == Variable) ||
			(i > 0 && tokens[i]->type == Operator && tokens[i]->operator == OpeningParentheses && tokens[i - 1]->type != Operator) ||
			(i > 0 && tokens[i]->type != Operator && tokens[i - 1]->type == Operator && tokens[i - 1]->operator == ClosingParentheses) ||
			(i > 0 && tokens[i]->type == Operator && tokens[i]->operator == OpeningParentheses && tokens[i - 1]->type == Operator && tokens[i - 1]->operator == ClosingParentheses)
			)
		{
			// get size of token
			int size = sizeof(mathsolver_token);
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

		if (!(flags & MATHSOLVER_STANDARDIZE_FLAG_IMPLICIT_ORDER_OF_OPERATIONS))
		{
			// explicitly state order of operations
			// 3*1/1+2 becomes ((3*1)/1)+2
			if (tokens[i]->type == Operator)
			{
				switch (tokens[i]->operator)
				{
				case Factorial:
				{
					// explicitly state factorial operator
					// -3!*2 becomes -(3!)*2
					if (i>0 && i < nTokens && i < limitTokens)
					{
						int openIndex = i - 1;
						int closeIndex = i + 1;

						if (tokens[i - 1]->type != Operator)
						{
							openIndex = i - 1;
						}
						else if (tokens[i - 1]->operator == ClosingParentheses)
						{
							// search left for opening parentheses with same relative depth
							int depth = 0;
							for (int j = i - 2; j >= 0; j--)
							{
								if (tokens[j]->type == Operator)
								{
									if (tokens[j]->operator == ClosingParentheses)
									{
										depth++;
									}
									else if (tokens[j]->operator == OpeningParentheses)
									{
										if (depth == 0)
										{
											openIndex = j;
											break;
										}
										else
										{
											depth--;
										}
									}
								}
							}
						}

						// get size of token
						int size = sizeof(mathsolver_token);
						mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
						if (token != NULL)
						{
							// create token
							token->type = Operator;
							token->operator= OpeningParentheses;
							token->size = size;

							// insert opar op at open index and shift tokens
							nTokens = mathsolver_insert_token(token, openIndex, tokens, nTokens, limitTokens);
						}

						// get size of token
						size = sizeof(mathsolver_token);
						token = (mathsolver_token*)malloc(size); // allocate
						if (token != NULL)
						{
							// create token
							token->type = Operator;
							token->operator= ClosingParentheses;
							token->size = size;

							// insert cpar op at close index + 1 (since prev insert shifted this index) and shift tokens
							nTokens = mathsolver_insert_token(token, closeIndex + 1, tokens, nTokens, limitTokens);
						}

						i++;
					}
					break;
				}
				}
			}
		}
	}

	// double token pass
	for (int i = 0; i < nTokens && i < limitTokens; i++)
	{
		if (!(flags & MATHSOLVER_STANDARDIZE_FLAG_IMPLICIT_ORDER_OF_OPERATIONS))
		{
			// explicitly state order of operations
			// 3*1/1+2 becomes ((3*1)/1)+2
			if (tokens[i]->type == Operator)
			{
				switch (tokens[i]->operator)
				{
				case Sub:
				{
					// explicitly state negate operator
					// -(x+2) becomes (-(x+2))
					// Note: ONLY if nothing or ( to the left
					if (i < nTokens - 1 && i < limitTokens - 1)
					{
						int openIndex = -1;
						if (i == 0 || (i > 0 && tokens[i - 1]->type == Operator && tokens[i - 1]->operator != ClosingParentheses))
						{
							openIndex = i;
						}
						else
						{
							break;
						}

						int closeIndex = i + 1;

						if (tokens[i + 1]->type != Operator)
						{
							closeIndex = i + 2;
						}
						else if (tokens[i + 1]->operator == OpeningParentheses)
						{
							// search right for closing parentheses with same relative depth
							int depth = 0;
							for (int j = i + 2; j < nTokens && j < limitTokens; j++)
							{
								if (tokens[j]->type == Operator)
								{
									if (tokens[j]->operator == OpeningParentheses)
									{
										depth++;
									}
									else if (tokens[j]->operator == ClosingParentheses)
									{
										if (depth == 0)
										{
											closeIndex = j + 1;
											break;
										}
										else
										{
											depth--;
										}
									}
								}
							}
						}

						// get size of token
						int size = sizeof(mathsolver_token);
						mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
						if (token != NULL)
						{
							// create token
							token->type = Operator;
							token->operator= OpeningParentheses;
							token->size = size;

							// insert opar op at open index and shift tokens
							nTokens = mathsolver_insert_token(token, openIndex, tokens, nTokens, limitTokens);
						}

						// get size of token
						size = sizeof(mathsolver_token);
						token = (mathsolver_token*)malloc(size); // allocate
						if (token != NULL)
						{
							// create token
							token->type = Operator;
							token->operator= ClosingParentheses;
							token->size = size;

							// insert cpar op at close index + 1 (since prev insert shifted this index) and shift tokens
							nTokens = mathsolver_insert_token(token, closeIndex + 1, tokens, nTokens, limitTokens);
						}

						i++;
					}
					break;
				}
				}
			}
		}
	}

	// triple token pass
	for (int i = 0; i < nTokens && i < limitTokens; i++)
	{
		if (!(flags & MATHSOLVER_STANDARDIZE_FLAG_IMPLICIT_ORDER_OF_OPERATIONS))
		{
			// explicitly state order of operations
			// 3*1/1+2 becomes ((3*1)/1)+2
			if (tokens[i]->type == Operator)
			{
				switch (tokens[i]->operator)
				{
				case Mul:
				case Div:
				{
					if (i < nTokens - 1 && i < limitTokens - 1)
					{
						// prevent unnecessary parentheses ((1*1))
						if (!(i > 1 && i < nTokens - 2 && i < limitTokens - 2 &&
							tokens[i - 1]->type != Operator &&
							tokens[i + 1]->type != Operator &&
							tokens[i - 2]->type == Operator &&
							tokens[i - 2]->operator == OpeningParentheses &&
							tokens[i + 2]->type == Operator &&
							tokens[i + 2]->operator == ClosingParentheses
							))
						{
							int openIndex = i - 1;
							int closeIndex = i + 1;

							if (tokens[i - 1]->type != Operator)
							{
								openIndex = i - 1;
							}
							else if (tokens[i - 1]->operator == ClosingParentheses)
							{
								// search left for opening parentheses with same relative depth
								int depth = 0;
								for (int j = i - 2; j >= 0; j--)
								{
									if (tokens[j]->type == Operator)
									{
										if (tokens[j]->operator == ClosingParentheses)
										{
											depth++;
										}
										else if (tokens[j]->operator == OpeningParentheses)
										{
											if (depth == 0)
											{
												openIndex = j;
												break;
											}
											else
											{
												depth--;
											}
										}
									}
								}
							}

							if (tokens[i + 1]->type != Operator)
							{
								closeIndex = i + 2;
							}
							else if (tokens[i + 1]->operator == OpeningParentheses)
							{
								// search right for closing parentheses with same relative depth
								int depth = 0;
								for (int j = i + 2; j < nTokens && j < limitTokens; j++)
								{
									if (tokens[j]->type == Operator)
									{
										if (tokens[j]->operator == OpeningParentheses)
										{
											depth++;
										}
										else if (tokens[j]->operator == ClosingParentheses)
										{
											if (depth == 0)
											{
												closeIndex = j + 1;
												break;
											}
											else
											{
												depth--;
											}
										}
									}
								}
							}

							// get size of token
							int size = sizeof(mathsolver_token);
							mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
							if (token != NULL)
							{
								// create token
								token->type = Operator;
								token->operator= OpeningParentheses;
								token->size = size;

								// insert opar op at open index and shift tokens
								nTokens = mathsolver_insert_token(token, openIndex, tokens, nTokens, limitTokens);
							}

							// get size of token
							size = sizeof(mathsolver_token);
							token = (mathsolver_token*)malloc(size); // allocate
							if (token != NULL)
							{
								// create token
								token->type = Operator;
								token->operator= ClosingParentheses;
								token->size = size;

								// insert cpar op at close index + 1 (since prev insert shifted this index) and shift tokens
								nTokens = mathsolver_insert_token(token, closeIndex + 1, tokens, nTokens, limitTokens);
							}

							i++;
						}
					}
					break;
				}
				}
			}
		}
	}

	return nTokens;
}

int mathsolver_reduce(mathsolver_token** tokens, int nTokens)
{
	for (int i = nTokens - 1; i >= 0; i--)
	{
		// reduce explicitly stated order of operations
		// ((3*1)/1)+2 becomes 3*1/1+2 but (3*(1+2)) becomes 3*(1+2)
		if (i > 0 && tokens[i]->type == Operator)
		{
			switch (tokens[i]->operator)
			{
			case Mul:
			case Div:
			{
				// prevent reducing no parentheses _1*1_ -> 1*1
				if (i > 1 && i < nTokens - 2)
				{
					int openIndex = -1;
					int closeIndex = -1;

					if (tokens[i - 1]->type != Operator && tokens[i - 2]->type == Operator && tokens[i - 2]->operator == OpeningParentheses)
					{
						openIndex = i - 2;
					}
					else if (tokens[i - 1]->operator == ClosingParentheses)
					{
						// search left for opening parentheses with same relative depth
						int depth = 0;
						for (int j = i - 2; j >= 0; j--)
						{
							if (tokens[j]->type == Operator)
							{
								if (tokens[j]->operator == ClosingParentheses)
								{
									depth++;
								}
								else if (tokens[j]->operator == OpeningParentheses)
								{
									if (depth == -1)
									{
										openIndex = j;
										break;
									}
									else
									{
										depth--;
									}
								}
							}
						}
					}

					if (tokens[i + 1]->type != Operator && tokens[i + 2]->type == Operator && tokens[i + 2]->operator == ClosingParentheses)
					{
						closeIndex = i + 2;
					}
					else if (tokens[i + 1]->operator == OpeningParentheses)
					{
						// search right for closing parentheses with same relative depth
						int depth = 0;
						for (int j = i + 2; j < nTokens; j++)
						{
							if (tokens[j]->type == Operator)
							{
								if (tokens[j]->operator == OpeningParentheses)
								{
									depth++;
								}
								else if (tokens[j]->operator == ClosingParentheses)
								{
									if (depth == -1)
									{
										closeIndex = j;
										break;
									}
									else
									{
										depth--;
									}
								}
							}
						}
					}

					if (closeIndex != -1 && openIndex != -1)
					{
						nTokens = mathsolver_remove_token(closeIndex, tokens, nTokens);
						nTokens = mathsolver_remove_token(openIndex, tokens, nTokens);
						i--;
					}
				}
				break;
			}
			case Factorial:
			{
				break;
			}
			}
		}
	}

	// second pass because implicit order of operations reduces from both sides
	for (int i = nTokens - 1; i >= 0; i--)
	{
		// reduce implicit multiplication
		// [not-op]*[var] or [not-op]*[opar]
		// for example: 3*x, y*x, 3*(, x*(
		if (
			i > 0 && i < nTokens - 1 && tokens[i]->type == Operator && tokens[i]->operator == Mul && (
				(tokens[i - 1]->type != Operator && tokens[i + 1]->type == Variable) ||
				(tokens[i - 1]->type != Operator && tokens[i + 1]->type == Operator && tokens[i + 1]->operator == OpeningParentheses)
				)
			)
		{
			nTokens = mathsolver_remove_token(i, tokens, nTokens);
		}
	}

	return nTokens;
}

mathsolver_inflated_tokens* mathsolver_inflate(mathsolver_token** tokens, int nTokens)
{
	mathsolver_inflated_tokens* root = malloc(sizeof(mathsolver_inflated_tokens));
	if (root == NULL) return NULL;

	mathsolver_inflated_tokens* currentChild = root;
	mathsolver_token** currentDepthTokens = malloc(sizeof(mathsolver_token*) * nTokens);
	if (currentDepthTokens != NULL) {
		int tind = 0;
		int depth = 0;
		root->depth = depth;
		root->parent = NULL;
		root->initialized = 0;
		for (int i = 0; i < nTokens; i++)
		{
			if (tokens[i]->type == Operator && tokens[i]->operator == OpeningParentheses)
			{
				depth++;
				
				mathsolver_inflated_tokens* tmp = currentChild;
				// add current depth tokens to current depth child
				currentChild->type = InflatedList;
				if (!currentChild->initialized)
				{
					currentChild->initialized = 1;
					currentChild->children_count = 0;
					currentChild->max_count = tind+1;
					currentChild->children = malloc(sizeof(mathsolver_inflated_tokens*) * currentChild->max_count);
				}
				for (int j = 0; j < tind; j++)
				{
					mathsolver_inflated_tokens* child = malloc(sizeof(mathsolver_inflated_tokens));
					if (child == NULL) return NULL;
					child->type = Token;
					child->depth = depth;
					child->token = currentDepthTokens[j];
					child->parent = currentChild;
					child->initialized = 1;

					if (currentChild->children_count >= currentChild->max_count)
					{
						mathsolver_inflated_tokens** rel = realloc(currentChild->children, sizeof(mathsolver_inflated_tokens*) * currentChild->max_count * 2);
						if (rel != NULL)
						{
							currentChild->children = rel;
							currentChild->max_count = currentChild->max_count * 2;
						}
						else
						{
							return NULL;
						}
					}
					currentChild->children[currentChild->children_count++] = child;
				}
				tind = 0;

				// create new depth list
				currentChild = malloc(sizeof(mathsolver_inflated_tokens));
				if (currentChild != NULL)
				{
					currentChild->depth = depth;
					currentChild->parent = tmp;
					currentChild->initialized = 0;

					if (tmp->children_count >= tmp->max_count)
					{
						mathsolver_inflated_tokens** rel = realloc(tmp->children, sizeof(mathsolver_inflated_tokens*) * tmp->max_count * 2);
						if (rel != NULL)
						{
							tmp->children = rel;
							tmp->max_count = tmp->max_count * 2;
						}
						else
						{
							return NULL;
						}
					}
					if (tmp->children != NULL)
					{
						tmp->children[tmp->children_count++] = currentChild;
					}
				}
				else
				{
					return NULL;
				}
			}
			else if (tokens[i]->type == Operator && tokens[i]->operator == ClosingParentheses)
			{
				if (depth > 0)
				{
					depth--;

					// add current depth tokens to current depth child
					currentChild->type = InflatedList;
					if (!currentChild->initialized)
					{
						currentChild->initialized = 1;
						currentChild->children_count = 0;
						currentChild->max_count = tind;
						currentChild->children = malloc(sizeof(mathsolver_inflated_tokens*) * currentChild->max_count);
					}
					for (int j = 0; j < tind; j++)
					{
						mathsolver_inflated_tokens* child = malloc(sizeof(mathsolver_inflated_tokens));
						if (child == NULL) return NULL;
						child->type = Token;
						child->depth = currentChild->depth + 1;
						child->token = currentDepthTokens[j];
						child->parent = currentChild;
						child->initialized = 1;

						if (currentChild->children_count >= currentChild->max_count)
						{
							mathsolver_inflated_tokens** rel = realloc(currentChild->children, sizeof(mathsolver_inflated_tokens*) * currentChild->max_count * 2);
							if (rel != NULL)
							{
								currentChild->children = rel;
								currentChild->max_count = currentChild->max_count * 2;
							}
							else
							{
								return NULL;
							}
						}
						currentChild->children[currentChild->children_count++] = child;
					}
					tind = 0;

					// set current depth to parent
					currentChild = currentChild->parent;
				}
				else
				{
					// illegal closing parenthese without opening pair
					return NULL;
				}
			}
			else
			{
				currentDepthTokens[tind++] = tokens[i];
			}
		}
		if (tind > 0)
		{
			// has leftover tokens in depth
			if (depth > 0)
				return NULL; // invalid syntax: every opening parentheses requires a closing pair

			if (tind == 1 && nTokens == 1)
			{
				root->initialized = 1;
				root->type = Token;
				root->token = currentDepthTokens[0];
			}
			else
			{
				// add current depth tokens to current depth child
				root->type = InflatedList;
				if (!root->initialized)
				{
					root->initialized = 1;
					root->children_count = 0;
					root->max_count = tind;
					root->children = malloc(sizeof(mathsolver_inflated_tokens) * root->max_count);
				}
				for (int j = 0; j < tind; j++)
				{
					mathsolver_inflated_tokens* child = malloc(sizeof(mathsolver_inflated_tokens));
					if (child == NULL) return NULL;
					child->type = Token;
					child->depth = root->depth+1;
					child->token = currentDepthTokens[j];
					child->parent = root;
					child->initialized = 1;

					if (root->children_count >= root->max_count)
					{
						mathsolver_inflated_tokens** rel = realloc(root->children, sizeof(mathsolver_inflated_tokens*) * root->max_count * 2);
						if (rel != NULL)
						{
							root->children = rel;
							root->max_count = root->max_count * 2;
						}
						else
						{
							return NULL;
						}
					}
					if(root->children != NULL)
						root->children[root->children_count++] = child;
				}
				tind = 0;
			}
		}
		free(currentDepthTokens);
	}

	return root;
}

int mathsolver_count_inflated(mathsolver_inflated_tokens* tokens)
{
	if (tokens->type == Token)
	{
		return 1;
	}
	else
	{
		// add number of tokens in children
		int c = tokens->depth > 0 ? 2 : 0; // surround token with parenthesis
		for (int i = 0; i < tokens->children_count; i++)
		{
			c += mathsolver_count_inflated(tokens->children[i]);
		}
		return c;
	}
}

int mathsolver_copy_tokens(mathsolver_inflated_tokens* tokens, mathsolver_token** dest, int index, int limit)
{
	if (tokens->type == Token)
	{
		if (index < limit) {
			dest[index++] = tokens->token;
		}
		return index;
	}
	else
	{
		int ind = index;

		// surround token with parenthesis
		if (tokens->depth > 0)
		{
			if (ind < limit) {
				// get size of token
				int size = sizeof(mathsolver_token);
				mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
				if (token != NULL)
				{
					// create token
					token->type = Operator;
					token->operator= OpeningParentheses;
					token->size = size;
					dest[ind++] = token;
				}
			}
		}

		// copy tokens in children
		for (int i = 0; i < tokens->children_count; i++)
		{
			ind = mathsolver_copy_tokens(tokens->children[i], dest, ind, limit);
		}

		// surround token with parenthesis
		if (tokens->depth > 0)
		{
			if (ind < limit) {
				// get size of token
				int size = sizeof(mathsolver_token);
				mathsolver_token* token = (mathsolver_token*)malloc(size); // allocate
				if (token != NULL)
				{
					// create token
					token->type = Operator;
					token->operator= ClosingParentheses;
					token->size = size;
					dest[ind++] = token;
				}
			}
		}
		return ind;
	}
}

int mathsolver_deflate(mathsolver_inflated_tokens* tokens, mathsolver_token** deflated, int sDeflated)
{
	return mathsolver_copy_tokens(tokens, deflated, 0, sDeflated);
}

mathsolver_expression* mathsolver_to_expression(mathsolver_inflated_tokens* tokens)
{
	mathsolver_expression* expression = malloc(sizeof(mathsolver_expression));
	expression->parent = NULL;
	expression->nodes = NULL;
	expression->copy_of = NULL;
	if (expression == NULL)
		return NULL;

	if (tokens->type == Token)
	{
		switch (tokens->token->type)
		{
		case Number:
			expression->type = expNumber;
			expression->number = atof(tokens->token->number);
			break;
		case Variable:
			expression->type = expVariable;
			expression->variable = tokens->token->variable;
			expression->variable_length = tokens->token->size;
			break;
		}
	}
	else
	{
		int childListSize = 2;
		mathsolver_expression** childList = malloc(sizeof(mathsolver_expression*) * childListSize);
		if (childList == NULL)
			return NULL;
		int childCount = 0;

		mathsolver_expression* currentInst = expression;

		for (int i = 0; i < tokens->children_count; i++)
		{
			// parsing: left to right (3*5+x is mul(3,add(5,x)))
			// Note: since input is expected to be standardized, order of operations is preserved
			if (tokens->children[i]->type == Token && tokens->children[i]->token->type == Operator)
			{
				currentInst->type = expInstruction;
				switch (tokens->children[i]->token->operator)
				{
				case Add:
					currentInst->instruction = iAdd;
					break;
				case Sub:
				{
					// if nothing before sub, then negate
					if (childCount == 0) // since parsing is left to right
						currentInst->instruction = iNeg;
					else
						currentInst->instruction = iSub;
				}
				break;
				case Mul:
					currentInst->instruction = iMul;
					break;
				case Div:
					currentInst->instruction = iDiv;
					break;
				case EQ:
					currentInst->instruction = iEq;
					break;
				case GT:
					currentInst->instruction = iGt;
					break;
				case LT:
					currentInst->instruction = iLt;
					break;
				case GE:
					currentInst->instruction = iGe;
					break;
				case LE:
					currentInst->instruction = iLe;
					break;
				case NEQ:
					currentInst->instruction = iNeq;
					break;
				case Factorial:
					currentInst->instruction = iFactorial;
					break;
				}

				mathsolver_expression* tmp = currentInst;

				currentInst = malloc(sizeof(mathsolver_expression));
				if (currentInst == NULL)
					return NULL;
				currentInst->parent = tmp;
				currentInst->copy_of = NULL;

				if (tmp->instruction != iFactorial)
				{
					if (childCount >= childListSize)
					{
						mathsolver_expression** rel = realloc(childList, sizeof(mathsolver_expression*) * childListSize * 2);
						if (rel != NULL)
						{
							childList = rel;
							childListSize = childListSize * 2;
						}
						else
						{
							return NULL;
						}
					}
					childList[childCount++] = currentInst;
				}

				tmp->node_count = childCount;
				tmp->nodes = childList;

				// reset child list
				childListSize = 2;
				childList = malloc(sizeof(mathsolver_expression*) * childListSize);
				if (childList == NULL)
					return NULL;
				childCount = 0;
			}
			else
			{
				mathsolver_expression* child = mathsolver_to_expression(tokens->children[i]);
				if (childCount >= childListSize)
				{
					mathsolver_expression** rel = realloc(childList, sizeof(mathsolver_expression*) * childListSize * 2);
					if (rel != NULL)
					{
						childList = rel;
						childListSize = childListSize * 2;
					}
					else
					{
						return NULL;
					}
				}
				childList[childCount++] = child;
			}
		}

		if (currentInst->parent != NULL && childCount > 0)
		{
			mathsolver_expression** rel = realloc(currentInst->parent->nodes, sizeof(mathsolver_expression*) * (currentInst->parent->node_count - 1 + childCount));
			if (rel != NULL)
			{
				currentInst->parent->nodes = rel;
				currentInst->parent->node_count = currentInst->parent->node_count - 1 + childCount;
			}
			else
			{
				return NULL;
			}

			// copy leftover children to parent of arg expression
			memcpy(currentInst->parent->nodes + currentInst->parent->node_count - childCount, childList, sizeof(mathsolver_expression*) * childCount);
		}
		else if (childCount == 1)
		{
			// only possible to have root expression with one child
			currentInst->type = childList[0]->type;
			switch (currentInst->type)
			{
			case expNumber:
				currentInst->number = childList[0]->number;
				break;
			case expVariable:
				currentInst->variable = childList[0]->variable;
				currentInst->variable_length = childList[0]->variable_length;
				break;
			case expInstruction:
				currentInst->instruction = childList[0]->instruction;
				currentInst->nodes = childList[0]->nodes;
				currentInst->node_count = childList[0]->node_count;
				break;
			}
		}
	}

	return expression;
}

mathsolver_inflated_tokens* mathsolver_from_expression_parent(mathsolver_expression* expression, mathsolver_inflated_tokens* parent)
{
	mathsolver_inflated_tokens* tokens = malloc(sizeof(mathsolver_inflated_tokens));
	if (tokens == NULL)
		return NULL;
	tokens->parent = parent;
	tokens->initialized = 0;
	tokens->depth = parent != NULL ? parent->depth + 1 : 0;

	if (expression->type != expInstruction)
	{
		tokens->type = Token;
		tokens->initialized = 1;
		mathsolver_token* token = malloc(sizeof(mathsolver_token));
		if (token == NULL)
			return NULL;

		switch (expression->type)
		{
		case expNumber:
		{
			token->type = Number;
			char* buf = malloc(sizeof(char) * 256);
			if (buf == NULL)
				return NULL;
			snprintf(buf, 256, "%f", expression->number);
			int len = (int)strlen(buf);
			char* ptr = buf + len - 1;
			if (strchr(buf, '.') != NULL)
			{
				while (*ptr == '0' || *ptr == '.' /* decimal */)
				{
					char p = *ptr;
					*ptr = '\0';
					ptr--;
					len--;
					if (p == '.')
						break;
				}
			}
			token->number = buf;
			token->size = len;
			break;
		}
		case expVariable:
		{
			token->type = Variable;
			token->size = expression->variable_length;
			token->variable = expression->variable;
			break;
		}
		}

		tokens->token = token;
	}
	else
	{
		tokens->initialized = 1;
		tokens->type = InflatedList;
		tokens->children = malloc(sizeof(mathsolver_inflated_tokens*) * (expression->node_count+1)); // make room for operator token
		if (tokens->children == NULL)
			return NULL;
		tokens->children_count = expression->node_count+1;
		tokens->max_count = expression->node_count+1;

		for (int i = 0; i < expression->node_count; i++)
		{
			if (i == 0 && expression->instruction != iNeg)
			{
				mathsolver_inflated_tokens* child = mathsolver_from_expression_parent(expression->nodes[0], tokens);
				tokens->children[0] = child;

				mathsolver_inflated_tokens* childOp = malloc(sizeof(mathsolver_inflated_tokens));
				if (childOp == NULL)
					return NULL;
				childOp->parent = child->parent;
				childOp->initialized = 1;
				childOp->depth = child->depth;
				childOp->type = Token;

				mathsolver_token* token = malloc(sizeof(mathsolver_token));
				if (token == NULL)
					return NULL;
				token->type = Operator;
				switch (expression->instruction)
				{
				case iAdd:
					token->operator = Add;
					break;
				case iSub:
				case iNeg:
					token->operator = Sub;
					break;
				case iMul:
					token->operator = Mul;
					break;
				case iDiv:
					token->operator = Div;
					break;
				case iEq:
					token->operator = EQ;
					break;
				case iGt:
					token->operator = GT;
					break;
				case iLt:
					token->operator = LT;
					break;
				case iGe:
					token->operator = GE;
					break;
				case iLe:
					token->operator = LE;
					break;
				case iNeq:
					token->operator = NEQ;
					break;
				case iFactorial:
					token->operator = Factorial;
					break;
				}
				childOp->token = token;
				tokens->children[1] = childOp;
			}
			else if (i == 0)
			{
				mathsolver_inflated_tokens* child = mathsolver_from_expression_parent(expression->nodes[0], tokens);
				tokens->children[1] = child;

				mathsolver_inflated_tokens* childOp = malloc(sizeof(mathsolver_inflated_tokens));
				if (childOp == NULL)
					return NULL;
				childOp->parent = child->parent;
				childOp->initialized = 1;
				childOp->depth = child->depth;
				childOp->type = Token;

				mathsolver_token* token = malloc(sizeof(mathsolver_token));
				if (token == NULL)
					return NULL;
				token->type = Operator;
				switch (expression->instruction)
				{
				case iNeg:
					token->operator = Sub;
					break;
				}
				childOp->token = token;
				tokens->children[0] = childOp;
			}
			else
			{
				mathsolver_inflated_tokens* child = mathsolver_from_expression_parent(expression->nodes[i], tokens);
				child->parent = tokens;
				tokens->children[i + 1] = child;
			}
		}
	}

	return tokens;
}

mathsolver_inflated_tokens* mathsolver_from_expression(mathsolver_expression* expression)
{
	return mathsolver_from_expression_parent(expression, NULL);
}

mathsolver_expression* mathsolver_evaluate(mathsolver_expression* expression)
{
	if (expression->type == expNumber)
	{
		return expression;
	}
	else if (expression->type == expVariable)
	{
		// convert var into new number expression if defined
		mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
		if (exp == NULL)
			return expression;

		exp->type = expNumber;
		exp->parent = expression->parent;
		exp->number = 12.34;
		exp->copy_of = expression;
		return exp;
	}
	else
	{
		// evaluate instruction
		switch (expression->instruction)
		{
		case iAdd:
		{
			if (expression->node_count == 2)
			{
				mathsolver_expression* left = mathsolver_evaluate(expression->nodes[0]);
				mathsolver_expression* right = mathsolver_evaluate(expression->nodes[1]);

				if (left->type == expNumber && right->type == expNumber)
				{
					mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
					if (exp == NULL)
						return expression;
					exp->type = expNumber;
					exp->parent = expression->parent;
					exp->number = left->number + right->number;
					exp->copy_of = expression;

					if (left->copy_of != NULL) // prevent memory leaks for copied expressions
						mathsolver_expression_free(&left);
					if (right->copy_of != NULL)
						mathsolver_expression_free(&right);

					return exp;
				}
			}
			break;
		}
		case iSub:
		{
			if (expression->node_count == 2)
			{
				mathsolver_expression* left = mathsolver_evaluate(expression->nodes[0]);
				mathsolver_expression* right = mathsolver_evaluate(expression->nodes[1]);

				if (left->type == expNumber && right->type == expNumber)
				{
					mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
					if (exp == NULL)
						return expression;
					exp->type = expNumber;
					exp->parent = expression->parent;
					exp->number = left->number - right->number;
					exp->copy_of = expression;

					if (left->copy_of != NULL) // prevent memory leaks for copied expressions
						mathsolver_expression_free(&left);
					if (right->copy_of != NULL)
						mathsolver_expression_free(&right);

					return exp;
				}
			}
			break;
		}
		case iNeg:
		{
			if (expression->node_count == 1)
			{
				mathsolver_expression* left = mathsolver_evaluate(expression->nodes[0]);

				if (left->type == expNumber)
				{
					mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
					if (exp == NULL)
						return expression;
					exp->type = expNumber;
					exp->parent = expression->parent;
					exp->number = -left->number;
					exp->copy_of = expression;

					if (left->copy_of != NULL) // prevent memory leaks for copied expressions
						mathsolver_expression_free(&left);

					return exp;
				}
			}
			break;
		}
		case iMul:
		{
			if (expression->node_count == 2)
			{
				mathsolver_expression* left = mathsolver_evaluate(expression->nodes[0]);
				mathsolver_expression* right = mathsolver_evaluate(expression->nodes[1]);

				if (left->type == expNumber && right->type == expNumber)
				{
					mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
					if (exp == NULL)
						return expression;
					exp->type = expNumber;
					exp->parent = expression->parent;
					exp->number = left->number * right->number;
					exp->copy_of = expression;

					if (left->copy_of != NULL) // prevent memory leaks for copied expressions
						mathsolver_expression_free(&left);
					if (right->copy_of != NULL)
						mathsolver_expression_free(&right);

					return exp;
				}
			}
			break;
		}
		case iDiv:
		{
			if (expression->node_count == 2)
			{
				mathsolver_expression* left = mathsolver_evaluate(expression->nodes[0]);
				mathsolver_expression* right = mathsolver_evaluate(expression->nodes[1]);

				if (left->type == expNumber && right->type == expNumber)
				{
					mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
					if (exp == NULL)
						return expression;
					exp->type = expNumber;
					exp->parent = expression->parent;
					exp->number = left->number / right->number;
					exp->copy_of = expression;

					if (left->copy_of != NULL) // prevent memory leaks for copied expressions
						mathsolver_expression_free(&left);
					if (right->copy_of != NULL)
						mathsolver_expression_free(&right);

					return exp;
				}
			}
			break;
		}
		case iFactorial:
		{
			if (expression->node_count == 1)
			{
				mathsolver_expression* left = mathsolver_evaluate(expression->nodes[0]);

				if (left->type == expNumber)
				{
					if (!is_integer(left->number) || left->number < 0) // factorial requires whole integers
						return expression;

					double out = 1;
					for (double v = 1; v <= left->number; v++)
					{
						out *= v;
					}

					mathsolver_expression* exp = malloc(sizeof(mathsolver_expression));
					if (exp == NULL)
						return expression;
					exp->type = expNumber;
					exp->parent = expression->parent;
					exp->number = out;
					exp->copy_of = expression;

					if (left->copy_of != NULL) // prevent memory leaks for copied expressions
						mathsolver_expression_free(&left);

					return exp;
				}
			}
			break;
		}
		}
		return expression;
	}
}