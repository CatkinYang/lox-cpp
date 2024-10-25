#include "Scanner.h"
#include "gtest/gtest.h"
#include <ostream>

namespace lox {

TEST(ScannerTest, BasicTest1) {
    std::string source = "var a = 123;";
    auto scan = new Scanner(source);
    scan->scanTokens();
    auto token_vec = scan->getTokens();
    for (const auto &t : token_vec) {
        std::cout << t->toString() << std::endl;
    }
    // EXPECT_EQ("123", token_vec[0]->getLexeme());
    // EXPECT_EQ("abc", token_vec[1]->getLexeme());
    // EXPECT_EQ("var", token_vec[2]->getLexeme());
}

} // namespace lox

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); // Runs all the tests
}
