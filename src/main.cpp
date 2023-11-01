#include <iostream>

#include "codegen.h"
#include "parser.h"
#include "typechecker.h"
#include "vm.h"

int main(int argc, char** argv)
{
    Compiler compiler;
    Parser parser(compiler, "{ let x = -34; let y = 35; let nice = x + y; { let haha = 69420; } }");

    compiler.begin_scope();
    auto opt_block = parser.parse_block_stmt();
    compiler.end_scope();

    if (parser.has_errors()) {
        for (auto const& error : parser.errors()) {
            std::cerr << error << '\n';
        }

        return 1;
    }

    auto block = opt_block.value();
    
    Typechecker typechecker(compiler);
    block->accept(typechecker);
    
    if (!block->done) {
        return 1;
    }
    
    NumbVm vm;
    
    BytecodeGenerator generator(compiler, vm);
    block->accept(generator);

    vm.bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::Halt));

    vm.execute();
    vm.dump_stack();
}
