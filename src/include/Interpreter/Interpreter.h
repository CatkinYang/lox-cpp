#pragma once
#include "Environment.h"
#include "Expression.h"
#include "Object.h"
#include "Statements.h"
#include "Token.h"

#include <memory>
#include <string>

namespace lox {

class Interpreter;
using InterpreterRef = std::shared_ptr<Interpreter>;

// 解释器
class Interpreter : public Visitor<Object>,
                    public StmtVisitor,
                    public std::enable_shared_from_this<Interpreter> {
  public:
    Interpreter();

    auto visitLiteralExpr(LiteralExpressionRef<Object> expr) -> Object;
    auto visitGroupingExpr(GroupingExpressionRef<Object> expr) -> Object;
    auto visitUnaryExpr(UnaryExpressionRef<Object> expr) -> Object;
    auto visitBinaryExpr(BinaryExpressionRef<Object> expr) -> Object;
    auto visitVariableExpr(VariableExpressionRef<Object> expr) -> Object;
    auto visitAssignmentExpr(AssignmentExpressionRef<Object> expr) -> Object;
    auto visitLogicalExpr(LogicalExpressionRef<Object> expr) -> Object;
    auto visitCallExpr(CallExpressionRef<Object> expr) -> Object;
    auto visitGetExpr(GetExpressionRef<Object> expr) -> Object;
    auto visitSetExpr(SetExpressionRef<Object> expr) -> Object;
    auto visitThisExpr(ThisExpressionRef<Object> expr) -> Object;
    auto visitSuperExpr(SuperExpressionRef<Object> expr) -> Object;

    auto evaluate(AbstractExpressionRef<Object> expr) -> Object;

    auto visitExpressionStmt(ExpressionStmtRef stmt) -> void;
    auto visitPrintStmt(PrintStmtRef stmt) -> void;
    auto visitVarStmt(VarStmtRef stmt) -> void;
    auto visitBlockStmt(BlockStmtRef stmt) -> void;
    auto visitIfStmt(IfStmtRef stmt) -> void;
    auto visitWhileStmt(WhileStmtRef stmt) -> void;
    auto visitFunStmt(FunStmtRef stmt) -> void;
    auto visitReturnStmt(ReturnStmtRef stmt) -> void;
    auto visitClassStmt(ClassStmtRef stmt) -> void;

    auto evaluate(StmtRef stmt) -> void;

    auto execute(StmtRef stmt) -> void;
    auto executeBlock(std::vector<StmtRef> statements, EnvironmentRef env)
        -> void;

    auto resolve(AbstractExpressionRef<Object> expr, int depth) -> void;

    auto lookUpVariable(TokenRef name, AbstractExpressionRef<Object> expr)
        -> Object;

    auto isTruthy(Object obj) -> bool;
    auto isEqual(Object a, Object b) -> bool;

    auto checkNumberOperand(TokenRef operation, Object operand) -> void;
    auto checkNumberOperands(TokenRef operation, Object left, Object right)
        -> void;

    auto interpret(std::vector<StmtRef> statements) -> void;
    auto stringify(Object obj) -> std::string;

    auto getEnvironment() { return m_env; }
    auto getGlobals() { return globals; }

    EnvironmentRef globals;
    EnvironmentRef m_env;
    std::unordered_map<AbstractExpressionRef<Object>, int> m_locals;
};

} // namespace lox
