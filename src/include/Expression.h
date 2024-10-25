#pragma once

#include "Token.h"
#include <memory>
namespace lox {

class Token;
using TokenRef = std::shared_ptr<Token>;

template <class R> class AbstractExpression;
template <class R> class BinaryExpression;
template <class R> class UnaryExpression;
template <class R> class LiteralExpression;
template <class R> class GroupingExpression;

template <class R>
using AbstractExpressionRef = std::shared_ptr<AbstractExpression<R>>;
template <class R>
using BinaryExpressionRef = std::shared_ptr<BinaryExpression<R>>;
template <class R>
using UnaryExpressionRef = std::shared_ptr<UnaryExpression<R>>;
template <class R>
using LiteralExpressionRef = std::shared_ptr<LiteralExpression<R>>;
template <class R>
using GroupingExpressionRef = std::shared_ptr<GroupingExpression<R>>;

template <class R> class Visitor {
  public:
    virtual ~Visitor() = default;
    virtual R visitBinaryExpr(BinaryExpressionRef<R> expr) = 0;
    virtual R visitUnaryExpr(UnaryExpressionRef<R> expr) = 0;
    virtual R visitLiteralExpr(LiteralExpressionRef<R> expr) = 0;
    virtual R visitGroupingExpr(GroupingExpressionRef<R> expr) = 0;
};

template <class R> using VisitorRef = std::shared_ptr<Visitor<R>>;

// 表达式基类
template <class R> class AbstractExpression {
  public:
    virtual R accept(VisitorRef<R> visitor) = 0;
    virtual ~AbstractExpression() = default;

  private:
};

template <class R>
class BinaryExpression
    : public AbstractExpression<R>,
      public std::enable_shared_from_this<BinaryExpression<R>> {
  public:
    explicit BinaryExpression(AbstractExpressionRef<R> left,
                              AbstractExpressionRef<R> right, TokenRef opt);
    auto accept(VisitorRef<R> visitor) -> R override;
    auto getOperation() { return m_opt; }
    auto getLeftExpr() { return m_left; }
    auto getRightExpr() { return m_right; }

  private:
    AbstractExpressionRef<R> m_left;
    AbstractExpressionRef<R> m_right;
    TokenRef m_opt;
};

template <class R>
class UnaryExpression
    : public AbstractExpression<R>,
      public std::enable_shared_from_this<UnaryExpression<R>> {
  public:
    explicit UnaryExpression(AbstractExpressionRef<R> right, TokenRef opt);
    auto accept(VisitorRef<R>) -> R override;
    auto getOperation() { return m_opt; }
    auto getRightExpr() { return m_right; }

  private:
    AbstractExpressionRef<R> m_right;
    TokenRef m_opt;
};

template <class R>
class LiteralExpression
    : public AbstractExpression<R>,
      public std::enable_shared_from_this<LiteralExpression<R>> {
  public:
    explicit LiteralExpression(R literal);
    R accept(VisitorRef<R>) override;
    R getValue() { return m_literal; }

  private:
    R m_literal;
};

template <class R>
class GroupingExpression
    : public AbstractExpression<R>,
      public std::enable_shared_from_this<GroupingExpression<R>> {
  public:
    explicit GroupingExpression(AbstractExpressionRef<R> expr);
    R accept(VisitorRef<R>) override;
    auto getExpr() { return m_expr; }

  private:
    AbstractExpressionRef<R> m_expr;
};
} // namespace lox
