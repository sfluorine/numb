#pragma once

#include <string_view>

#include "ast.h"
#include "compiler.h"
#include "numbspec.h"

struct Typechecker : public AstVisitor {
    Typechecker(Compiler& compiler)
        : compiler(compiler)
    {
    }

    OVERLOAD_VISIT();

    template<typename... Args>
    void error(size_t, std::string_view, Args&&...);

    Compiler& compiler;
    bool has_error { false };
    NumbType type;
};
