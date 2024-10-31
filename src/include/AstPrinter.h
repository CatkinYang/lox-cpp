#pragma once
#include "Expression.h"
#include "Object.h"
#include <memory>
#include <string>
namespace lox {

class AstPrinter : public Visitor<std::string>,
                   public std::enable_shared_from_this<AstPrinter> {
  public:
    std::string print(const AbstractExpressionRef<std::string> &expr);
    std::string visitLiteralExpr(LiteralExpressionRef<std::string> expr);
    std::string visitBinaryExpr(BinaryExpressionRef<std::string> expr);
    std::string visitUnaryExpr(UnaryExpressionRef<std::string> expr);
    std::string visitGroupingExpr(GroupingExpressionRef<std::string> expr);
    std::string visitVariableExpr(VariableExpressionRef<std::string> expr);
    std::string visitAssignmentExpr(AssignmentExpressionRef<std::string> expr);
    std::string visitLogicalExpr(LogicalExpressionRef<std::string> expr);
    std::string visitCallExpr(CallExpressionRef<std::string> expr);
    std::string visitGetExpr(GetExpressionRef<std::string> expr) { return ""; }
    std::string visitSetExpr(SetExpressionRef<std::string> expr) { return ""; }
    std::string visitThisExpr(ThisExpressionRef<std::string> expr) {
        return "";
    }
    std::string visitSuperExpr(SuperExpressionRef<std::string> expr) {
        return "";
    }
};

} // namespace lox
