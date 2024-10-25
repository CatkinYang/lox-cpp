#pragma once

#include "Expression.h"
#include "Token.h"
#include "Tokentype.h"
#include <future>
#include <memory>
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
    Parser(std::vector<std::shared_ptr<Token>> tokens) {
        m_tokens = std::move(tokens);
    }
    auto parse() -> AbstractExpressionRef;

    // 顶级表达式，可以匹配任何优先级的表达式
    auto expression() -> AbstractExpressionRef;
    // 等于 == !=
    auto equality() -> AbstractExpressionRef;
    // 比较 > >= < <=
    auto comparison() -> AbstractExpressionRef;
    // 加减 + -
    auto term() -> AbstractExpressionRef;
    auto factor() -> AbstractExpressionRef; // 乘除 * /
    auto unary() -> AbstractExpressionRef;  // 一元运算符 ！-
    // 基本表达式
    auto primary() -> AbstractExpressionRef;

    // 检查当前current指向的token的type是否和传入的type相等
    auto check(TokenType type) -> bool;
    // 返回当前的token，并且前进一位
    auto advance() -> TokenRef;

    // 判断当前Token是否是结尾（==EOF）
    auto isAtEnd() -> bool;
    // 返回当前指向的TokenRef
    auto peek() -> TokenRef;
    // 返回前一个TokenRef
    auto previous() -> TokenRef;
    // 报错
    auto error(TokenRef token, std::string message) -> ParserError;
    auto synchronize() -> void;

    // 检查当前的type是否存在于传入的type中，如果存在，那么前进并且返回true；否则返回false
    template <typename... Args> auto match(Args... types) -> bool;
    auto consume(TokenType type, std::string message) -> TokenRef;

  private:
    int m_current = 0;
    std::vector<TokenRef> m_tokens;
};

} // namespace lox
