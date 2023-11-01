#include "compiler.h"

bool Compiler::push_local(std::string_view name)
{
    if (find_local(name) != -1) {
        return false;
    }

    locals.emplace_back(name, scope_depth);
    return true;
}

int32_t Compiler::find_local(std::string_view name)
{
    for (int32_t i = 0; i < locals.size(); i++) {
        if (locals[i].name == name) {
            return i;
        }
    }

    return -1;
}

void Compiler::begin_scope()
{
    ++scope_depth;
}

void Compiler::end_scope()
{
    scope_depth--;
}
