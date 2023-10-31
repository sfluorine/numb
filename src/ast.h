#pragma once

#include <memory>

#include "token.h"

class AstVisitor;

#define OVERLOAD_ACCEPT() virtual void accept(AstVisitor&) override;
#define DEFINE_OVERLOADED_ACCEPT(class) void class ::accept(AstVisitor& visitor)

struct Ast {
    virtual ~Ast()
    {
    }

    virtual void accept(AstVisitor&) = 0;

    // This function will be called when typechecking succeed
    void set_done()
    {
        done = !done;
    }

    bool done { false };
    size_t line { 1 };

protected:
    Ast(size_t line)
        : line(line)
    {
    }
};

struct Expr : public Ast {
    virtual ~Expr()
    {
    }

protected:
    Expr(size_t line)
        : Ast(line)
    {
    }
};

struct ExprInt : public Expr {
    ExprInt(int64_t parsed_int, size_t line)
        : parsed_int(parsed_int)
        , Expr(line)
    {
    }

    OVERLOAD_ACCEPT();

    int64_t parsed_int;
};

struct ExprBool : public Expr {
    ExprBool(bool parsed_bool, size_t line)
        : parsed_bool(parsed_bool)
        , Expr(line)
    {
    }

    OVERLOAD_ACCEPT();

    bool parsed_bool;
};

struct ExprUnary : public Expr {
    ExprUnary(Token op, std::shared_ptr<Expr> expr, size_t line)
        : op(op)
        , expr(std::move(expr))
        , Expr(line)
    {
    }

    OVERLOAD_ACCEPT();

    Token op;
    std::shared_ptr<Expr> expr;
};

struct ExprBinary : public Expr {
    ExprBinary(Token op, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs,
        size_t line)
        : op(op)
        , lhs(std::move(lhs))
        , rhs(std::move(rhs))
        , Expr(line)
    {
    }

    OVERLOAD_ACCEPT();

    Token op;
    std::shared_ptr<Expr> lhs;
    std::shared_ptr<Expr> rhs;
};

struct Stmt : public Ast {
    virtual ~Stmt()
    {
    }

protected:
    Stmt(size_t line)
        : Ast(line)
    {
    }
};

struct StmtLet : public Stmt {
    StmtLet(std::string name, std::shared_ptr<Expr> expr, size_t line)
        : name(std::move(name))
        , expr(std::move(expr))
        , Stmt(line)
    {
    }

    OVERLOAD_ACCEPT();

    std::string name;
    std::shared_ptr<Expr> expr;
};

#define DEFINE_VIRTUAL_VISIT()           \
    virtual void visit(ExprInt*) = 0;    \
    virtual void visit(ExprBool*) = 0;   \
    virtual void visit(ExprUnary*) = 0;  \
    virtual void visit(ExprBinary*) = 0; \
    virtual void visit(StmtLet*) = 0;

#define OVERLOAD_VISIT()                      \
    virtual void visit(ExprInt*) override;    \
    virtual void visit(ExprBool*) override;   \
    virtual void visit(ExprUnary*) override;  \
    virtual void visit(ExprBinary*) override; \
    virtual void visit(StmtLet*) override;

struct AstVisitor {
    virtual ~AstVisitor()
    {
    }

    DEFINE_VIRTUAL_VISIT();
};
