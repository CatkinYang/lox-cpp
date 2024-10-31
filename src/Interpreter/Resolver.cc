#include "Interpreter/Resolver.h"
#include "Interpreter/Expression.h"
#include "Interpreter/Lox.h"
#include "Interpreter/Object.h"
#include "Interpreter/Parser.h"
#include "Interpreter/Statements.h"

#include <string>
#include <unordered_map>
namespace lox {

static Lox lox;
auto Resolver::resolve(std::vector<StmtRef> statements) -> void {
    for (auto statement : statements) {
        resolve(statement);
    }
};

auto Resolver::resolve(StmtRef stmt) -> void {
    stmt->accept(shared_from_this());
}

auto Resolver::resolve(AbstractExpressionRef<Object> expr) -> void {
    expr->accept(shared_from_this());
}

auto Resolver::beginScope() -> void {
    std::unordered_map<std::string, bool> scope;
    m_scopes.push_back(scope);
}

auto Resolver::endScope() -> void { m_scopes.pop_front(); }

auto Resolver::declare(TokenRef name) -> void {
    if (m_scopes.empty()) {
        return;
    }
    auto scope = m_scopes.back();
    if (scope.find(name->getLexeme()) != scope.end()) {
        lox.error(name, "Already variable with this name in this scope.");
    }
    scope.insert({name->getLexeme(), false});
}
auto Resolver::define(TokenRef name) -> void {
    if (m_scopes.empty()) {
        return;
    }
    m_scopes.back().insert({name->getLexeme(), true});
}

auto Resolver::resolveLocal(AbstractExpressionRef<Object> expr, TokenRef name)
    -> void {
    for (int i = m_scopes.size() - 1; i >= 0; i--) {
        if (m_scopes[i].find(name->getLexeme()) != m_scopes[i].end()) {
            m_interpret->resolve(expr, m_scopes.size() - 1 - i);
            return;
        }
    }
}

auto Resolver::resolveFun(FunStmtRef fun, FunctionType type) -> void {
    FunctionType enclosingFun = current_function;
    current_function = type;
    beginScope();
    for (auto param : fun->getParams()) {
        declare(param);
        define(param);
    }
    resolve(fun->getBody());
    endScope();
    current_function = enclosingFun;
}

auto Resolver::visitBlockStmt(BlockStmtRef stmt) -> void {
    beginScope();
    resolve(stmt->getStmt());
    endScope();
    return;
}

auto Resolver::visitVarStmt(VarStmtRef stmt) -> void {
    declare(stmt->getName());
    if (stmt->getInitExpr() != nullptr) {
        resolve(stmt->getInitExpr());
    }
    define(stmt->getName());
    return;
}

auto Resolver::visitFunStmt(FunStmtRef stmt, FunctionType type) -> void {
    declare(stmt->getName());
    define(stmt->getName());
    resolveFun(stmt, FunctionType::FUNCTION);
    return;
}

auto Resolver::visitExpressionStmt(ExpressionStmtRef stmt) -> void {
    resolve(stmt);
    return;
}

auto Resolver::visitIfStmt(IfStmtRef stmt) -> void {
    resolve(stmt->getCondition());
    resolve(stmt->getThen());
    if (stmt->getElse() != nullptr)
        resolve(stmt->getElse());
    return;
}

auto Resolver::visitPrintStmt(PrintStmtRef stmt) -> void {
    resolve(stmt->getExpr());
    return;
}

auto Resolver::visitReturnStmt(ReturnStmtRef stmt) -> void {
    if (current_function == FunctionType::NONE) {
        lox.error(stmt->getKeyword(), "Can't return from top-level code.");
    }
    if (stmt->getValue() != nullptr) {
        if (current_function == FunctionType::INITIALIZER) {
            lox.error(stmt->getKeyword(),
                      "Can't return a value from an initializer.");
        }

        resolve(stmt->getValue());
    }
    return;
}

auto Resolver::visitWhileStmt(WhileStmtRef stmt) -> void {
    resolve(stmt->getCondition());
    resolve(stmt->getBody());
    return;
}

auto Resolver::visitClassStmt(ClassStmtRef stmt) -> void {
    ClassType enclosingClass = current_class;
    current_class = ClassType::CLASS;
    declare(stmt->getName());
    define(stmt->getName());
    if (stmt->getSuper() != nullptr &&
        stmt->getName()->getLexeme() ==
            stmt->getSuper()->getName()->getLexeme()) {
        lox.error(stmt->getSuper()->getName(),
                  "A class can't inherit from itself.");
    }
    if (stmt->getSuper() != nullptr) {
        current_class = ClassType::SUBCLASS;
        resolve(stmt->getSuper());
    }

    if (stmt->getSuper() != nullptr) {
        beginScope();
        m_scopes.back().insert({"super", true});
    }

    beginScope();
    m_scopes.back().insert({"this", true});

    for (auto &method : stmt->getMethods()) {
        FunctionType declaration = FunctionType::METHOD;
        if (method->getName()->getLexeme() == "init") {
            declaration = FunctionType::INITIALIZER;
        }

        resolveFun(method, declaration);
    }
    endScope();
    if (stmt->getSuper() != nullptr)
        endScope();
    current_class = enclosingClass;
    return;
}

/*************************************************************/
/*   Expression      */
/*************************************************************/

auto Resolver::visitVariableExpr(VariableExpressionRef<Object> expr) -> Object {
    if (!m_scopes.empty() &&
        m_scopes.back().at(expr->getName()->getLexeme()) == false) {
        lox.error(expr->getName(),
                  "Can't read local variable in its own initializer.");
    }
    resolveLocal(expr, expr->getName());
    return Object::make_nil_obj();
}

auto Resolver::visitAssignmentExpr(AssignmentExpressionRef<Object> expr)
    -> Object {
    resolve(expr->getValue());
    resolveLocal(expr, expr->getName());
    return Object::make_nil_obj();
}

auto Resolver::visitBinaryExpr(BinaryExpressionRef<Object> expr) -> Object {
    resolve(expr->getLeftExpr());
    resolve(expr->getRightExpr());
    return Object::make_nil_obj();
}

auto Resolver::visitCallExpr(CallExpressionRef<Object> expr) -> Object {
    resolve(expr->getCallee());
    for (auto arg : expr->getArgs()) {
        resolve(arg);
    }
    return Object::make_nil_obj();
}

auto Resolver::visitGroupingExpr(GroupingExpressionRef<Object> expr) -> Object {
    resolve(expr->getExpr());
    return Object::make_nil_obj();
}

auto Resolver::visitLiteralExpr(LiteralExpressionRef<Object> expr) -> Object {
    return Object::make_nil_obj();
}

auto Resolver::visitLogicalExpr(LogicalExpressionRef<Object> expr) -> Object {
    resolve(expr->getLeftExpr());
    resolve(expr->getRightExpr());
    return Object::make_nil_obj();
}

auto Resolver::visitUnaryExpr(UnaryExpressionRef<Object> expr) -> Object {
    resolve(expr->getRightExpr());
    return Object::make_nil_obj();
}

auto Resolver::visitGetExpr(GetExpressionRef<Object> expr) -> Object {
    resolve(expr->getObject());
    return Object::make_nil_obj();
}

auto Resolver::visitSetExpr(SetExpressionRef<Object> expr) -> Object {
    resolve(expr->getValue());
    resolve(expr->getObject());
    return Object::make_nil_obj();
}

auto Resolver::visitThisExpr(ThisExpressionRef<Object> expr) -> Object {
    if (current_class == ClassType::NONE) {
        lox.error(expr->getKeyword(), "Can't use 'this' outside of a class.");
        return Object::make_nil_obj();
    }
    resolveLocal(expr, expr->getKeyword());
    return Object::make_nil_obj();
}

auto Resolver::visitSuperExpr(SuperExpressionRef<Object> expr) -> Object {

    if (current_class == ClassType::NONE) {
        lox.error(expr->getKey(), "Can't use 'super' outside of a class.");
    } else if (current_class == ClassType::SUBCLASS) {

        lox.error(expr->getKey(),
                  "Can't use 'super' in a class with no superclass.");
    }

    resolveLocal(expr, expr->getKey());
    return Object::make_nil_obj();
}

} // namespace lox
