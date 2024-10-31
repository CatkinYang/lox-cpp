
#include "Interpreter/AstPrinter.h"
#include "Interpreter/Parser.h"
#include "Interpreter/Scanner.h"
#include "gtest/gtest.h"
#include <memory>

namespace lox {

TEST(ParserTest, BasicTest1) {
    std::string source = "var a = 123;";
    auto scanner = std::make_unique<Scanner>(source);
    auto tokens = scanner->scanTokens();
    auto parser = std::make_unique<Parser>(tokens);
    auto expr = parser->parse();

    auto ap = std::make_unique<AstPrinter>();
}

} // namespace lox

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); // Runs all the tests
}
