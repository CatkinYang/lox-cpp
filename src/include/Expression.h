#pragma once
#include "Object.h"
#include "Token.h"
#include <memory>
namespace lox {

class Token;
using TokenRef = std::shared_ptr<Token>;

class AbstractExpression;
using AbstractExpressionRef = std::shared_ptr<AbstractExpression>;

class AbstractExpression {
  public:
    virtual ~AbstractExpression() = default;

  private:
};

class BinaryExpression : AbstractExpression {
  public:
    BinaryExpression(AbstractExpressionRef left, AbstractExpressionRef right,
                     TokenRef opt);

  private:
    AbstractExpressionRef m_left;
    AbstractExpressionRef m_right;
    TokenRef m_opt;
};

class UnaryExpression : AbstractExpression {
  public:
    UnaryExpression(AbstractExpressionRef right, TokenRef opt);

  private:
    AbstractExpressionRef m_right;
    TokenRef m_opt;
};

class LiteralExpression : AbstractExpression {
  public:
    LiteralExpression(ObjectRef literal) : m_literal(literal) {}

  private:
    ObjectRef m_literal;
};

class GroupingExpression : AbstractExpression {
  public:
  private:
};

} // namespace lox
