#include "mathsolver.h"
#include <memory.h>
#include "test_utils.h"

int main()
{
	/*
	|___3
	|___x
	|___*
	|   |___2
	|   |___x
	|   |___+
	|   |   |___2
	|   |   |___y
	|   |   |___-
	|   |   |___3
	|   |___+
	|   |___5
	|___x
	|   |___2
	|   |   |___z
	|   |   |___/
	|   |   |___2
	|   |___z
	|___3
	|___x
	*/
	mathsolver_token* tokens[32];
	int count = mathsolver_parse("3x*(2x+(2y-3)+5)x(2(z/2)z)3x", tokens, 32);
	mathsolver_inflated_tokens* inflated = mathsolver_inflate(tokens, count);

	int result;
	if (
		inflated->type == InflatedList && inflated->children_count == 8 &&
		inflated->children[0]->type == Token && token_isnum(inflated->children[0]->token, 3) &&
		inflated->children[1]->type == Token && token_isvar(inflated->children[1]->token, "x") &&
		inflated->children[2]->type == Token && token_ismul(inflated->children[2]->token) &&
		inflated->children[4]->type == Token && token_isvar(inflated->children[4]->token, "x") &&
		inflated->children[6]->type == Token && token_isnum(inflated->children[6]->token, 3) &&
		inflated->children[7]->type == Token && token_isvar(inflated->children[7]->token, "x") &&
		inflated->children[3]->type == InflatedList && inflated->children[3]->children_count == 6 &&
		inflated->children[3]->children[0]->type == Token && token_isnum(inflated->children[3]->children[0]->token, 2) &&
		inflated->children[3]->children[1]->type == Token && token_isvar(inflated->children[3]->children[1]->token, "x") &&
		inflated->children[3]->children[2]->type == Token && token_isadd(inflated->children[3]->children[2]->token) &&
		inflated->children[3]->children[4]->type == Token && token_isadd(inflated->children[3]->children[4]->token) &&
		inflated->children[3]->children[5]->type == Token && token_isnum(inflated->children[3]->children[5]->token, 5) &&
		inflated->children[3]->children[3]->type == InflatedList && inflated->children[3]->children[3]->children_count == 4 &&
		inflated->children[3]->children[3]->children[0]->type == Token && token_isnum(inflated->children[3]->children[3]->children[0]->token, 2) &&
		inflated->children[3]->children[3]->children[1]->type == Token && token_isvar(inflated->children[3]->children[3]->children[1]->token, "y") &&
		inflated->children[3]->children[3]->children[2]->type == Token && token_issub(inflated->children[3]->children[3]->children[2]->token) &&
		inflated->children[3]->children[3]->children[3]->type == Token && token_isnum(inflated->children[3]->children[3]->children[3]->token, 3) &&
		inflated->children[5]->type == InflatedList && inflated->children[5]->children_count == 3 &&
		inflated->children[5]->children[0]->type == Token && token_isnum(inflated->children[5]->children[0]->token, 2) &&
		inflated->children[5]->children[2]->type == Token && token_isvar(inflated->children[5]->children[2]->token, "z") &&
		inflated->children[5]->children[1]->type == InflatedList && inflated->children[5]->children[1]->children_count == 3 &&
		inflated->children[5]->children[1]->children[0]->type == Token && token_isvar(inflated->children[5]->children[1]->children[0]->token, "z") &&
		inflated->children[5]->children[1]->children[1]->type == Token && token_isdiv(inflated->children[5]->children[1]->children[1]->token) &&
		inflated->children[5]->children[1]->children[2]->type == Token && token_isnum(inflated->children[5]->children[1]->children[2]->token, 2)
		)
	{
		result = 0;
	}
	else
	{
		result = 1;
	}

	mathsolver_inflated_tokens_free(&inflated);
	return result;
}