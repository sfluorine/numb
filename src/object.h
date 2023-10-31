#pragma once

#include <assert.h>
#include <string>

enum class GcObjectType {
    String,
};

class GcObject {
public:
    virtual ~GcObject() = default;

    GcObject(GcObject const&) = default;
    GcObject& operator=(GcObject const&) = default;

    GcObject(GcObject&&) = default;
    GcObject& operator=(GcObject&&) = default;

    virtual GcObjectType type() const { assert(false && "GcObject::type() is not implemented"); }

    virtual void mark() { assert(false && "GcObject::mark() is not implemented"); }

    bool marked() const { return m_marked; }

protected:
    bool m_marked { false };

protected:
    GcObject() = default;
};

class ObjString : public GcObject {
public:
    ObjString(std::string str)
        : str(std::move(str))
    {
    }

    virtual GcObjectType type() const override;

    virtual void mark() override;

    std::string str;
};
