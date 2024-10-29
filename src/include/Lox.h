#pragma once
#include "Interpreter.h"
#include "RuntimeError.h"
#include "Token.h"
#include <string>

namespace lox {
class Lox {
  public:
    void run(const std::string &content);
    void runFile(const std::string &path);
    void runPrompt();
    void report(int line, std::string where, std::string message);

    void error(int line, std::string message);
    auto error(TokenRef token, std::string message) -> void;
    void runtimeError(RuntimeError error);

  private:
    static bool hasError;
    static bool hasRuntimeError;
};
} // namespace lox
