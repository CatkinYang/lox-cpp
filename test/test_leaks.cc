#include "Interpreter/AstPrinter.h"
#include "Interpreter/Expression.h"
#include "Interpreter/Object.h"
#include "Interpreter/Token.h"
#include "Interpreter/Tokentype.h"
#include "gtest/gtest.h"
#include <memory>
#include <string>
using namespace lox;
int main() {
    std::string str1 = "45.67";
    std::string str2 = "123";

    auto literal_expr =
        std::make_shared<lox::LiteralExpression<std::string>>(str1);
    auto literal_expr_ref =
        std::dynamic_pointer_cast<AbstractExpression<std::string>>(
            literal_expr);

    auto grouping_expr =
        std::make_shared<GroupingExpression<std::string>>(literal_expr_ref);
    auto grouping_ref =
        std::dynamic_pointer_cast<AbstractExpression<std::string>>(
            grouping_expr);

    auto literal_expr_1 =
        std::make_shared<LiteralExpression<std::string>>(str2);
    auto literal_expr_1_ref =
        std::dynamic_pointer_cast<AbstractExpression<std::string>>(
            literal_expr_1);

    auto nil_obj_ref = std::make_shared<Object>(Object::make_nil_obj());

    auto star_ref =
        std::make_shared<Token>(TokenType::STAR, "*", nil_obj_ref, 1);
    auto minus_ref =
        std::make_shared<Token>(TokenType::MINUS, "-", nil_obj_ref, 1);

    auto unary_expr = std::make_shared<UnaryExpression<std::string>>(
        literal_expr_1_ref, minus_ref);

    auto unary_ref =
        std::dynamic_pointer_cast<AbstractExpression<std::string>>(unary_expr);

    auto binary_expr = std::make_shared<BinaryExpression<std::string>>(
        unary_ref, grouping_ref, star_ref);
    auto binary_ref =
        std::dynamic_pointer_cast<AbstractExpression<std::string>>(binary_expr);

    auto ast_printer = std::make_shared<AstPrinter>();
    auto res = ast_printer->print(binary_ref);
    std::cout << res << std::endl;
    return 0;
}
