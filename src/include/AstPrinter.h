#pragma once
#include "Expression.h"
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
};
} // namespace lox