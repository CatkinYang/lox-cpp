#include "include/Expression.h"
#include <algorithm>

namespace lox {

BinaryExpression::BinaryExpression(AbstractExpressionRef left,
                                   AbstractExpressionRef right, TokenRef opt) {
    m_left = std::move(left);
    m_right = std::move(right);
    m_opt = std::move(opt);
}

UnaryExpression::UnaryExpression(AbstractExpressionRef right, TokenRef opt) {
    m_right = std::move(right);
    m_opt = std::move(opt);
}

} // namespace lox
