#pragma once

#include <map>
#include <string_view>

#include "token.h"

class Lexer {
public:
    Lexer(std::string_view);

    Token next();

private:
    char const* start_span() const;

    bool eof() const;

    char current() const;

    void consume();

    void skip_whitespaces();

    // TODO: implement this
    void skip_comments();

    Token::Type resolve_keywords(std::string_view) const;

private:
    static std::map<std::string_view, Token::Type> m_keywords;

    std::string_view m_input;
    size_t m_cursor;
    size_t m_line;
};
