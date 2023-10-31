#include "numbspec.h"

std::optional<NumbBinop> token_type_to_numb_binop(Token::Type type)
{
    switch (type) {
    case Token::Type::Plus:
        return NumbBinop::Add;
    case Token::Type::Minus:
        return NumbBinop::Sub;
    case Token::Type::Star:
        return NumbBinop::Mul;
    case Token::Type::Slash:
        return NumbBinop::Div;
    case Token::Type::LessThan:
        return NumbBinop::LessThan;
    case Token::Type::GreaterThan:
        return NumbBinop::GreaterThan;
    case Token::Type::EqualEqual:
        return NumbBinop::Equal;
    case Token::Type::BangEqual:
        return NumbBinop::NotEqual;
    default:
        return {};
    }
}

std::optional<NumbUnary> token_type_to_numb_unary(Token::Type type)
{
    switch (type) {
    case Token::Type::Minus:
        return NumbUnary::Minus;
    case Token::Type::Bang:
        return NumbUnary::Not;
    default:
        return {};
    }
}

std::map<NumbType, NumbTypeInfo> g_numb_type_infos = {
    { NumbType::Int, NumbTypeInfo(NumbType::Int, "int", true, true) },
    { NumbType::Bool, NumbTypeInfo(NumbType::Bool, "bool", true, true) },
    { NumbType::Void, NumbTypeInfo(NumbType::Void, "void", false, true) },
};

std::map<NumbBinop, NumbBinopInfo> g_numb_binop_infos = {
    { NumbBinop::Add,
        NumbBinopInfo(NumbBinop::Add, "+", { NumbType::Int }, NumbType::Int) },
    { NumbBinop::Sub,
        NumbBinopInfo(NumbBinop::Sub, "-", { NumbType::Int }, NumbType::Int) },
    { NumbBinop::Mul,
        NumbBinopInfo(NumbBinop::Mul, "*", { NumbType::Int }, NumbType::Int) },
    { NumbBinop::Div,
        NumbBinopInfo(NumbBinop::Div, "/", { NumbType::Int }, NumbType::Int) },
    { NumbBinop::LessThan,
        NumbBinopInfo(NumbBinop::LessThan, "<", { NumbType::Int }, NumbType::Bool) },
    { NumbBinop::GreaterThan, NumbBinopInfo(NumbBinop::GreaterThan, ">", { NumbType::Int }, NumbType::Bool) },
    { NumbBinop::Equal,
        NumbBinopInfo(NumbBinop::Equal, "==", { NumbType::Int, NumbType::Bool },
            NumbType::Bool) },
    { NumbBinop::NotEqual,
        NumbBinopInfo(NumbBinop::NotEqual, "!=", { NumbType::Int, NumbType::Bool },
            NumbType::Bool) },
};

std::map<NumbUnary, NumbUnaryInfo> g_numb_unary_infos = {
    { NumbUnary::Minus,
        NumbUnaryInfo(NumbUnary::Minus, "-", { NumbType::Int }, NumbType::Int) },
    { NumbUnary::Not,
        NumbUnaryInfo(NumbUnary::Not, "!", { NumbType::Bool }, NumbType::Bool) },
};
