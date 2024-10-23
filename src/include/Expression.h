#pragma once
#include "Token.h"
#include <memory>
namespace lox {
class AbstractExpression {
  public:
    virtual ~AbstractExpression() = default;

  private:
};

class BinaryExpression : AbstractExpression {
  public:
  private:
    std::unique_ptr<AbstractExpression> left;
    std::unique_ptr<AbstractExpression> right;
    std::unique_ptr<Token> opt;
};
} // namespace lox
