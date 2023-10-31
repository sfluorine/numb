#pragma once

#include "object.h"

#include <stdint.h>

#include <variant>
#include <vector>

enum class ValueType {
    Empty,
    I64,
    F64,
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

    Value(GcObject* object)
        : m_type(ValueType::Object)
        , m_value(object)
    {
    }

    ValueType type() const { return m_type; }

    int64_t& as_i64() { return std::get<int64_t>(m_value); }

    double& as_f64() { return std::get<double>(m_value); }

    GcObject*& as_object() { return std::get<GcObject*>(m_value); }

private:
    ValueType m_type;
    std::variant<Empty, int64_t, double, GcObject*> m_value;
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

    T* get(int64_t index)
    {
        if (index >= m_index) {
            return nullptr;
        }

        return &m_data[index];
    }

private:
    int64_t m_index { 0 };
    std::vector<T> m_data;
};

enum class Instruction : uint8_t {
    Halt,

    I64Const,
    F64Const,

    I64Add,
    I64Sub,
    I64Mul,
    I64Div,
};

#define INSTRUCTION_TO_BYTE(ins) (static_cast<uint8_t>(ins))
#define BYTE_TO_INSTRUCTION(byte) (static_cast<Instruction>(byte))

class NumbVm {
public:
    NumbVm() { }

private:
    uint8_t fetch();

    int64_t read_qword();

    int32_t read_dword();

public:
    std::vector<uint8_t> bytecode;

    Pool<int64_t> i64_pool;
    Pool<double> f64_pool;

private:
    bool m_halt { true };
    int64_t m_pc { -1 }; // program counter
    int64_t m_sp { -1 }; // stack pointer
};
