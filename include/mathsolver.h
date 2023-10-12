#include <malloc.h>

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
	ClosingParentheses
} mathsolver_operator;

typedef struct
{
	mathsolver_token_type type;
	union 
	{
		char* number;
		char* variable;
		mathsolver_operator operator;
	};
	int size;
} mathsolver_token;

int mathsolver_parse(char* str, mathsolver_token** tokens);