#pragma once
#include "RuntimeError.h"
#include "Token.h"
#include <exception>
#include <string>

namespace lox {
class RuntimeError : public std::exception {
  public:
    RuntimeError(TokenRef token, std::string messgae)
        : m_token(token), m_message(messgae) {};
    auto getToken() -> TokenRef;
    auto getMessage() -> std::string;

    virtual const char *what() const throw() { return m_message.c_str(); }

  private:
    TokenRef m_token;
    std::string m_message;
};
} // namespace lox
