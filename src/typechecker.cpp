#include <format>
#include <iostream>
#include <map>

#include "typechecker.h"

template<typename... Args>
void Typechecker::error(size_t line, std::string_view format, Args&&... args)
{
    std::cerr << std::format(
        "{}: ERROR: {}", line,
        std::vformat(format, std::make_format_args(std::forward<Args>(args)...)))
              << '\n';
}

void Typechecker::visit(ExprInt* expr)
{
    type = NumbType::Int;
    expr->result_type = type;
    expr->set_done();
}

void Typechecker::visit(ExprBool* expr)
{
    type = NumbType::Bool;
    expr->result_type = type;
    expr->set_done();
}

void Typechecker::visit(ExprUnary* expr)
{
    auto& inner_expr = expr->expr;
    inner_expr->accept(*this);

    if (!inner_expr->done) {
        return;
    }

    auto& expr_type = g_numb_type_infos[type];

    auto opt_op = token_type_to_numb_unary(expr->op.type);
    if (!opt_op) {
        error(expr->line, "unimplemented for binop '{}'", Token::type_to_string(expr->op.type));
        return;
    }

    auto op = opt_op.value();
    auto& op_info = g_numb_unary_infos[op];

    type = op_info.returns;
    expr->result_type = type;

    for (auto const& take : op_info.takes) {
        if (expr_type.type == op_info.returns) {
            expr->set_done();
            return;
        }
    }

    error(expr->line,
        "unsupported type for '{}'\n    expr -> '{}'",
        Token::type_to_string(expr->op.type), expr_type.name);
}

void Typechecker::visit(ExprBinary* expr)
{
    expr->lhs->accept(*this);
    auto& lhs_type = g_numb_type_infos[type];

    expr->rhs->accept(*this);
    auto& rhs_type = g_numb_type_infos[type];

    if (lhs_type.type != rhs_type.type) {
        error(expr->line,
            "mismatched types for '{}'\n    lhs -> '{}'\n    rhs -> '{}'",
            Token::type_to_string(expr->op.type), lhs_type.name,
            rhs_type.name);
        return;
    }

    auto opt_op = token_type_to_numb_binop(expr->op.type);
    if (!opt_op) {
        error(expr->line, "unimplemented for binop '{}'", Token::type_to_string(expr->op.type));
        return;
    }

    auto op = opt_op.value();
    auto& op_info = g_numb_binop_infos[op];

    type = op_info.returns;
    expr->result_type = type;

    for (auto const& take : op_info.takes) {
        if (lhs_type.type == take) {
            expr->set_done();
            return;
        }
    }

    error(expr->line,
        "unsupported types for '{}'\n    lhs -> '{}'\n    rhs -> '{}'",
        Token::type_to_string(expr->op.type), lhs_type.name, rhs_type.name);
}

void Typechecker::visit(StmtLet* stmt)
{
    stmt->expr->accept(*this);

    if (stmt->expr->done) {
        stmt->set_done();
    }
}
