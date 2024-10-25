#include "include/Scanner.h"
#include "Lox.h"
#include "include/Object.h"
#include "include/Token.h"
#include "include/Tokentype.h"
#include <cctype>
#include <memory>
#include <string>

namespace lox {

static Lox lox;
auto Scanner::isAtEnd() -> bool {
    return m_current >= static_cast<int>(m_source.size());
}

auto Scanner::advance() -> char {
    m_current++;
    return m_source[m_current - 1];
}

// TODO: 根据type生成对应的token对象加入m_tokens中
auto Scanner::addToken(TokenType type) -> void {
    auto literal_obj = lox::Object::make_nil_obj();
    auto literal = std::make_shared<Object>(literal_obj);
    addToken(type, literal);
}

auto Scanner::addToken(TokenType type, ObjectRef literal) -> void {
    std::string lexeme = m_source.substr(m_start, m_current - m_start);
    m_tokens.push_back(std::make_shared<Token>(type, lexeme, literal, m_line));
}

auto Scanner::match(char expected) -> bool {
    if (isAtEnd())
        return false;
    if (m_source[m_current] != expected)
        return false;
    m_current++;
    return true;
}

auto Scanner::peek() -> char {
    if (isAtEnd())
        return '\0';
    return m_source[m_current];
}

auto Scanner::peekNext() -> char {
    if (m_current + 1 >= static_cast<int>(m_source.size()))
        return '\0';
    return m_source[m_current + 1];
}

auto Scanner::get_string() -> void {
    // 当下一个不是第二个"并且不是末尾的时候移动current
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            m_line++;
        advance();
    }
    if (isAtEnd()) {
        lox.error(m_line, "Unterminated string.");
        return;
    }
    advance();
    // 获取字面量的具体值并且构建string_obj
    auto value = m_source.substr(m_start + 1, m_current - m_start - 2);
    auto literal = std::make_shared<Object>(Object::make_str_obj(value));

    addToken(STRING, literal);
}

auto Scanner::get_number() -> void {
    while (isdigit(peek()))
        advance();
    // Look for a fractional part.
    if (peek() == '.' && isdigit(peekNext())) {
        // Consume the "."
        advance();
        while (isdigit(peek()))
            advance();
    }

    auto literal_obj = Object::make_num_obj(
        std::stod(m_source.substr(m_start, m_current - m_start)));
    auto literal = std::make_shared<Object>(literal_obj);
    addToken(NUMBER, literal);
}

auto Scanner::identifier() -> void {
    while (std::isalpha(peek()) || std::isdigit(peek())) {
        advance();
    }
    auto text = m_source.substr(m_start, m_current - m_start);
    TokenType type;
    if (lox::stringToTokenType.find(text) == lox::stringToTokenType.end()) {
        type = IDENTIFIER;
    } else {
        type = stringToTokenType.at(text);
    }
    addToken(type);
}

auto Scanner::scanToken() -> void {
    auto c = advance();
    switch (c) {
    // 单字符词素
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(LEFT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case '-':
        addToken(MINUS);
        break;
    case '+':
        addToken(PLUS);
        break;
    case ';':
        addToken(SEMICOLON);
        break;
    case '*':
        addToken(STAR);
        break;
    // 可能是单字符，也可能是双字符词素
    case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
    case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
    // 更长的字符词素
    case '/':
        // 说明是注释
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd())
                advance();
        } else { // 说明是除法
            addToken(SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        m_line++;
        break;
    case '"':
        get_string();
        break;
    default:
        if (std::isdigit(c)) {
            get_number();
        } else if (std::isalpha(c)) {
            identifier();
        } else {
            lox.error(m_line, "Unexpected character.");
        }
        break;
    }
}

auto Scanner::scanTokens() -> std::vector<TokenRef> {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    auto literal_obj = lox::Object::make_nil_obj();
    auto literal = std::make_shared<Object>(literal_obj);
    m_tokens.push_back(std::make_shared<Token>(lox::TokenType::EOF_TOKEN, "",
                                               literal, m_line));
    return m_tokens;
}

} // namespace lox
