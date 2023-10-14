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

void mathsolver_addtoken(mathsolver_token_type* currentTokenType, char** str, char** currentValueStart, mathsolver_token*** tokens, int* tokenCount, int nTokens)
{
	if ((*tokenCount) < nTokens - 1) {
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
					currentChild->max_count = tind;
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
					tmp->children[tmp->children_count++] = currentChild;
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

mathsolver_expression* mathsolver_to_expression(mathsolver_token** tokens, int nTokens)
{
	for (int i = 0; i < nTokens; i++)
	{
		if (tokens[i]->type == Operator)
		{

		}
	}
}

int mathsolver_from_expression(mathsolver_expression* expression, mathsolver_token** tokens, int nTokens)
{

}