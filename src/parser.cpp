#include <charconv>
#include <format>

#include "parser.h"

#define RETURN_IF_ERROR() \
    if (has_errors()) {   \
        return {};        \
    }

std::optional<std::shared_ptr<Expr>> Parser::parse_expression()
{
    auto const starting_line = m_current.line;
    auto lhs = TRY(parse_term());

    while (expect(Token::Type::LessThan) || expect(Token::Type::GreaterThan) || expect(Token::Type::EqualEqual) || expect(Token::Type::BangEqual)) {
        auto op = m_current;
        consume();

        auto rhs = TRY(parse_term());
        auto binary = std::make_shared<ExprBinary>(
            op, std::move(lhs), std::move(rhs), starting_line);

        lhs = binary;
    }

    return lhs;
}

std::optional<std::shared_ptr<Expr>> Parser::parse_term()
{
    auto const starting_line = m_current.line;
    auto lhs = TRY(parse_factor());

    while (expect(Token::Type::Plus) || expect(Token::Type::Minus)) {
        auto op = m_current;
        consume();

        auto rhs = TRY(parse_factor());
        auto binary = std::make_shared<ExprBinary>(
            op, std::move(lhs), std::move(rhs), starting_line);

        lhs = binary;
    }

    return lhs;
}

std::optional<std::shared_ptr<Expr>> Parser::parse_factor()
{
    auto const starting_line = m_current.line;
    auto lhs = TRY(parse_primary());

    while (expect(Token::Type::Star) || expect(Token::Type::Slash)) {
        auto op = m_current;
        consume();

        auto rhs = TRY(parse_primary());
        auto binary = std::make_shared<ExprBinary>(
            op, std::move(lhs), std::move(rhs), starting_line);

        lhs = binary;
    }

    return lhs;
}

std::optional<std::shared_ptr<Expr>> Parser::parse_primary()
{
    auto const current_line = m_current.line;

    if (expect(Token::Type::LParen)) {
        consume();

        auto expr = TRY(parse_expression());

        match(Token::Type::RParen);

        RETURN_IF_ERROR();

        return expr;
    } else if (expect(Token::Type::IntLiteral)) {
        auto tok = m_current;
        auto span = tok.span;

        consume();

        int64_t result;
        std::from_chars(span.data(), span.data() + span.length(), result);

        return std::make_shared<ExprInt>(result, current_line);
    } else if (expect(Token::Type::True)) {
        consume();
        return std::make_shared<ExprBool>(true, current_line);
    } else if (expect(Token::Type::False)) {
        consume();
        return std::make_shared<ExprBool>(false, current_line);
    } else {
        return TRY(parse_unary());
    }
}

std::optional<std::shared_ptr<Expr>> Parser::parse_unary()
{
    auto const current_line = m_current.line;

    if (expect(Token::Type::Minus) || expect(Token::Type::Bang)) {
        auto tok = m_current;
        consume();

        auto opt_expr = parse_expression();
        if (!opt_expr) {
            error("expected expression");
            return {};
        }

        return std::make_shared<ExprUnary>(tok, std::move(opt_expr.value()),
            current_line);
    } else {
        error("expected unary");
        return {};
    }
}

std::optional<std::shared_ptr<Stmt>> Parser::parse_statement()
{
    return TRY(parse_let_stmt());
}

std::optional<std::shared_ptr<Stmt>> Parser::parse_let_stmt()
{
    auto const current_line = m_current.line;
    match(Token::Type::Let);

    auto name = m_current;
    auto name_span = name.span;
    match(Token::Type::Identifier);

    match(Token::Type::Equal);

    auto expr = parse_expression();
    if (!expr) {
        error("expected expression");
        return {};
    }

    match(Token::Type::Semicolon);

    RETURN_IF_ERROR();

    return std::make_shared<StmtLet>(std::string(name_span),
        std::move(expr.value()), current_line);
}

template<typename... Args>
void Parser::error(std::string_view format, Args&&... args)
{
    m_errors.emplace_back(std::format(
        "{}: ERROR: {}", m_current.line,
        std::vformat(format,
            std::make_format_args(std::forward<Args>(args)...))));
}

bool Parser::expect(Token::Type type) const
{
    return m_current.type == type;
}

bool Parser::eof() const
{
    return expect(Token::Type::EndOfFile);
}

void Parser::consume()
{
    if (eof()) {
        return;
    }

    m_current = m_lexer.next();
}

void Parser::match(Token::Type type)
{
    if (!expect(type)) {
        error("expected '{}' but got '{}'", Token::type_to_string(type),
            Token::type_to_string(m_current.type));
    }

    consume();
}
