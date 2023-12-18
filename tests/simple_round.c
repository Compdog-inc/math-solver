#include "mathsolver.h"

int main()
{
    mathsolver_expression* expression = mathsolver_parsed_expression("3x*(2x+(2y-3)+5)x(2(z/2)z)3x", 64);

    char buf[256];
    mathsolver_format_expression(buf, 256, expression, 64);

    mathsolver_expression_free(&expression);

    if (strcmp(buf, "3x(2x+((2y-3)+5))*x(2z/2)*z*3x") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}