#ifndef _MATHSOLVER_H_
#define _MATHSOLVER_H_

#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include <memory.h>

typedef enum
{
	Number,
	Variable,
	Operator
} mathsolver_token_type;

typedef enum
{
	Add,
	Sub,
	Mul,
	Div,
	EQ,
	GT,
	LT,
	GE,
	LE,
	NEQ,
	OpeningParentheses,
	ClosingParentheses,
	Factorial
} mathsolver_operator;

typedef struct
{
	mathsolver_token_type type;
	union
	{
		char *number;
		char *variable;
		mathsolver_operator operator;
	};
	int size;
} mathsolver_token;

typedef enum
{
	Token,
	InflatedList
} mathsolver_inflated_token_type;

typedef struct _mathsolver_inflated_tokens
{
	mathsolver_inflated_token_type type;
	union
	{
		mathsolver_token* token;
		struct
		{
			struct _mathsolver_inflated_tokens** children;
			int children_count;
			int max_count;
		};
	};
	uint8_t initialized;
	int depth;
	struct _mathsolver_inflated_tokens* parent;
} mathsolver_inflated_tokens;

typedef enum
{
	expInstruction,
	expNumber,
	expVariable
} mathsolver_expression_type;

typedef enum
{
	iAdd,
	iNeg,
	iSub,
	iMul,
	iDiv,
	iEq,
	iGt,
	iLt,
	iGe,
	iLe,
	iNeq,
	iFactorial
} mathsolver_instruction;

typedef struct _mathsolver_expression
{
	mathsolver_expression_type type;
	union
	{
		double number;
		char* variable;
		struct
		{
			struct _mathsolver_expression** nodes;
			int node_count;
			mathsolver_instruction instruction;
		};
	};
} mathsolver_expression;

void mathsolver_token_free(mathsolver_token** token);
// Note: Does not free the underlying mathsolver_token* pointer or parent, only the children arrays and children inflated tokens + struct pointers
void mathsolver_inflated_tokens_free(mathsolver_inflated_tokens** tokens);

uint8_t is_numeric(char ch, char hintPrev, char hintNext);
uint8_t is_alphabetic(char ch);
uint8_t is_whitespace(char ch);

int mathsolver_parse(char *str, mathsolver_token **tokens, int nTokens);
int mathsolver_format(char* output, int sOutput, mathsolver_token** tokens, int nTokens);

int mathsolver_standardize(mathsolver_token** tokens, int nTokens, int limitTokens);
int mathsolver_reduce(mathsolver_token** tokens, int nTokens);

mathsolver_inflated_tokens* mathsolver_inflate(mathsolver_token** tokens, int nTokens);
int mathsolver_count_inflated(mathsolver_inflated_tokens* tokens);
int mathsolver_deflate(mathsolver_inflated_tokens* tokens, mathsolver_token** deflated, int sDeflated);

// Note: requires standardized tokens
mathsolver_expression* mathsolver_to_expression(mathsolver_token** tokens, int nTokens);
int mathsolver_from_expression(mathsolver_expression* expression, mathsolver_token** tokens, int nTokens);

#endif