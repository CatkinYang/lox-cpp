#include "include/Parser.h"
#include "include/Expression.h"
#include "include/Lox.h"
#include "include/Object.h"
#include "include/Scanner.h"
#include "include/Token.h"
#include "include/Tokentype.h"
#include <memory>
namespace lox {

static Lox lox;

auto Parser::parse() -> AbstractExpressionRef {
    try {
        return expression();
    } catch (ParserError error) {
    }
}

auto Parser::expression() -> AbstractExpressionRef { return equality(); }

auto Parser::equality() -> AbstractExpressionRef {
    auto expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        auto opt = previous();
        auto right = comparison();
        expr = std::static_pointer_cast<AbstractExpression>(
            std::make_shared<BinaryExpression>(expr, right, opt));
    }
    return expr;
}

auto Parser::comparison() -> AbstractExpressionRef {
    auto expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        auto opt = previous();
        auto right = term();
        expr = std::static_pointer_cast<AbstractExpression>(
            std::make_shared<BinaryExpression>(expr, right, opt));
    }
    return expr;
}

auto Parser::term() -> AbstractExpressionRef {
    auto expr = factor();
    while (match(MINUS, PLUS)) {
        auto opt = previous();
        auto right = factor();
        expr = std::static_pointer_cast<AbstractExpression>(
            std::make_shared<BinaryExpression>(expr, right, opt));
    }
    return expr;
}

auto Parser::factor() -> AbstractExpressionRef {
    auto expr = unary();
    while (match(SLASH, STAR)) {
        auto opt = previous();
        auto right = unary();
        expr = std::static_pointer_cast<AbstractExpression>(
            std::make_shared<BinaryExpression>(expr, right, opt));
    }
    return expr;
}

auto Parser::unary() -> AbstractExpressionRef {
    if (match(BANG, MINUS)) {
        auto opt = previous();
        auto right = unary();
        return std::static_pointer_cast<AbstractExpression>(
            std::make_shared<UnaryExpression>(right, opt));
    }
    return primary();
}

auto Parser::primary() -> AbstractExpressionRef {
    if (match(FALSE)) {
        auto false_literal_obj = Object::make_bool_obj(false);
        auto literal_expr =
            std::make_shared<LiteralExpression>(false_literal_obj);
        auto res = std::static_pointer_cast<AbstractExpression>(literal_expr);
        return res;
    }
    if (match(TRUE)) {
        auto true_literal_obj = Object::make_bool_obj(true);
        auto literal_expr =
            std::make_shared<LiteralExpression>(true_literal_obj);
        auto res = std::static_pointer_cast<AbstractExpression>(literal_expr);
        return res;
    }

    if (match(NIL)) {
        auto nil_literal_obj = Object::make_nil_obj();
        auto literal_expr =
            std::make_shared<LiteralExpression>(nil_literal_obj);
        auto res = std::static_pointer_cast<AbstractExpression>(literal_expr);
        return res;
    }

    if (match(NUMBER, STRING)) {
        auto pre_literal_obj = previous()->getLiteral();
        auto pre_literal_expr =
            std::make_shared<LiteralExpression>(pre_literal_obj);
        auto res =
            std::static_pointer_cast<AbstractExpression>(pre_literal_expr);
        return res;
    }

    if (match(LEFT_PAREN)) {
        auto expr = expression();
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

auto Parser::consume(TokenType type, std::string message) -> TokenRef {
    if (check(type))
        return advance();
    throw error(peek(), message);
}

auto Parser::check(TokenType type) -> bool {
    if (isAtEnd())
        return false;
    return peek()->getType() == type;
}

auto Parser::advance() -> TokenRef {
    if (!isAtEnd())
        m_current++;
    return previous();
}

auto Parser::isAtEnd() -> bool { return peek()->getType() == EOF_TOKEN; }
auto Parser::peek() -> TokenRef { return m_tokens[m_current]; }
auto Parser::previous() -> TokenRef { return m_tokens[m_current - 1]; }

auto Parser::error(TokenRef token, std::string message) -> ParserError {
    lox.error(token, message);
    return ParserError();
}

auto Parser::synchronize() -> void {
    advance();
    while (!isAtEnd()) {
        if (previous()->getType() == SEMICOLON)
            return;

        switch (peek()->getType()) {
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
