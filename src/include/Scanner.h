#pragma once
#include "Token.h"
#include <string>
#include <vector>
namespace lox {

// 将输入的字符流切分成token保存到m_tokens中

class Scanner {
  public:
    Scanner(std::string source) { m_source = source; }
    // help funcitons
    // 判断是否扫描到了source的末尾
    auto isAtEnd() -> bool;
    // 获取当前字符，增加current
    auto advance() -> char;
    auto addToken(TokenType type) -> void;
    auto addToken(TokenType type, ObjectRef literal) -> void;
    // 判断当前的current指向的字符是否和expected一致，
    // 不一致的话或者已经在末尾就会返回false
    // 否则就会增加current并且返回true
    auto match(char expected) -> bool;
    // 如果是末尾就返回\0，否则输出当前字符
    auto peek() -> char;
    auto peekNext() -> char;
    // 当扫描器扫描到"时会调用去获取 string字面量
    auto get_string() -> void;
    // 当扫描器扫描到digit时会调用去获取 digit字面量
    auto get_number() -> void;
    // 获取关键字
    auto identifier() -> void;

    auto scanToken() -> void;
    auto scanTokens() -> std::vector<TokenRef>;

    auto getTokens() -> std::vector<TokenRef> { return m_tokens; }

  private:
    std::string m_source;           // 输入流
    std::vector<TokenRef> m_tokens; // 序列
    int m_start = 0;                // 指向被扫描的string中的第一个字符
    int m_current = 0;              // 指向当前正在处理的字符
    int m_line = 1;                 // current所在源文件的行数
};

} // namespace lox
