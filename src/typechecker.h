#pragma once

#include <string_view>

#include "ast.h"
#include "numbspec.h"

struct Typechecker : public AstVisitor {
    OVERLOAD_VISIT();

    template<typename... Args>
    void error(size_t, std::string_view, Args&&...);

    NumbType type;
};
