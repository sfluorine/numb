#pragma once

#include "ast.h"
#include "vm.h"

class BytecodeGenerator : public AstVisitor {
public:
    BytecodeGenerator(NumbVm& vm)
        : vm(vm)
        , bytecode(vm.bytecode)
    {
    }

    OVERLOAD_VISIT();

private:
    void write_qword(int64_t);

    void write_dword(int32_t);

private:
    NumbVm& vm;
    std::vector<uint8_t>& bytecode;
};
