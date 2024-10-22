#include "include/Token.h"
#include "include/Tokentype.h"

namespace lox {
auto Token::toString() -> std::string {
    auto type = lox::tokenTypeToString[m_type];
    auto literal = m_literal.toString();
    std::string res = type + " " + m_lexeme + " " + literal;
    return res;
}
} // namespace lox
