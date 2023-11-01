#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

#include "numbspec.h"

struct LocalVariable {
    LocalVariable() { }

    LocalVariable(std::string_view name, size_t depth)
        : name(name)
        , depth(depth)
    {
    }

    std::string_view name;
    size_t depth;
};

struct Compiler {
    Compiler()
        : scope_depth(0)
    {
    }

    bool push_local(std::string_view);

    int32_t find_local(std::string_view);

    void begin_scope();

    void end_scope();

    std::vector<LocalVariable> locals;
    std::unordered_map<std::string_view, NumbTypeInfo> locals_type_infos;
    size_t scope_depth;
};
