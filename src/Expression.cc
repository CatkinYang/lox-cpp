#include "Expression.h"
#include "Object.h"
#include <string>

namespace lox {

template <class R>
auto BinaryExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitBinaryExpr(this->shared_from_this());
};

template <class R> auto UnaryExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitUnaryExpr(this->shared_from_this());
};

template <class R>
auto LiteralExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitLiteralExpr(this->shared_from_this());
};

template <class R>
auto GroupingExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitGroupingExpr(this->shared_from_this());
};

template <class R>
auto VariableExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitVariableExpr(this->shared_from_this());
};

template <class R>
auto AssignmentExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitAssignmentExpr(this->shared_from_this());
};

template <class R>
auto LogicalExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitLogicalExpr(this->shared_from_this());
};

template <class R> auto CallExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitCallExpr(this->shared_from_this());
};

template <class R> auto GetExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitGetExpr(this->shared_from_this());
};

template <class R> auto SetExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitSetExpr(this->shared_from_this());
};

template <class R> auto ThisExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitThisExpr(this->shared_from_this());
};

template <class R> auto SuperExpression<R>::accept(VisitorRef<R> visitor) -> R {
    return visitor->visitSuperExpr(this->shared_from_this());
};

template class LiteralExpression<std::string>;
template class UnaryExpression<std::string>;
template class BinaryExpression<std::string>;
template class GroupingExpression<std::string>;
template class VariableExpression<std::string>;
template class AssignmentExpression<std::string>;
template class LogicalExpression<std::string>;
template class CallExpression<std::string>;
template class GetExpression<std::string>;
template class SetExpression<std::string>;
template class ThisExpression<std::string>;
template class SuperExpression<std::string>;

template class AbstractExpression<Object>;
template class LiteralExpression<Object>;
template class UnaryExpression<Object>;
template class BinaryExpression<Object>;
template class GroupingExpression<Object>;
template class VariableExpression<Object>;
template class AssignmentExpression<Object>;
template class LogicalExpression<Object>;
template class CallExpression<Object>;
template class GetExpression<Object>;
template class SetExpression<Object>;
template class ThisExpression<Object>;
template class SuperExpression<Object>;

} // namespace lox
