#pragma once

#include "Object.h"
#include "Tokentype.h"
#include <memory>
#include <string>
namespace lox {

class Token;
using TokenRef = std::shared_ptr<Token>;

class Token {
  public:
    Token(TokenType type, std::string lexeme, ObjectRef literal, int line) {
        m_lexeme = lexeme;
        m_type = type;
        m_line = line;
        m_literal = literal;
    }
    auto toString() -> std::string;
    auto getType() -> TokenType;
    auto getLine() -> int;
    auto getLiteral() -> ObjectRef;
    auto getLexeme() -> std::string;

  private:
    TokenType m_type;     // token 种类
    std::string m_lexeme; // 词素，源代码的原始字符串
    ObjectRef m_literal;  // 子面量
    int m_line;           // 行号
};

} // namespace lox
