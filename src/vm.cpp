#include "vm.h"
#include <iostream>

constexpr auto STACK_CAPACITY = 2048;

NumbVm::NumbVm()
{
    stack.reserve(STACK_CAPACITY);
}

void NumbVm::execute()
{
    if (bytecode.size() > 0) {
        m_halt = false;
    }

    while (!m_halt) {
        catch_trap(eval());
    }
}

void NumbVm::dump_stack()
{
    std::cout << "STACK:\n";
    for (auto i = 0; i <= sp; i++) {
        switch (stack[i].type()) {
        case ValueType::Empty: {
            std::cout << "EMPTY\n";
            break;
        }
        case ValueType::I64: {
            std::cout << stack[i].as_i64() << '\n';
            break;
        }
        case ValueType::F64: {
            std::cout << stack[i].as_f64() << '\n';
            break;
        }
        case ValueType::Bool: {
            std::cout << stack[i].as_bool() << '\n';
            break;
        }
        case ValueType::Object: {
            std::cout << stack[i].as_object() << '\n';
            break;
        }
        }
    }
}

NumbTrap NumbVm::eval()
{
    switch (BYTE_TO_INSTRUCTION(fetch())) {
    case Instruction::Halt: {
        m_halt = true;
        break;
    }
    case Instruction::I64Const: {
        int64_t index = read_qword();

        auto constant = i64_pool.get(index);
        if (!constant) {
            return NumbTrap::InvalidOperand;
        }

        return push(*constant);
    }
    case Instruction::F64Const: {
        int64_t index = read_qword();

        auto constant = f64_pool.get(index);
        if (!constant) {
            return NumbTrap::InvalidOperand;
        }

        return push(*constant);
    }
    case Instruction::BoolConst: {
        int64_t index = read_qword();

        auto constant = bool_pool.get(index);
        if (!constant) {
            return NumbTrap::InvalidOperand;
        }

        return push(*constant);
    }
    case Instruction::Pop: {
        return pop();
    }
    case Instruction::I64Add: {
        if (sp < 1) {
            return NumbTrap::StackUnderflow;
        }

        stack[sp - 1].as_i64() += stack[sp].as_i64();
        return pop();
    }
    case Instruction::I64Sub: {
        if (sp < 1) {
            return NumbTrap::StackUnderflow;
        }

        stack[sp - 1].as_i64() -= stack[sp].as_i64();
        return pop();
    }
    case Instruction::I64Mul: {
        if (sp < 1) {
            return NumbTrap::StackUnderflow;
        }

        stack[sp - 1].as_i64() *= stack[sp].as_i64();
        return pop();
    }
    case Instruction::I64Div: {
        if (sp < 1) {
            return NumbTrap::StackUnderflow;
        }

        stack[sp - 1].as_i64() /= stack[sp].as_i64();
        return pop();
    }
    case Instruction::I64Minus: {
        if (sp < 0) {
            return NumbTrap::StackUnderflow;
        }

        stack[sp].as_i64() *= -1;
        break;
    }
    case Instruction::GetLocal: {
        int64_t index = read_qword();
        return push(stack[index]);
    }
    default:
        return NumbTrap::InvalidInstruction;
    }

    return NumbTrap::OK;
}

// TODO: make a helper function for emitting trap error for better error message.
void NumbVm::catch_trap(NumbTrap trap)
{
    switch (trap) {
    case NumbTrap::StackOverflow: {
        std::cerr << "STACK OVERFLOW\n";
        m_halt = true;
        break;
    }
    case NumbTrap::StackUnderflow: {
        std::cerr << "STACK UNDERFLOW\n";
        m_halt = true;
        break;
    }
    case NumbTrap::InvalidInstruction: {
        std::cerr << "INVALID INSTRUCTION\n";
        m_halt = true;
        break;
    }
    case NumbTrap::InvalidOperand: {
        std::cerr << "INVALID OPERAND\n";
        m_halt = true;
        break;
    }
    default:
        break;
    }
}

NumbTrap NumbVm::push(Value value)
{
    if (sp >= STACK_CAPACITY) {
        return NumbTrap::StackOverflow;
    }

    stack[++sp] = std::move(value);
    return NumbTrap::OK;
}

NumbTrap NumbVm::pop()
{
    if (sp < 0) {
        return NumbTrap::StackUnderflow;
    }

    sp--;
    return NumbTrap::OK;
}

uint8_t NumbVm::fetch()
{
    return bytecode[++pc];
}

int64_t NumbVm::read_qword()
{
    int64_t qword = 0;

    for (size_t i = 0; i < sizeof(qword); i++) {
        qword |= static_cast<int64_t>(fetch()) << (i * 8);
    }

    return qword;
}

int32_t NumbVm::read_dword()
{
    int32_t dword = 0;

    for (size_t i = 0; i < sizeof(dword); i++) {
        dword |= static_cast<int32_t>(fetch()) << (i * 8);
    }

    return dword;
}
