#include "mathsolver.h"

int main()
{
    mathsolver_token *tokens[16];
    int count = mathsolver_parse("2<=6<=3", tokens, 16);
    if (
        count == 5 &&
        tokens[1]->type == Operator &&
        tokens[1]->operator== LE &&
        tokens[3]
            ->type == Operator &&
        tokens[3]
            ->
            operator== LE)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}