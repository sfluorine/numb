#include "codegen.h"

#include <assert.h>
#include <iostream>

void BytecodeGenerator::visit(ExprInt* expr)
{
    auto index = vm.i64_pool.put(expr->parsed_int);
    bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Const));
    write_qword(index);
}

void BytecodeGenerator::visit(ExprIdentifier* expr)
{
    if (compiler.find_local(expr->span) == -1) {
        assert(false && "global variable is not supported yet.");
    }

    auto index = vm.locals[expr->span];
    bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::GetLocal));
    write_qword(index);
}

void BytecodeGenerator::visit(ExprBool* expr)
{
    auto index = vm.bool_pool.put(expr->parsed_bool);
    bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::BoolConst));
    write_qword(index);
}

void BytecodeGenerator::visit(ExprUnary* expr)
{
    switch (expr->op.type) {
    case Token::Type::Minus: {
        expr->expr->accept(*this);
        bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Minus));
        break;
    }
    case Token::Type::Bang: {
        expr->expr->accept(*this);
        bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::BoolNot));
        break;
    }
    }
}

void BytecodeGenerator::visit(ExprBinary* expr)
{
    expr->lhs->accept(*this);
    expr->rhs->accept(*this);

    switch (expr->op.type) {
    case Token::Type::Plus:
        switch (expr->result_type) {
        case NumbType::Int:
            bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Add));
            break;
        };
        break;
    case Token::Type::Minus:
        switch (expr->result_type) {
        case NumbType::Int:
            bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Sub));
            break;
        };
        break;
    case Token::Type::Star:
        switch (expr->result_type) {
        case NumbType::Int:
            bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Mul));
            break;
        };
        break;
    case Token::Type::Slash:
        switch (expr->result_type) {
        case NumbType::Int:
            bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::I64Div));
            break;
        };
        break;
    }
}

void BytecodeGenerator::visit(StmtLet* stmt)
{
    // TODO: support global variable declarations.
    if (compiler.find_local(stmt->name) == -1) {
        assert(false && "global variable is not supported yet.");
    }

    stmt->expr->accept(*this);
    vm.locals[stmt->name] = compiler.find_local(stmt->name);
}

void BytecodeGenerator::visit(StmtBlock* stmt)
{
    compiler.begin_scope();
    for (auto& stmt : stmt->statements) {
        stmt->accept(*this);
    }
    compiler.end_scope();

    while (compiler.locals.size() > 0 && compiler.locals[compiler.locals.size() - 1].depth > compiler.scope_depth + 1) {
        bytecode.push_back(INSTRUCTION_TO_BYTE(Instruction::Pop));
        compiler.locals.pop_back();
    }
}

void BytecodeGenerator::write_qword(int64_t qword)
{
    for (size_t i = 0; i < sizeof(qword); i++) {
        uint8_t byte = static_cast<uint8_t>((qword >> (i * 8)) & 0xFF);
        bytecode.push_back(byte);
    }
}

void BytecodeGenerator::write_dword(int32_t dword)
{
    for (size_t i = 0; i < sizeof(dword); i++) {
        uint8_t byte = static_cast<uint8_t>((dword >> (i * 8)) & 0xFF);
        bytecode.push_back(byte);
    }
}
