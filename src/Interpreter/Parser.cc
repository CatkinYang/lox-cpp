#include "Interpreter/Parser.h"
#include "Interpreter/Expression.h"
#include "Interpreter/Object.h"
#include "Interpreter/Statements.h"
#include "Interpreter/Token.h"
#include "Interpreter/Tokentype.h"

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

    if (match(RETURN))
        return std::dynamic_pointer_cast<Stmt>(returnStatement());

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
        if (match(CLASS)) {
            return classDeclaration();
        }
        if (match(FUN)) {
            return function("function");
        }
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
    auto var_stmt = std::make_shared<VarStmt>(name, initializer);
    return var_stmt;
}

auto Parser::function(std::string kind) -> StmtRef {
    auto name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<TokenRef> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }
            parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<StmtRef> body = block();
    auto fun_stmt = std::make_shared<FunStmt>(name, parameters, body);
    return fun_stmt;
}

auto Parser::classDeclaration() -> StmtRef {
    auto name = consume(IDENTIFIER, "Expect class name.");
    VariableExpressionRef<Object> superclass = nullptr;
    if (match(LESS)) {
        consume(IDENTIFIER, "Expect superclass name.");
        superclass = std::make_shared<VariableExpression<Object>>(previous());
    }

    consume(LEFT_BRACE, "Expect '{' before class body.");
    std::vector<FunStmtRef> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        methods.push_back(
            std::dynamic_pointer_cast<FunStmt>(function("method")));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    auto class_stmt = std::make_shared<ClassStmt>(name, superclass, methods);
    return class_stmt;
}

auto Parser::printStatement() -> StmtRef {
    auto value = expression();
    consume(SEMICOLON, "Exprect ';' after value.");
    auto print_stmt = std::make_shared<PrintStmt>(value);
    return print_stmt;
}

auto Parser::expressionStatment() -> StmtRef {
    auto value = expression();
    consume(SEMICOLON, "Exprect ';' after expression.");
    auto expr_stmt = std::make_shared<ExpressionStmt>(value);
    return expr_stmt;
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
    auto if_stmt = std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
    return if_stmt;
}

auto Parser::whileStatement() -> StmtRef {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' aftercondition");

    auto body = statement();
    auto while_stmt = std::make_shared<WhileStmt>(condition, body);
    return while_stmt;
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

auto Parser::returnStatement() -> StmtRef {
    auto Keyword = previous();
    AbstractExpressionRef<Object> value = nullptr;
    if (!check(SEMICOLON)) {
        value = expression();
    }
    consume(SEMICOLON, "Expect ';' after return value.");
    auto return_stmt = std::make_shared<ReturnStmt>(Keyword, value);
    return return_stmt;
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
    // return primary();
    return call();
}

auto Parser::call() -> AbstractExpressionRef<Object> {
    auto expr = primary();
    while (true) {
        if (match(LEFT_PAREN)) {
            expr = finishCall(expr);
        } else if (match(DOT)) {
            auto name = consume(IDENTIFIER, "Expect property name after '.'.");
            auto getExpr = std::make_shared<GetExpression<Object>>(expr, name);
            expr = getExpr;
        } else {
            break;
        }
    }
    return expr;
}

auto Parser::finishCall(AbstractExpressionRef<Object> callee)
    -> AbstractExpressionRef<Object> {
    std::vector<AbstractExpressionRef<Object>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match(COMMA));
    }
    auto paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");
    auto call_res =
        std::make_shared<CallExpression<Object>>(callee, paren, arguments);
    return call_res;
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
    if (match(SUPER)) {
        auto keyword = previous();
        consume(DOT, "Expect '.' after 'super'.");
        auto method = consume(IDENTIFIER, "Expect superclass method name.");
        auto super = std::make_shared<SuperExpression<Object>>(keyword, method);
        auto res = std::static_pointer_cast<AbstractExpression<Object>>(super);
        return res;
    }

    if (match(THIS)) {
        auto this_obj = previous();
        auto this_expr = std::make_shared<ThisExpression<Object>>(this_obj);
        return std::static_pointer_cast<AbstractExpression<Object>>(this_expr);
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
