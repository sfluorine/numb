#include <iostream>

#include "parser.h"

int main(int argc, char** argv)
{
    Parser parser("let x = false * true");
    auto statement = parser.parse_statement().value();

    return 0;
}
