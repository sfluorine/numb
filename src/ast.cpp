#include "ast.h"

DEFINE_OVERLOADED_ACCEPT(ExprInt)
{
    visitor.visit(this);
}

DEFINE_OVERLOADED_ACCEPT(ExprBool)
{
    visitor.visit(this);
}

DEFINE_OVERLOADED_ACCEPT(ExprBinary)
{
    visitor.visit(this);
}

DEFINE_OVERLOADED_ACCEPT(StmtLet)
{
    visitor.visit(this);
}
