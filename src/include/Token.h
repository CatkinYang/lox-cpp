#pragma once
#include "Object.h"
#include "Tokentype.h"
#include <string>
#include <string_view>
namespace lox {

class Token {
  public:
    Token(lox::TokenType type, std::string lexeme, Object literal, int line) {
        m_lexeme = lexeme;
        m_type = type;
        m_line = line;
        m_literal = literal;
    }
    auto toString() -> std::string;

  private:
    TokenType m_type;     // token 种类
    std::string m_lexeme; // 词素
    Object m_literal;
    int m_line; // 行号
};

} // namespace lox
