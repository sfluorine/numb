#pragma once

#include "ast.h"
#include "compiler.h"
#include "vm.h"

class BytecodeGenerator : public AstVisitor {
public:
    BytecodeGenerator(Compiler& compiler, NumbVm& vm)
        : compiler(compiler), vm(vm)
        , bytecode(vm.bytecode)
    {
    }

    OVERLOAD_VISIT();

private:
    void write_qword(int64_t);

    void write_dword(int32_t);

private:
    Compiler& compiler;
    NumbVm& vm;
    std::vector<uint8_t>& bytecode;
};
