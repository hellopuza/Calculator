/*------------------------------------------------------------------------------
    * File:        main.cpp                                                    *
    * Description: Program for calculating math expressions.                   *
    * Created:     15 may 2021                                                 *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#include "Calculator/Calculator.h"

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        Calculator calc;

        return calc.Run();
    }
    else
    {
        Calculator calc(argv[1]);

        return calc.Run();
    }

    return 0;
}
