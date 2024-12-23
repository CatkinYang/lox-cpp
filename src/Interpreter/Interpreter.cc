#include "Interpreter/Interpreter.h"
#include "Interpreter/Environment.h"
#include "Interpreter/Lox.h"
#include "Interpreter/LoxCallable.h"
#include "Interpreter/LoxFunction.h"
#include "Interpreter/LoxInstance.h"
#include "Interpreter/Object.h"
#include "Interpreter/Return.h"
#include "Interpreter/RuntimeError.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace lox {

static Lox lox;
Interpreter::Interpreter() {}

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
    // return *m_env->get(expr->getName()).get();
    return lookUpVariable(expr->getName(), expr);
}

auto Interpreter::visitAssignmentExpr(AssignmentExpressionRef<Object> expr)
    -> Object {
    auto value = evaluate(expr->getValue());
    auto valueRef = std::make_shared<Object>(value);

    auto iter = m_locals.find(expr);
    if (iter != m_locals.end()) {
        m_env->assignAt(iter->second, expr->getName(), valueRef);
    } else {
        globals->assign(expr->getName(), valueRef);
    }
    return value;
}

auto Interpreter::visitLogicalExpr(LogicalExpressionRef<Object> expr)
    -> Object {
    auto left = evaluate(expr->getLeftExpr());
    if (expr->getOperation()->getType() == OR) {
        if (isTruthy(left))
            return left;
    } else {
        if (!isTruthy(left))
            return left;
    }
    return evaluate(expr->getRightExpr());
}

auto Interpreter::visitCallExpr(CallExpressionRef<Object> expr) -> Object {
    auto callee = evaluate(expr->getCallee());
    std::vector<ObjectRef> arguments;
    for (auto arg : expr->getArgs()) {
        arguments.push_back(std::make_shared<Object>(evaluate(arg)));
    }
    //  检查callee是否是LoxCallable类的对象
    if (callee.getType() != Object::Object_fun) {
        throw RuntimeError(expr->getParen(),
                           "Can only call functions and classes.");
    }

    LoxCallableRef function;

    if (arguments.size() != (size_t)function->arity()) {
        throw RuntimeError(expr->getParen(),
                           "Expected " + std::to_string(function->arity()) +
                               " arguments but got " +
                               std::to_string(arguments.size()) + ".");
    }
    return *function->call(shared_from_this(), arguments).get();
}

auto Interpreter::visitGetExpr(GetExpressionRef<Object> expr) -> Object {
    auto obj = evaluate(expr->getObject());
    if (obj.getType() == Object::Object_instance) {
        auto name = expr->getName();
        return *((obj.getInstance())->get(expr->getName())).get();
        // return res;
    }

    throw RuntimeError(expr->getName(), "Only instances have properties.");
}

auto Interpreter::visitSetExpr(SetExpressionRef<Object> expr) -> Object {
    auto object = evaluate(expr->getObject());

    if (!(object.getType() != Object::Object_instance)) {
        throw RuntimeError(expr->getName(), "Only instances have fields.");
    }
    auto value = evaluate(expr->getValue());
    auto value_obj = std::make_shared<Object>(value);
    object.getInstance()->set(expr->getName(), value_obj);
    return value;
}

auto Interpreter::evaluate(AbstractExpressionRef<Object> expr) -> Object {
    return expr->accept(shared_from_this());
}

auto Interpreter::lookUpVariable(TokenRef name,
                                 AbstractExpressionRef<Object> expr) -> Object {
    auto iter = m_locals.find(expr);
    if (iter != m_locals.end()) {
        return *m_env->getAt(iter->second, name->getLexeme()).get();
    } else {
        return *globals->get(name).get();
    }
}

auto Interpreter::visitThisExpr(ThisExpressionRef<Object> expr) -> Object {
    return lookUpVariable(expr->getKeyword(), expr);
}

auto Interpreter::visitSuperExpr(SuperExpressionRef<Object> expr) -> Object {
    auto distance = m_locals.at(expr);
    auto superclass_obj = m_env->getAt(distance, "super");
    auto superclass = superclass_obj->getClass();

    auto instance_obj = m_env->getAt(distance - 1, "this");
    auto instance = instance_obj->getInstance();

    auto method_obj = superclass->findMethod(expr->getMethod()->getLexeme());

    if (method_obj == nullptr) {
        throw RuntimeError(expr->getMethod(),
                           "Undefined property '" +
                               expr->getMethod()->getLexeme() + "'.");
    }

    auto res = method_obj->bind(instance);
    return Object::make_fun_obj(method_obj);
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

auto Interpreter::visitIfStmt(IfStmtRef stmt) -> void {
    if (isTruthy(evaluate(stmt->getCondition()))) {
        execute(stmt->getThen());
    } else if (stmt->getElse() != nullptr) {
        execute(stmt->getElse());
    }
    return;
}

auto Interpreter::visitWhileStmt(WhileStmtRef stmt) -> void {
    if (isTruthy(evaluate(stmt->getCondition()))) {
        execute(stmt->getBody());
    }
    return;
}

auto Interpreter::visitFunStmt(FunStmtRef stmt) -> void {
    auto function = std::make_shared<LoxFunction>(stmt, m_env, false);
    auto fun_obj =
        Object::make_fun_obj(std::dynamic_pointer_cast<LoxCallable>(function));
    auto fun_obj_ref = std::make_shared<Object>(fun_obj);
    m_env->define(stmt->getName()->getLexeme(), fun_obj_ref);
    return;
}

auto Interpreter::visitReturnStmt(ReturnStmtRef stmt) -> void {
    Object value;
    if (stmt->getValue() == nullptr) {
        value = evaluate(stmt->getValue());
    }
    throw ReturnError(value);
}

auto Interpreter::visitClassStmt(ClassStmtRef stmt) -> void {
    ObjectRef superclass_obj = nullptr;

    if (stmt->getSuper() != nullptr) {
        superclass_obj = std::make_shared<Object>(evaluate(stmt->getSuper()));
        if (superclass_obj->getType() != Object::Object_class) {
            throw RuntimeError(stmt->getSuper()->getName(),
                               "Superclass must be a class.");
        }
    }

    m_env->define(stmt->getName()->getLexeme(), nullptr);

    if (stmt->getSuper() != nullptr) {
        m_env = std::make_shared<Environment>(m_env);
        m_env->define(stmt->getName()->getLexeme(), nullptr);
    }

    std::unordered_map<std::string, LoxFunctionRef> methods;
    for (auto &method : stmt->getMethods()) {
        bool tmp = (method->getName()->getLexeme() == "init");
        auto fun = std::make_shared<LoxFunction>(method, m_env, tmp);
        methods.insert({method->getName()->getLexeme(), fun});
    }

    auto klass = std::make_shared<LoxClass>(
        stmt->getName()->getLexeme(), superclass_obj->getClass(), methods);
    auto klass_obj = Object::make_class_obj(klass);
    auto klass_obj_ref = std::make_shared<Object>(klass_obj);

    if (superclass_obj != nullptr) {
        m_env = m_env->getEnclosing();
    }

    m_env->assign(stmt->getName(), klass_obj_ref);
    return;
}

auto Interpreter::evaluate(StmtRef stmt) -> void {
    stmt->accept(shared_from_this());
}

/*******************************************************************/
/*         */
/*******************************************************************/

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

auto Interpreter::resolve(AbstractExpressionRef<Object> expr, int depth)
    -> void {
    m_locals.insert({expr, depth});
}

/*******************************************************************/
/*         */
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
