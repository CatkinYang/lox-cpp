#pragma once
#include "Token.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

namespace lox {
class Lox {
  public:
    void run();
    void run(const std::string &content);
    void runFile(const std::string &path);
    void runPrompt();
    void report(int line, std::string where, std::string message);
    void error(int line, std::string message);
    auto error(Token token, std::string message) -> void;

  private:
    bool hasError = false;
};
} // namespace lox
