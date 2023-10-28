#pragma once

#include <optional>
#include <string>
#include <vector>

#include "ast.h"
#include "lexer.h"
#include "common.h"

class Parser
{
public:
    Parser(std::string_view input) : m_lexer(input), m_current(m_lexer.next())
    {
    }

    std::optional<std::shared_ptr<Expr>> parse_expression();
    std::optional<std::shared_ptr<Expr>> parse_term();
    std::optional<std::shared_ptr<Expr>> parse_factor();
    std::optional<std::shared_ptr<Expr>> parse_primary();

    std::optional<std::shared_ptr<Stmt>> parse_statement();
    std::optional<std::shared_ptr<Stmt>> parse_let_stmt();

    std::vector<std::string> const& errors() const
    {
        return m_errors;
    }

private:
    template <typename... Args>
    void error(std::string_view, Args&&...);

    bool expect(Token::Type) const;

    bool eof() const;

    void consume();

    void match(Token::Type);

private:
    Lexer m_lexer;
    Token m_current;
    std::vector<std::string> m_errors;
};
