#include "mathsolver.h"

int main()
{
    mathsolver_token *tokens[16];
    int count = mathsolver_parse("1>=3>=9", &tokens);
    if (
        count == 5 &&
        tokens[1]->type == Operator &&
        tokens[1]->operator== GE &&
        tokens[3]
            ->type == Operator &&
        tokens[3]
            ->
            operator== GE)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}