#pragma once

#include "object.h"

#include <stdint.h>

#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>

enum class ValueType {
    Empty,
    I64,
    F64,
    Bool,
    Object,
};

class Value {
public:
    struct Empty { };

    Value()
        : m_type(ValueType::Empty)
        , m_value(Empty {})
    {
    }

    Value(int64_t i64)
        : m_type(ValueType::I64)
        , m_value(i64)
    {
    }

    Value(double f64)
        : m_type(ValueType::F64)
        , m_value(f64)
    {
    }

    Value(bool boolean)
        : m_type(ValueType::Bool)
        , m_value(boolean)
    {
    }

    Value(GcObject* object)
        : m_type(ValueType::Object)
        , m_value(object)
    {
    }

    ValueType type() const { return m_type; }

    int64_t& as_i64() { return std::get<int64_t>(m_value); }

    double& as_f64() { return std::get<double>(m_value); }

    bool& as_bool() { return std::get<bool>(m_value); }

    GcObject*& as_object() { return std::get<GcObject*>(m_value); }

private:
    ValueType m_type;
    std::variant<Empty, int64_t, double, bool, GcObject*> m_value;
};

template<typename T>
class Pool {
public:
    Pool() { }

    template<typename... Args>
    int64_t put(Args&&... args)
    {
        m_data.push_back(T(std::forward<Args>(args)...));
        return m_index++;
    }

    std::optional<T> get(int64_t index)
    {
        if (index >= m_index) {
            return {};
        }

        return m_data[index];
    }

private:
    int64_t m_index { 0 };
    std::vector<T> m_data;
};

enum class Instruction : uint8_t {
    Halt,

    I64Const,
    F64Const,
    BoolConst,

    Pop,

    I64Add,
    I64Sub,
    I64Mul,
    I64Div,

    I64Minus,
    BoolNot,

    GetLocal,
    SetLocal,
};

#define INSTRUCTION_TO_BYTE(ins) (static_cast<uint8_t>(ins))
#define BYTE_TO_INSTRUCTION(byte) (static_cast<Instruction>(byte))

enum class NumbTrap {
    StackOverflow,
    StackUnderflow,

    InvalidInstruction,
    InvalidOperand,

    OK,
};

class NumbVm {
public:
    NumbVm();

    void execute();

    void dump_stack();

private:
    NumbTrap eval();

    NumbTrap push(Value value);

    NumbTrap pop();

    void catch_trap(NumbTrap);

    uint8_t fetch();

    int64_t read_qword();

    int32_t read_dword();

public:
    int64_t pc { -1 }; // program counter
    int64_t sp { -1 }; // stack pointer

    std::vector<uint8_t> bytecode;
    std::unordered_map<std::string_view, int64_t> locals;
    std::vector<Value> stack;

    Pool<int64_t> i64_pool;
    Pool<double> f64_pool;
    Pool<bool> bool_pool;

private:
    bool m_halt { true };
};
