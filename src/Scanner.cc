#include "include/Scanner.h"
#include "include/Object.h"
#include "include/Token.h"
#include "include/Tokentype.h"
#include <cctype>
namespace lox {

static Lox lox;

auto Scanner::isAtEnd() -> bool { return m_current >= m_source.size(); }

auto Scanner::advance() -> char {
    m_current++;
    return m_source[m_current - 1];
}

auto Scanner::addToken(TokenType type) -> void {
    Object literal;
    addToken(type, literal);
}

auto Scanner::addToken(TokenType type, Object literal) -> void {
    std::string text = m_source.substr(m_start, m_current);
    m_tokens.push_back(std::make_unique<Token>(type, text, literal, m_line));
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
    if (m_current + 1 >= m_source.size())
        return '\0';
    return m_source[m_current + 1];
}

auto Scanner::get_string() -> void {
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
    auto value = m_source.substr(m_start + 1, m_current - 1);
    // addToken(STRING, value);
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
    //   addToken(NUMBER, std::stod(m_source.substr(m_start, m_current)));
}

auto Scanner::identifier() -> void {
    while (std::isalpha(peek()) || std::isdigit(peek())) {
        advance();
    }
    auto text = m_source.substr(m_start, m_current);
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
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd())
                advance();
        } else {
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

auto Scanner::scanTokens() -> std::list<std::unique_ptr<Token>> {
    while (!isAtEnd()) {
        m_start = m_current;
        //   scanToken();
    }
    Object literal = lox::Object::make_nil_obj();
    m_tokens.push_back(std::make_unique<Token>(lox::TokenType::EOF_TOKEN, "",
                                               literal, m_line));
    return m_tokens;
}

} // namespace lox
