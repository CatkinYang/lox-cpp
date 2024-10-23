#include "include/Parser.h"
#include "include/Expression.h"
#include "include/Lox.h"
#include "include/Scanner.h"
#include "include/Token.h"
#include "include/Tokentype.h"
namespace lox {

static Lox lox;

auto Parser::parse() -> AbstractExpression {
    try {
        return expression();
    } catch (ParserError error) {
    }
}

auto Parser::expression() -> AbstractExpression { return equality(); }

auto Parser::equality() -> AbstractExpression {
    auto expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        auto opt = previous();
        auto right = comparison();
        expr = new BinaryExpression();
    }
    return expr;
}

auto Parser::comparison() -> AbstractExpression {
    auto expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        auto opt = previous();
        auto right = term();
        expr = new BinaryExpression();
    }
    return expr;
}

auto Parser::term() -> AbstractExpression {
    auto expr = factor();
    while (match(MINUS, PLUS)) {
        auto opt = previous();
        auto right = factor();
        expr = new BinaryExpression();
    }
    return expr;
}

auto Parser::factor() -> AbstractExpression {
    auto expr = unary();
    while (match(SLASH, STAR)) {
        auto opt = previous();
        auto right = unary();
        expr = new BinaryExpression();
    }
    return expr;
}

auto Parser::unary() -> AbstractExpression {
    if (match(BANG, MINUS)) {
        auto opt = previous();
        auto right = unary();
        return new Expr.unary();
    }
    return primary();
}

auto Parser::primary() -> AbstractExpression {
    if (match(FALSE))
        return new Expr.Literal(false);
    if (match(TRUE))
        return new Expr.Literal(true);
    if (match(NIL))
        return new Expr.Literal(null);

    if (match(NUMBER, STRING)) {
        return new Expr.Literal(previous().literal);
    }

    if (match(LEFT_PAREN)) {
        Expr expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Expr.Grouping(expr);
    }
    throw error(peek(), "Expect expression.");
}

template <typename... Args> auto Parser::match(Args... types) -> bool {
    for (TokenType type : {types...}) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

auto Parser::consume(TokenType type, std::string message) -> Token {
    if (check(type))
        return advance();
    throw error(peek(), message);
}

auto Parser::check(TokenType type) -> bool {
    if (isAtEnd())
        return false;
    return peek().getType() == type;
}

auto Parser::advance() -> Token {
    if (!isAtEnd())
        m_current++;
    return previous();
}

auto Parser::isAtEnd() -> bool { return peek().getType() == EOF_TOKEN; }
auto Parser::peek() -> Token { return m_tokens[m_current]; }
auto Parser::previous() -> Token { return m_tokens[m_current - 1]; }
auto Parser::error(Token token, std::string message) -> ParserError {
    lox.error(token, message);
    return ParserError();
}

auto Parser::synchronize() -> void {
    advance();
    while (!isAtEnd()) {
        if (previous().getType() == SEMICOLON)
            return;

        switch (peek().getType()) {
        default:
            return;
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }

        advance();
    }
}

} // namespace lox
