#ifndef _MATHSOLVER_H_
#define _MATHSOLVER_H_

#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	Factorial,
	Exponent
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
	iFactorial,
	iExponent
} mathsolver_instruction;

typedef struct _mathsolver_expression
{
	mathsolver_expression_type type;
	union
	{
		double number;
		struct
		{
			char* variable;
			int variable_length;
			uint8_t indexed;
		};
		struct
		{
			struct _mathsolver_expression** nodes;
			int node_count;
			mathsolver_instruction instruction;
		};
	};
	struct _mathsolver_expression* parent;
	struct _mathsolver_expression* copy_of;
	uint8_t flags;
} mathsolver_expression;

void mathsolver_token_free(mathsolver_token** token);
// Note: Does not free the underlying mathsolver_token* pointer or parent, only the children arrays and children inflated tokens + struct pointers
void mathsolver_inflated_tokens_free(mathsolver_inflated_tokens** tokens);
void mathsolver_expression_free(mathsolver_expression** expression);

uint8_t is_numeric(char ch, char hintPrev, char hintNext);
uint8_t is_alphabetic(char ch);
uint8_t is_whitespace(char ch);
uint8_t is_integer(double value);

int mathsolver_parse(char *str, mathsolver_token **tokens, int nTokens);
int mathsolver_format(char* output, int sOutput, mathsolver_token** tokens, int nTokens);
int mathsolver_format_expression(char* output, int sOutput, mathsolver_expression* expression, int maxTokens);

#define MATHSOLVER_STANDARDIZE_FLAG_NONE 0
#define MATHSOLVER_STANDARDIZE_FLAG_IMPLICIT_ORDER_OF_OPERATIONS 1<<0
int mathsolver_standardize(mathsolver_token** tokens, int nTokens, int limitTokens, uint8_t flags);
int mathsolver_reduce(mathsolver_token** tokens, int nTokens);

mathsolver_inflated_tokens* mathsolver_inflate(mathsolver_token** tokens, int nTokens);
int mathsolver_count_inflated(mathsolver_inflated_tokens* tokens);
int mathsolver_deflate(mathsolver_inflated_tokens* tokens, mathsolver_token** deflated, int sDeflated);

// Create expression with empty parent, type number and value [number]
mathsolver_expression* mathsolver_number_expression(double number);
// Parse, standardize, inflate, and get expression from input string (util for combining all other functions)
mathsolver_expression* mathsolver_parsed_expression(char* str, int maxTokens);

#define MATHSOLVER_EXPRESSION_FLAG_NONE (0)
#define MATHSOLVER_EXPRESSION_FLAG_DO_NOT_REFERENCE_TOKEN (1<<0)
// Note: requires standardized tokens
mathsolver_expression* mathsolver_to_expression(mathsolver_inflated_tokens* tokens, uint8_t flags);
mathsolver_inflated_tokens* mathsolver_from_expression(mathsolver_expression* expression);

typedef struct
{
	union
	{
		struct {
			char* name;
			mathsolver_expression* simple_value;
		};
		mathsolver_expression* indexed_value;
	};
} mathsolver_variable;

void mathsolver_push_variable_table(mathsolver_expression* expression, char** variables, int nVariables);
void mathsolver_pop_variable_table(mathsolver_expression* expression, char** variables, int nVariables);

mathsolver_expression* mathsolver_evaluate(mathsolver_expression* expression, mathsolver_variable* variables, int nVariables);
mathsolver_expression* mathsolver_convert_instruction(mathsolver_expression* expression, mathsolver_instruction toInstruction);
mathsolver_expression* mathsolver_to_shallow(mathsolver_expression* expression);
mathsolver_expression* mathsolver_to_deep(mathsolver_expression* expression);
mathsolver_expression* mathsolver_simplify(mathsolver_expression* expression);

#endif