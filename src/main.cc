#program once
#include <iostream>

namespace lox {
int main(int argc, const char *argv[]) {
    if (argc > 1) {
        std::cout << "Usage: lox [script]" << std::endl;
        std::exit(64);
    } else if (argc == 1) {
        runFile(argv[0]);
    } else {
        runPrompt();
    }
}

} // namespace lox
