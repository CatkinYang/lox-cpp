#include "include/Parser.h"
#include "include/Expression.h"
#include "include/Object.h"
#include "include/Statements.h"
#include "include/Token.h"
#include "include/Tokentype.h"
#include <memory>
#include <stdexcept>
#include <vector>
namespace lox {

auto Parser::parse() -> std::vector<StmtRef> {
    std::vector<StmtRef> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

auto Parser::statement() -> StmtRef {
    if (match(FOR))
        return std::dynamic_pointer_cast<Stmt>(forStatement());

    if (match(IF))
        return std::dynamic_pointer_cast<Stmt>(ifStatement());

    if (match(PRINT))
        return std::dynamic_pointer_cast<Stmt>(printStatement());

    if (match(WHILE))

        return std::dynamic_pointer_cast<Stmt>(whileStatement());

    if (match(LEFT_BRACE)) {
        auto blockStmt = std::make_shared<BlockStmt>(block());
        return std::dynamic_pointer_cast<Stmt>(blockStmt);
    }
    return std::dynamic_pointer_cast<Stmt>(expressionStatment());
}

auto Parser::declaration() -> StmtRef {
    try {
        if (match(VAR)) {
            return varDeclaration();
        }
        return statement();
    } catch (std::runtime_error error) {
        synchronize();
        return nullptr;
    }
}

auto Parser::varDeclaration() -> StmtRef {
    auto name = consume(IDENTIFIER, "Expect variable name.");
    AbstractExpressionRef<Object> initializer = nullptr;
    if (match(EQUAL)) {
        initializer = expression();
    }
    consume(SEMICOLON, "Exprect ':' after variable declaration.");
    auto varRes = std::make_shared<VarStmt>(name, initializer);
    return std::dynamic_pointer_cast<Stmt>(varRes);
}

// auto Parser::printStatement() -> PrintStmtRef {
//     auto value = expression();
//     consume(SEMICOLON, "Exprect ';' after value.");
//     auto res = std::make_shared<PrintStmt>(value);
//     return res;
// }

// auto Parser::expressionStatment() -> ExpressionStmtRef {
//     auto value = expression();
//     consume(SEMICOLON, "Exprect ';' after expression.");
//     auto res = std::make_shared<ExpressionStmt>(value);
//     return res;
// }

// auto Parser::ifStatement() -> IfStmtRef {
//     consume(LEFT_PAREN, "Expect '(' after 'if'.");
//     auto condition = expression();
//     consume(RIGHT_PAREN, "Expect ')' after if condition");

//     auto thenBranch = statement();
//     StmtRef elseBranch = nullptr;
//     if (match(ELSE)) {
//         elseBranch = statement();
//     }
//     auto if_res = std::make_shared<IfStmt>(condition, thenBranch,
//     elseBranch); return if_res;
// }

// auto Parser::whileStatement() -> WhileStmtRef {
//     consume(LEFT_PAREN, "Expect '(' after 'while'.");
//     auto condition = expression();
//     consume(RIGHT_PAREN, "Expect ')' aftercondition");

//     auto body = statement();
//     auto while_res = std::make_shared<WhileStmt>(condition, body);
//     return while_res;
// }

auto Parser::printStatement() -> StmtRef {
    auto value = expression();
    consume(SEMICOLON, "Exprect ';' after value.");
    auto res = std::make_shared<PrintStmt>(value);
    return res;
}

auto Parser::expressionStatment() -> StmtRef {
    auto value = expression();
    consume(SEMICOLON, "Exprect ';' after expression.");
    auto res = std::make_shared<ExpressionStmt>(value);
    return res;
}

auto Parser::ifStatement() -> StmtRef {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition");

    auto thenBranch = statement();
    StmtRef elseBranch = nullptr;
    if (match(ELSE)) {
        elseBranch = statement();
    }
    auto if_res = std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
    return if_res;
}

auto Parser::whileStatement() -> StmtRef {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' aftercondition");

    auto body = statement();
    auto while_res = std::make_shared<WhileStmt>(condition, body);
    return while_res;
}

auto Parser::forStatement() -> StmtRef {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");
    StmtRef initializer;
    if (match(SEMICOLON)) {
        initializer = nullptr;
    } else if (match(VAR)) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatment();
    }
    AbstractExpressionRef<Object> condition = nullptr;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(LEFT_PAREN, "Expect ';' after loop condition.");

    AbstractExpressionRef<Object> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    auto body = statement();

    if (increment != nullptr) {
        auto increment_stmt = std::make_shared<ExpressionStmt>(increment);
        std::vector<StmtRef> vec = {body, increment_stmt};
        body = std::make_shared<BlockStmt>(vec);
    }
    if (condition == nullptr) {
        auto literal_obj = Object::make_bool_obj(true);
        condition = std::make_shared<LiteralExpression<Object>>(literal_obj);
    }
    body = std::make_shared<WhileStmt>(
        std::dynamic_pointer_cast<AbstractExpression<Object>>(condition), body);
    if (initializer != nullptr) {
        std::vector<StmtRef> vec = {initializer, body};
        body = std::make_shared<BlockStmt>(vec);
    }
    return body;
}

auto Parser::block() -> std::vector<StmtRef> {
    std::vector<StmtRef> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

auto Parser::assignment() -> AbstractExpressionRef<Object> {
    auto expr = equality();
    if (match(EQUAL)) {
        auto equals = previous();
        auto value = assignment();
        auto variable = dynamic_cast<VariableExpression<Object> *>(expr.get());
        if (variable != nullptr) {
            auto name = variable->getName();
            auto res =
                std::make_shared<AssignmentExpression<Object>>(name, value);
            return res;
        }
        error(equals, "Invalid assignment target.");
    }
    return expr;
}

auto Parser::Or() -> AbstractExpressionRef<Object> {
    auto expr = And();
    while (match(OR)) {
        auto opt = previous();
        auto right = And();
        auto logical =
            std::make_shared<LogicalExpression<Object>>(expr, right, opt);
        expr = logical;
    }
    return expr;
}

auto Parser::And() -> AbstractExpressionRef<Object> {
    auto expr = equality();
    while (match(AND)) {
        auto opt = previous();
        auto right = equality();
        auto logical =
            std::make_shared<LogicalExpression<Object>>(expr, right, opt);
        expr = logical;
    }
    return expr;
}

auto Parser::expression() -> AbstractExpressionRef<Object> {
    return assignment();
}

auto Parser::equality() -> AbstractExpressionRef<Object> {
    auto expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        auto opt = previous();
        auto right = comparison();
        expr = std::static_pointer_cast<AbstractExpression<Object>>(
            std::make_shared<BinaryExpression<Object>>(expr, right, opt));
    }
    return expr;
}

auto Parser::comparison() -> AbstractExpressionRef<Object> {
    auto expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        auto opt = previous();
        auto right = term();
        expr = std::static_pointer_cast<AbstractExpression<Object>>(
            std::make_shared<BinaryExpression<Object>>(expr, right, opt));
    }
    return expr;
}

auto Parser::term() -> AbstractExpressionRef<Object> {
    auto expr = factor();
    while (match(MINUS, PLUS)) {
        auto opt = previous();
        auto right = factor();
        expr = std::static_pointer_cast<AbstractExpression<Object>>(
            std::make_shared<BinaryExpression<Object>>(expr, right, opt));
    }
    return expr;
}

auto Parser::factor() -> AbstractExpressionRef<Object> {
    auto expr = unary();
    while (match(SLASH, STAR)) {
        auto opt = previous();
        auto right = unary();
        expr = std::static_pointer_cast<AbstractExpression<Object>>(
            std::make_shared<BinaryExpression<Object>>(expr, right, opt));
    }
    return expr;
}

auto Parser::unary() -> AbstractExpressionRef<Object> {
    if (match(BANG, MINUS)) {
        auto opt = previous();
        auto right = unary();
        return std::static_pointer_cast<AbstractExpression<Object>>(
            std::make_shared<UnaryExpression<Object>>(right, opt));
    }
    return primary();
}

auto Parser::primary() -> AbstractExpressionRef<Object> {
    if (match(FALSE)) {
        auto false_literal_obj = Object::make_bool_obj(false);
        auto literal_expr =
            std::make_shared<LiteralExpression<Object>>(false_literal_obj);
        auto res =
            std::static_pointer_cast<AbstractExpression<Object>>(literal_expr);
        return res;
    }
    if (match(TRUE)) {
        auto true_literal_obj = Object::make_bool_obj(true);
        auto literal_expr =
            std::make_shared<LiteralExpression<Object>>(true_literal_obj);
        auto res =
            std::static_pointer_cast<AbstractExpression<Object>>(literal_expr);
        return res;
    }

    if (match(NIL)) {
        auto nil_literal_obj = Object::make_nil_obj();
        auto literal_expr =
            std::make_shared<LiteralExpression<Object>>(nil_literal_obj);
        auto res =
            std::static_pointer_cast<AbstractExpression<Object>>(literal_expr);
        return res;
    }

    if (match(NUMBER, STRING)) {
        auto pre_literal_obj = *previous()->getLiteral().get();
        auto pre_literal_expr =
            std::make_shared<LiteralExpression<Object>>(pre_literal_obj);
        auto res = std::static_pointer_cast<AbstractExpression<Object>>(
            pre_literal_expr);
        return res;
    }

    if (match(IDENTIFIER)) {
        auto var_obj = previous();
        auto var_expr = std::make_shared<VariableExpression<Object>>(var_obj);
        auto res =
            std::static_pointer_cast<AbstractExpression<Object>>(var_expr);
        return res;
    }

    if (match(LEFT_PAREN)) {
        auto expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        auto res = std::make_shared<GroupingExpression<Object>>(expr);
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

// 判断当前current指向的token的type和给定的token的type是否相同
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

std::runtime_error Parser::error(TokenRef token, std::string message) {
    if (token->getType() == EOF_TOKEN) {
        return std::runtime_error(std::to_string(token->getLine()) + " at end" +
                                  message);
    } else {
        return std::runtime_error(std::to_string(token->getLine()) + " at '" +
                                  token->getLexeme() + "'" + message);
    }
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
