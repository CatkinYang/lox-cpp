#pragma once

#include "Environment.h"
#include "LoxCallable.h"
#include "Statements.h"
#include <memory>
namespace lox {

class LoxFunction;
using LoxFunctionRef = std::shared_ptr<LoxFunction>;

class LoxFunction : public LoxCallable,
                    public std::enable_shared_from_this<LoxFunction> {
  public:
    explicit LoxFunction(FunStmtRef declaration, EnvironmentRef closure)
        : m_declaration(declaration), m_closure(closure) {};

    auto call(InterpreterRef interpreter, std::vector<ObjectRef> arguments)
        -> ObjectRef;
    auto arity() -> int;
    auto toString() -> std::string;

    auto getDeclaration() { return m_declaration; }

  private:
    FunStmtRef m_declaration;
    EnvironmentRef m_closure;
};

} // namespace lox