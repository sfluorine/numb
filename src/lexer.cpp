#include <ctype.h>

#include <iostream>

#include "lexer.h"

std::map<std::string_view, Token::Type> Lexer::m_keywords;

Lexer::Lexer(std::string_view input = "")
    : m_input(input)
    , m_cursor(0)
    , m_line(1)
{
    if (m_keywords.empty()) {
        m_keywords.insert({ "let", Token::Type::Let });
        m_keywords.insert({ "def", Token::Type::Def });
        m_keywords.insert({ "if", Token::Type::If });
        m_keywords.insert({ "else", Token::Type::Else });
        m_keywords.insert({ "true", Token::Type::True });
        m_keywords.insert({ "false", Token::Type::False });
    }
}

Token Lexer::next()
{
    skip_whitespaces();

    auto const start = start_span();
    auto const start_line = m_line;

    if (eof()) {
        return Token(Token::Type::EndOfFile, start_line);
    }

    switch (current()) {
    case '(':
        consume();
        return Token(Token::Type::LParen, start_line);
    case ')':
        consume();
        return Token(Token::Type::RParen, start_line);
    case '{':
        consume();
        return Token(Token::Type::LCurly, start_line);
    case '}':
        consume();
        return Token(Token::Type::RCurly, start_line);
    case ';':
        consume();
        return Token(Token::Type::Semicolon, start_line);
    case '+':
        consume();
        return Token(Token::Type::Plus, start_line);
    case '-':
        consume();
        return Token(Token::Type::Minus, start_line);
    case '*':
        consume();
        return Token(Token::Type::Star, start_line);
    case '/':
        consume();
        return Token(Token::Type::Slash, start_line);
    case '<':
        consume();
        return Token(Token::Type::LessThan, start_line);
    case '>':
        consume();
        return Token(Token::Type::GreaterThan, start_line);
    case '=':
        consume();
        if (current() == '=') {
            consume();
            return Token(Token::Type::EqualEqual, start_line);
        }
        return Token(Token::Type::Equal, start_line);
    case '!':
        consume();
        if (current() == '=') {
            consume();
            return Token(Token::Type::BangEqual, start_line);
        }
        return Token(Token::Type::Bang, start_line);
    default:
        break;
    }

    if (isdigit(current())) {
        size_t len = 0;
        do {
            len++;
            consume();
        } while (!eof() && isdigit(current()));

        auto span = std::string_view(start, len);

        return Token(Token::Type::IntLiteral, span, start_line);
    }

    if (isalpha(current()) || current() == '_') {
        size_t len = 0;
        do {
            len++;
            consume();
        } while (!eof() && (isalnum(current()) || current() == '_'));

        auto span = std::string_view(start, len);

        return Token(resolve_keywords(span), span, start_line);
    }

    size_t garbage_len = 0;
    do {
        garbage_len++;
        consume();
    } while (!eof() && !isspace(current()));

    auto garbage_span = std::string_view(start, garbage_len);

    return Token(Token::Type::Garbage, garbage_span, start_line);
}

char const* Lexer::start_span() const
{
    return m_input.data() + m_cursor;
}

bool Lexer::eof() const
{
    return m_cursor >= m_input.length();
}

char Lexer::current() const
{
    return m_input[m_cursor];
}

void Lexer::consume()
{
    if (eof()) {
        return;
    }

    if (current() == '\n') {
        m_line++;
    }

    m_cursor++;
}

void Lexer::skip_whitespaces()
{
    while (!eof() && isspace(current())) {
        consume();
    }
}

Token::Type Lexer::resolve_keywords(std::string_view span) const
{
    if (!m_keywords.contains(span)) {
        return Token::Type::Identifier;
    }

    return m_keywords[span];
}
