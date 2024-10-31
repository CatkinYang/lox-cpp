#include "Interpreter/Tokentype.h"

#include <string>
#include <unordered_map>

namespace lox {

// 宏定义：生成枚举值对应的字符串对
#define TOKEN_TYPE_MAP(XX)                                                     \
    XX(LEFT_PAREN)                                                             \
    XX(RIGHT_PAREN)                                                            \
    XX(LEFT_BRACE)                                                             \
    XX(RIGHT_BRACE)                                                            \
    XX(COMMA)                                                                  \
    XX(DOT)                                                                    \
    XX(MINUS)                                                                  \
    XX(PLUS)                                                                   \
    XX(SEMICOLON)                                                              \
    XX(SLASH)                                                                  \
    XX(STAR)                                                                   \
    XX(BANG)                                                                   \
    XX(BANG_EQUAL)                                                             \
    XX(EQUAL)                                                                  \
    XX(EQUAL_EQUAL)                                                            \
    XX(GREATER)                                                                \
    XX(GREATER_EQUAL)                                                          \
    XX(LESS)                                                                   \
    XX(LESS_EQUAL)                                                             \
    XX(IDENTIFIER)                                                             \
    XX(STRING)                                                                 \
    XX(NUMBER)                                                                 \
    XX(AND)                                                                    \
    XX(CLASS)                                                                  \
    XX(ELSE)                                                                   \
    XX(FALSE)                                                                  \
    XX(FUN)                                                                    \
    XX(FOR)                                                                    \
    XX(IF)                                                                     \
    XX(NIL)                                                                    \
    XX(OR)                                                                     \
    XX(PRINT)                                                                  \
    XX(RETURN)                                                                 \
    XX(SUPER)                                                                  \
    XX(THIS)                                                                   \
    XX(TRUE)                                                                   \
    XX(VAR)                                                                    \
    XX(WHILE)                                                                  \
    XX(EOF_TOKEN)

// 自动生成 tokenTypeToString 函数
#define GENERATE_TOKEN_STRING_CASE(name)                                       \
    case name:                                                                 \
        return #name;

std::string TokenTypeToString(TokenType type) {
    switch (type) {
        TOKEN_TYPE_MAP(GENERATE_TOKEN_STRING_CASE)
    default:
        return "UNKNOWN";
    }
}
} // namespace lox
