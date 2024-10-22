#pragma once
#include "Lox.h"
#include "Token.h"
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace lox {
class Scanner {
  public:
    Scanner(std::string source) { m_source = source; }
    // help funcitons
    // 判断是否扫描到了source的末尾
    auto isAtEnd() -> bool;
    // 获取当前字符，增加current并返回
    auto advance() -> char;
    // 获取当前token的文本，并且创建一个新的token
    auto addToken(TokenType type) -> void;

    auto addToken(TokenType type, Object literal) -> void;
    // 判断当前的current指向的字符是否和expected一致，不一致的话或者已经在末尾就会返回false
    // 否则就会增加current并且返回true
    auto match(char expected) -> bool;
    // 如果是末尾就返回\0，否则输出当前字符
    auto peek() -> char;
    auto peekNext() -> char;

    auto get_string() -> void;
    auto get_number() -> void;
    auto identifier() -> void;

    auto scanToken() -> void;
    auto scanTokens() -> std::list<std::unique_ptr<Token>>;

  private:
    std::string m_source;                       // 输入流
    std::list<std::unique_ptr<Token>> m_tokens; // 序列
    int m_start = 0;   // 指向被扫描的token中的第一个字符
    int m_current = 0; // 指向当前正在处理的字符
    int m_line = 1;    // current所在源文件的行数
};

} // namespace lox
