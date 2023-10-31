#pragma once

#include <stdint.h>

#include <string_view>

struct Token {
    enum class Type : uint8_t {
        EndOfFile,
        Garbage,

        Let,
        Def,
        If,
        Else,
        True,
        False,

        IntLiteral,
        Identifier,

        LParen,
        RParen,
        LCurly,
        RCurly,
        Semicolon,

        Plus,
        Minus,
        Star,
        Slash,

        LessThan,
        GreaterThan,
        Equal,
        EqualEqual,

        Bang,
        BangEqual,

        TypeCount,
    };

    static char const* type_to_string(Type);

    Token()
        : type(Type::Garbage)
        , span("")
        , line(1)
    {
    }

    Token(Type type, size_t line)
        : type(type)
        , span("")
        , line(line)
    {
    }

    Token(Type type, std::string_view span, size_t line)
        : type(type)
        , span(span)
        , line(line)
    {
    }

    Type type;
    std::string_view span;
    size_t line;
};
