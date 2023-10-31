#include <assert.h>

#include "token.h"

char const* Token::type_to_string(Type type)
{
    switch (type) {
    case Token::Type::EndOfFile:
        return "end of file";
    case Token::Type::Garbage:
        return "garbage";

    case Token::Type::Let:
        return "let";
    case Token::Type::Def:
        return "def";
    case Token::Type::If:
        return "if";
    case Token::Type::Else:
        return "else";

    case Token::Type::True:
        return "true";
    case Token::Type::False:
        return "false";

    case Token::Type::IntLiteral:
        return "int literal";
    case Token::Type::Identifier:
        return "identifier";

    case Token::Type::LParen:
        return "(";
    case Token::Type::RParen:
        return ")";
    case Token::Type::LCurly:
        return "{";
    case Token::Type::RCurly:
        return "}";
    case Token::Type::Semicolon:
        return ";";

    case Token::Type::Plus:
        return "+";
    case Token::Type::Minus:
        return "-";
    case Token::Type::Star:
        return "*";
    case Token::Type::Slash:
        return "/";

    case Token::Type::LessThan:
        return "<";
    case Token::Type::GreaterThan:
        return ">";
    case Token::Type::Equal:
        return "=";
    case Token::Type::EqualEqual:
        return "==";

    case Token::Type::Bang:
        return "!";
    case Token::Type::BangEqual:
        return "!=";

    default:
        assert(false && "please update this code immediately");
    }
}
