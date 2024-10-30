#pragma once

#include "Expression.h"
#include "Interpreter.h"
#include "Object.h"
#include "Statements.h"
#include "Token.h"
#include <deque>
#include <memory>
#include <unordered_map>
#include <vector>
namespace lox {

enum class FunctionType { NONE, FUNCTION };

class Resolver : public StmtVisitor,
                 public Visitor<Object>,
                 public std::enable_shared_from_this<Resolver> {
  public:
    Resolver(InterpreterRef interpret) : m_interpret(interpret) {};

    auto resolve(std::vector<StmtRef> statement) -> void;
    auto resolve(StmtRef stmt) -> void;
    auto resolve(AbstractExpressionRef<Object> expr) -> void;

    auto beginScope() -> void;
    auto endScope() -> void;

    auto declare(TokenRef name) -> void;
    auto define(TokenRef name) -> void;

    auto resolveLocal(AbstractExpressionRef<Object> expr, TokenRef name)
        -> void;
    auto resolveFun(FunStmtRef fun, FunctionType type) -> void;

    auto visitBlockStmt(BlockStmtRef stmt) -> void;
    auto visitVarStmt(VarStmtRef stmt) -> void;
    auto visitFunStmt(FunStmtRef stmt, FunctionType type) -> void;
    auto visitExpressionStmt(ExpressionStmtRef stmt) -> void;
    auto visitIfStmt(IfStmtRef stmt) -> void;
    auto visitPrintStmt(PrintStmtRef stmt) -> void;
    auto visitReturnStmt(ReturnStmtRef stmt) -> void;
    auto visitWhileStmt(WhileStmtRef stmt) -> void;

    auto visitLiteralExpr(LiteralExpressionRef<Object> expr) -> Object;
    auto visitGroupingExpr(GroupingExpressionRef<Object> expr) -> Object;
    auto visitUnaryExpr(UnaryExpressionRef<Object> expr) -> Object;
    auto visitBinaryExpr(BinaryExpressionRef<Object> expr) -> Object;
    auto visitLogicalExpr(LogicalExpressionRef<Object> expr) -> Object;
    auto visitCallExpr(CallExpressionRef<Object> expr) -> Object;
    auto visitVariableExpr(VariableExpressionRef<Object> expr) -> Object;
    auto visitAssignmentExpr(AssignmentExpressionRef<Object> expr) -> Object;

  private:
    InterpreterRef m_interpret;
    std::deque<std::unordered_map<std::string, bool>> m_scopes;
    FunctionType current_function = FunctionType::NONE;
};

} // namespace lox
