#include "include/Token.h"
#include "include/Tokentype.h"

namespace lox {
auto Token::toString() -> std::string {
    auto type = lox::tokenTypeToString[m_type];
    auto literal = m_literal.toString();
    std::string res = type + " " + m_lexeme + " " + literal;
    return res;
}
auto Token::getType() -> TokenType { return m_type; }
auto Token::getLine() -> int { return m_line; }
auto Token::getLiteral() -> Object { return m_literal; }
auto Token::getLexeme() -> std::string { return m_lexeme; }
} // namespace lox
