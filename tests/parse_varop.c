#include "mathsolver.h"
#include "test_utils.h"

int main()
{
    mathsolver_token *tokens[20];
    int count = mathsolver_parse("x>=y<=z!=a=b!+h(a)", tokens);
    if (
        count == 15 &&
        token_isvar(tokens[0], "x") &&
        token_isge(tokens[1]) &&
        token_isvar(tokens[2], "y") &&
        token_isle(tokens[3]) &&
        token_isvar(tokens[4], "z") &&
        token_isneq(tokens[5]) &&
        token_isvar(tokens[6], "a") &&
        token_iseq(tokens[7]) &&
        token_isvar(tokens[8], "b") &&
        token_isfac(tokens[9]) &&
        token_isadd(tokens[10]) &&
        token_isvar(tokens[11], "h") &&
        token_isopar(tokens[12]) &&
        token_isvar(tokens[13], "a") &&
        token_iscpar(tokens[14])
        )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}