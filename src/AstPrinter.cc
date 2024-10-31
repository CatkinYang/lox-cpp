#include "AstPrinter.h"
#include "Expression.h"
#include <memory>
#include <string>

namespace lox {

std::string AstPrinter::print(const AbstractExpressionRef<std::string> &expr) {
    return expr->accept(
        std::static_pointer_cast<Visitor<std::string>>(shared_from_this()));
}

std::string
AstPrinter::visitLiteralExpr(LiteralExpressionRef<std::string> expr) {
    auto value = expr->getValue();
    if (value == "")
        return "nil";
    return value;
}

std::string AstPrinter::visitBinaryExpr(BinaryExpressionRef<std::string> expr) {
    std::string res = "(";
    auto lexeme = expr->getOperation()->getLexeme();
    res += lexeme;
    auto left = expr->getLeftExpr()->accept(
        std::static_pointer_cast<Visitor<std::string>>(shared_from_this()));
    auto right = expr->getRightExpr()->accept(
        std::static_pointer_cast<Visitor<std::string>>(shared_from_this()));
    res += " " + left + " " + right + ")";
    return res;
}

std::string AstPrinter::visitUnaryExpr(UnaryExpressionRef<std::string> expr) {
    std::string res = "(";
    auto lexeme = expr->getOperation()->getLexeme();
    res += lexeme + " ";
    auto right = expr->getRightExpr()->accept(
        std::static_pointer_cast<Visitor<std::string>>(shared_from_this()));
    res += right;
    res += ")";
    return res;
}

std::string
AstPrinter::visitGroupingExpr(GroupingExpressionRef<std::string> expr) {
    std::string res = "(group ";
    auto exp = expr->getExpr()->accept(
        std::static_pointer_cast<Visitor<std::string>>(shared_from_this()));
    res += exp;
    res += ")";
    return res;
}

std::string
AstPrinter::visitVariableExpr(VariableExpressionRef<std::string> expr) {
    return "";
}

std::string
AstPrinter::visitAssignmentExpr(AssignmentExpressionRef<std::string> expr) {
    return "";
}

std::string
AstPrinter::visitLogicalExpr(LogicalExpressionRef<std::string> expr) {
    return "";
}

std::string AstPrinter::visitCallExpr(CallExpressionRef<std::string> expr) {
    return "";
}

} // namespace lox
