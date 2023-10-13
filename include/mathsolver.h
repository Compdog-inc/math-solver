#ifndef _MATHSOLVER_H_
#define _MATHSOLVER_H_

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

__uint8_t is_numeric(char ch, char hintPrev, char hintNext);
__uint8_t is_alphabetic(char ch);
__uint8_t is_whitespace(char ch);
int mathsolver_parse(char *str, mathsolver_token **tokens);

#endif