#include "codegen.h"

void BytecodeGenerator::visit(ExprInt* expr)
{
    auto index = vm.i64_pool.put(expr->parsed_int);
    bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Const));
    write_qword(index);
}

void BytecodeGenerator::visit(ExprBool*)
{
}

void BytecodeGenerator::visit(ExprUnary*)
{
}

void BytecodeGenerator::visit(ExprBinary* binary)
{
    binary->lhs->accept(*this);
    binary->rhs->accept(*this);

    switch (binary->op.type) {
    case Token::Type::Plus:
        switch (binary->result_type) {
            case NumbType::Int:
                bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Add));
                break;
        };
        break;
    case Token::Type::Minus:
        switch (binary->result_type) {
            case NumbType::Int:
                bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Sub));
                break;
        };
        break;
    case Token::Type::Star:
        switch (binary->result_type) {
            case NumbType::Int:
                bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Mul));
                break;
        };
        break;
    case Token::Type::Slash:
        switch (binary->result_type) {
            case NumbType::Int:
                bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Div));
                break;
        };
        break;
    }
}

void BytecodeGenerator::visit(StmtLet*)
{
}

void BytecodeGenerator::write_qword(int64_t qword)
{
    for (int i = 0; i < sizeof(qword); i++) {
        uint8_t byte = static_cast<uint8_t>((qword >> (i * 8)) & 0xFF);
        bytecode.push_back(byte);
    }
}

void BytecodeGenerator::write_dword(int32_t dword)
{
    for (int i = 0; i < sizeof(dword); i++) {
        uint8_t byte = static_cast<uint8_t>((dword >> (i * 8)) & 0xFF);
        bytecode.push_back(byte);
    }
}
