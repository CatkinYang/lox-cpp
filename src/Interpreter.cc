#include "Interpreter.h"
#include "Environment.h"
#include "Lox.h"
#include "Object.h"
#include "RuntimeError.h"
#include "Statements.h"
#include "Tokentype.h"
#include <iostream>
#include <memory>

namespace lox {

static Lox lox;

/*******************************************************************/
/*                Expression    */
/*******************************************************************/

auto Interpreter::visitLiteralExpr(LiteralExpressionRef<Object> expr)
    -> Object {
    return expr->getValue();
}

auto Interpreter::visitGroupingExpr(GroupingExpressionRef<Object> expr)
    -> Object {
    return evaluate(expr->getExpr());
}

auto Interpreter::visitUnaryExpr(UnaryExpressionRef<Object> expr) -> Object {
    auto right = evaluate(expr->getRightExpr());
    switch (expr->getOperation()->getType()) {
    case MINUS:
        return Object::make_num_obj(-stod(right.toString()));
    case BANG:
        return Object::make_bool_obj(!isTruthy(right));
    default:
        return Object::make_nil_obj();
    }
}

auto Interpreter::visitBinaryExpr(BinaryExpressionRef<Object> expr) -> Object {
    auto left = evaluate(expr->getLeftExpr());
    auto right = evaluate(expr->getRightExpr());
    auto opt = expr->getOperation();
    bool result_bool = false;
    std::string result_str = "";
    double result_num = 0;
    Object foo;
    switch (expr->getOperation()->getType()) {
    case GREATER:
        checkNumberOperands(opt, left, right);
        result_bool = left.getNum() > right.getNum();
        return Object::make_bool_obj(result_bool);
    case GREATER_EQUAL:
        checkNumberOperands(opt, left, right);
        result_bool = left.getNum() >= right.getNum();
        return Object::make_bool_obj(result_bool);
    case LESS:
        checkNumberOperands(opt, left, right);
        result_bool = left.getNum() < right.getNum();
        return Object::make_bool_obj(result_bool);
    case LESS_EQUAL:
        checkNumberOperands(opt, left, right);
        result_bool = left.getNum() <= right.getNum();
        return Object::make_bool_obj(result_bool);
    case MINUS:
        checkNumberOperands(opt, left, right);
        result_num = left.getNum() - right.getNum();
        return Object::make_num_obj(result_num);
    case PLUS:
        if (left.getType() == Object::Object_num &&
            right.getType() == Object::Object_num) {
            return Object::make_num_obj(left.getNum() + right.getNum());
        } else if (left.getType() == Object::Object_str &&
                   right.getType() == Object::Object_str) {
            return Object::make_str_obj(left.getStr() + right.getStr());
        }
        throw RuntimeError(opt, "Operands must be two numbers or two strings.");
    case SLASH:
        checkNumberOperands(opt, left, right);
        result_num = left.getNum() / right.getNum();
        return Object::make_num_obj(result_num);
    case STAR:
        checkNumberOperands(opt, left, right);
        result_num = left.getNum() * right.getNum();
        return Object::make_num_obj(result_num);
    case BANG_EQUAL:
        return Object::make_bool_obj(!isEqual(left, right));
    case EQUAL_EQUAL:
        return Object::make_bool_obj(isEqual(left, right));
    default:
        return Object::make_nil_obj();
    }
    return Object::make_nil_obj();
}

auto Interpreter::visitVariableExpr(VariableExpressionRef<Object> expr)
    -> Object {
    return *m_env->get(expr->getName()).get();
}

auto Interpreter::visitAssignmentExpr(AssignmentExpressionRef<Object> expr)
    -> Object {
    auto value = evaluate(expr->getValue());
    auto valueRef = std::make_shared<Object>(value);
    m_env->assign(expr->getName(), valueRef);
    return value;
}

auto Interpreter::evaluate(AbstractExpressionRef<Object> expr) -> Object {
    return expr->accept(shared_from_this());
}

/*******************************************************************/
/*         Statements      */
/*******************************************************************/

auto Interpreter::visitExpressionStmt(ExpressionStmtRef stmt) -> void {
    evaluate(stmt->getExpr());
    return;
}

auto Interpreter::visitPrintStmt(PrintStmtRef stmt) -> void {
    Object value = evaluate(stmt->getExpr());
    std::cout << stringify(value) << std::endl;
    return;
}

auto Interpreter::visitVarStmt(VarStmtRef stmt) -> void {
    ObjectRef value = nullptr;
    if (stmt->getInitExpr() != nullptr) {
        value = std::make_shared<Object>(evaluate(stmt->getInitExpr()));
    }
    m_env->define(stmt->getName()->getLexeme(), value);
    return;
}

auto Interpreter::visitBlockStmt(BlockStmtRef stmt) -> void {
    auto new_env = std::make_shared<Environment>(getEnvironment());
    executeBlock(stmt->getStmt(), new_env);
    return;
}

auto Interpreter::execute(StmtRef stmt) -> void {
    stmt->accept(shared_from_this());
}

auto Interpreter::executeBlock(std::vector<StmtRef> statements,
                               EnvironmentRef env) -> void {
    auto prev_env = getEnvironment();
    m_env = env;
    for (auto stmt : statements) {
        execute(stmt);
        m_env = prev_env;
    }
}

auto Interpreter::evaluate(StmtRef stmt) -> void {
    stmt->accept(shared_from_this());
}

/*******************************************************************/

auto Interpreter::isTruthy(Object obj) -> bool {
    if (obj.getType() == Object::Object_nil) {
        return false;
    }
    if (obj.getType() == Object::Object_bool) {
        return obj.getBool();
    }
    return true;
}

auto Interpreter::isEqual(Object a, Object b) -> bool {
    if (a.getType() == Object::Object_nil &&
        b.getType() == Object::Object_nil) {
        return true;
    }
    if (a.getType() == Object::Object_nil)
        return false;
    if (a.getType() == b.getType()) {
        switch (a.getType()) {
        case Object::Object_bool:
            return a.getBool() == b.getBool();
        case Object::Object_num:
            return a.getNum() == b.getNum();
        case Object::Object_str:
            return a.getStr() == b.getStr();
        default:
            return false;
        }
    }
    return false;
}

auto Interpreter::checkNumberOperand(TokenRef operation, Object operand)
    -> void {
    if (operand.getType() == Object::Object_num)
        return;
    throw RuntimeError(operation, "Operand must be a number.");
}
auto Interpreter::checkNumberOperands(TokenRef operation, Object left,
                                      Object right) -> void {
    if (left.getType() == Object::Object_num &&
        right.getType() == Object::Object_num)
        return;
    throw RuntimeError(operation, "Operand must be a number.");
}

auto Interpreter::interpret(std::vector<StmtRef> statements) -> void {
    try {
        for (auto statement : statements) {
            execute(statement);
        }
    } catch (RuntimeError error) {
        lox.runtimeError(error);
    }
}

bool endsWith(std::string str, std::string suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

auto Interpreter::stringify(Object obj) -> std::string {
    if (obj.getType() == Object::Object_nil)
        return "nil";
    if (obj.getType() == Object::Object_str) {
        auto text = obj.toString();
        std::string check_str = ".000000";
        if (endsWith(text, check_str)) {
            return text.erase(text.size() - check_str.size());
        }
        return text;
    }
    return obj.toString();
}

} // namespace lox
