#include "include/Lox.h"

namespace lox {

void Lox::run(const std::string &content) { std::stringstream input(content); }

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

} // namespace lox
