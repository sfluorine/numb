#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "token.h"

enum class NumbType {
    Int,
    Bool,
    Void,
};

struct NumbTypeInfo {
    NumbTypeInfo()
    {
    }

    NumbTypeInfo(NumbType type, std::string name, bool valid_variable_type,
                 bool valid_function_return_type)
        : type(type),
          name(std::move(name)),
          valid_variable_type(valid_variable_type),
          valid_function_return_type(valid_function_return_type)
    {
    }

    NumbType type;
    std::string name;

    bool valid_variable_type;
    bool valid_function_return_type;
};

enum class NumbBinop {
    Add,
    Sub,
    Mul,
    Div,

    LessThan,
    GreaterThan,
    Equal,
    NotEqual,
};

std::optional<NumbBinop> token_type_to_numb_binop(Token::Type);

struct NumbBinopInfo {
    NumbBinopInfo()
    {
    }

    NumbBinopInfo(NumbBinop type, std::string repr, std::vector<NumbType> takes,
                  NumbType returns)
        : type(type),
          repr(std::move(repr)),
          takes(std::move(takes)),
          returns(returns)
    {
    }

    NumbBinop type;
    std::string repr;

    std::vector<NumbType> takes;
    NumbType returns;
};

enum class NumbUnary {
    Minus,
    Not,
};

std::optional<NumbUnary> token_type_to_numb_unary(Token::Type);

struct NumbUnaryInfo {
    NumbUnaryInfo()
    {
    }

    NumbUnaryInfo(NumbUnary type, std::string repr, std::vector<NumbType> takes,
                  NumbType returns)
        : type(type),
          repr(std::move(repr)),
          takes(std::move(takes)),
          returns(returns)
    {
    }

    NumbUnary type;
    std::string repr;

    std::vector<NumbType> takes;
    NumbType returns;
};

extern std::map<NumbType, NumbTypeInfo> g_numb_type_infos;
extern std::map<NumbBinop, NumbBinopInfo> g_numb_binop_infos;
extern std::map<NumbUnary, NumbUnaryInfo> g_numb_unary_infos;
