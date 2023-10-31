#include "object.h"

GcObjectType ObjString::type() const
{
    return GcObjectType::String;
}

void ObjString::mark()
{
    m_marked = true;
}
