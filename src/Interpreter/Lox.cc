#include "Interpreter/Lox.h"
#include "Interpreter/Interpreter.h"
#include "Interpreter/Parser.h"
#include "Interpreter/Resolver.h"
#include "Interpreter/Scanner.h"
#include "Interpreter/Tokentype.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
namespace lox {

auto Lox::hasError = false;
auto Lox::hasRuntimeError = false;

void Lox::run(const std::string &source) {
    auto scanner = std::make_shared<Scanner>(source);
    auto tokens = scanner->scanTokens();
    auto parser = std::make_shared<Parser>(tokens);

    auto expr = parser->parse();

    if (hasError)
        return;

    auto interpreter = std::make_shared<Interpreter>();
    auto resolver = std::make_shared<Resolver>(interpreter);
    if (hasError)
        return;
    interpreter->interpret(expr);
}

void Lox::runFile(const std::string &path) {
    // 检查文件是否存在
    if (!std::__fs::filesystem::exists(path)) {
        throw std::runtime_error("File not found: " + path);
    }
    // 打开文件并读取内容
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // 将文件内容读入缓冲区
    run(buffer.str());      // 将内容传递给run函数
    if (hasError)
        std::exit(65);
    if (hasRuntimeError)
        std::exit(70);
}

void Lox::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) { // 从标准输入读取一行
            break;                           // 如果输入流结束，退出循环
        }
        run(line); // 将输入传递给run函数处理
        hasError = false;
    }
};

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message;
    hasError = true;
}

void Lox::error(int line, std::string message) { report(line, "", message); }

auto Lox::error(TokenRef token, std::string message) -> void {
    if (token->getType() == TokenType::EOF_TOKEN) {
        report(token->getLine(), "at end", message);
    } else {
        report(token->getLine(), " at ", message);
    }
}
void Lox::runtimeError(RuntimeError error) {
    std::cout << std::string(error.what()) + "\n[line " +
                     std::to_string(error.getToken()->getLine()) + "]"
              << std::endl;
    hasRuntimeError = true;
}

} // namespace lox
