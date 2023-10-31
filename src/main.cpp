#include <iostream>

#include "codegen.h"
#include "parser.h"
#include "typechecker.h"
#include "vm.h"

int main(int argc, char** argv)
{
    Parser parser("69420 + 36");
    auto opt_expr = parser.parse_expression();

    if (parser.has_errors()) {
        for (auto const& error : parser.errors()) {
            std::cerr << error << '\n';
        }

        return 1;
    }

    auto expr = opt_expr.value();

    Typechecker typechecker;
    expr->accept(typechecker);

    if (!expr->done) {
        return 1;
    }

    NumbVm vm;
    BytecodeGenerator generator(vm);
    expr->accept(generator);

    std::cout << "written: " << vm.bytecode.size() << " bytes\n";
}
