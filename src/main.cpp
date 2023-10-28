#include <iostream>

#include "parser.h"
#include "typechecker.h"

int main(int argc, char** argv)
{
    Parser parser("let __x = !(false == true);");
    auto opt_stmt = parser.parse_statement();

    if (parser.has_errors()) {
        for (auto const& error : parser.errors()) {
            std::cerr << error << '\n';
        }

        return 1;
    }

    auto stmt = opt_stmt.value();

    Typechecker typechecker;
    stmt->accept(typechecker);

    std::cout << stmt->done << '\n';
    return 0;
}
