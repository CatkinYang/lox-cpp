#include "include/Expression.h"

namespace lox {

template <class R>
BinaryExpression<R>::BinaryExpression(AbstractExpressionRef<R> left,
                                      AbstractExpressionRef<R> right,
                                      TokenRef opt) {
    m_left = left;
    m_right = right;
    m_opt = opt;
}
template <class R>
auto BinaryExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitBinaryExpr(this->shared_from_this());
};

template <class R>
UnaryExpression<R>::UnaryExpression(AbstractExpressionRef<R> right,
                                    TokenRef opt) {
    m_right = right;
    m_opt = opt;
}
template <class R> auto UnaryExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitUnaryExpr(this->shared_from_this());
};

template <class R> LiteralExpression<R>::LiteralExpression(R literal) {
    m_literal = literal;
}
template <class R>
auto LiteralExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitLiteralExpr(this->shared_from_this());
};

template <class R>
GroupingExpression<R>::GroupingExpression(AbstractExpressionRef<R> expr) {
    m_expr = expr;
}
template <class R>
auto GroupingExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitGroupingExpr(this->shared_from_this());
};

template class LiteralExpression<std::string>;
template class UnaryExpression<std::string>;
template class BinaryExpression<std::string>;
template class GroupingExpression<std::string>;

} // namespace lox
