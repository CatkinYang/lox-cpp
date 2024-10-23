#pragma once

#include "Expression.h"
#include "Token.h"
#include "Tokentype.h"
#include <stdexcept>
#include <string>
#include <vector>
namespace lox {

class Parser {
  public:
    class ParserError : std::runtime_error {
      public:
        ParserError();
    };

  public:
    Parser(std::vector<Token> tokens) { m_tokens = tokens; }
    auto parse() -> AbstractExpression;

    auto expression() -> AbstractExpression;
    auto equality() -> AbstractExpression;
    auto comparison() -> AbstractExpression;
    auto term() -> AbstractExpression;
    auto factor() -> AbstractExpression;
    auto unary() -> AbstractExpression;
    auto primary() -> AbstractExpression;

    auto check(TokenType type) -> bool;
    auto advance() -> Token;

    auto isAtEnd() -> bool;
    auto peek() -> Token;
    auto previous() -> Token;
    auto error(Token token, std::string message) -> ParserError;
    auto synchronize() -> void;

    template <typename... Args> auto match(Args... types) -> bool;
    auto consume(TokenType type, std::string message);

  private:
    int m_current = 0;
    std::vector<Token> m_tokens;
};

} // namespace lox
