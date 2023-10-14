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

void mathsolver_token_free(mathsolver_token** token);

uint8_t is_numeric(char ch, char hintPrev, char hintNext);
uint8_t is_alphabetic(char ch);
uint8_t is_whitespace(char ch);

int mathsolver_parse(char *str, mathsolver_token **tokens);
int mathsolver_format(char* output, int sOutput, mathsolver_token** tokens, int nTokens);

int mathsolver_standardize(mathsolver_token** tokens, int nTokens, int limitTokens);

#endif