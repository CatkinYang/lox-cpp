#pragma once

#include "Expression.h"
#include "Object.h"
#include "Statements.h"
#include "Token.h"
#include "Tokentype.h"
#include <stdexcept>
#include <string>
#include <vector>
namespace lox {
// 解析器类，接受扫描器传入的tokens生成AST
// 递归下降法
class Parser {
  public:
    Parser(std::vector<TokenRef> tokens) { m_tokens = std::move(tokens); }
    // parse方法启动解析过程，返回AST的根节点；尝试解析一个表达式并返回其AST表示。
  public:
    auto parse() -> std::vector<StmtRef>;
    auto statement() -> StmtRef;
    auto declaration() -> StmtRef;
    auto varDeclaration() -> StmtRef;

    // auto printStatement() -> PrintStmtRef;
    // auto expressionStatment() -> ExpressionStmtRef;
    // auto ifStatement() -> IfStmtRef;
    // auto whileStatement() -> WhileStmtRef;
    // auto forStatement() -> ForStmtRef;

    auto printStatement() -> StmtRef;
    auto expressionStatment() -> StmtRef;
    auto ifStatement() -> StmtRef;
    auto whileStatement() -> StmtRef;
    auto forStatement() -> StmtRef;

    auto block() -> std::vector<StmtRef>;

    auto assignment() -> AbstractExpressionRef<Object>;
    auto Or() -> AbstractExpressionRef<Object>;
    auto And() -> AbstractExpressionRef<Object>;

  public:
    // 顶级表达式，可以匹配任何优先级的表达式
    auto expression() -> AbstractExpressionRef<Object>;
    auto equality() -> AbstractExpressionRef<Object>;
    auto comparison() -> AbstractExpressionRef<Object>;
    auto term() -> AbstractExpressionRef<Object>;
    auto factor() -> AbstractExpressionRef<Object>;
    auto unary() -> AbstractExpressionRef<Object>;
    auto primary() -> AbstractExpressionRef<Object>;

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
    std::runtime_error error(TokenRef token, std::string message);
    auto synchronize() -> void;

    // 检查当前的type是否存在于传入的type中，如果存在，那么前进并且返回true；否则返回false
    template <typename... Args> auto match(Args... types) -> bool;
    auto consume(TokenType type, std::string message) -> TokenRef;

  private:
    int m_current = 0;
    std::vector<TokenRef> m_tokens;
};

} // namespace lox
