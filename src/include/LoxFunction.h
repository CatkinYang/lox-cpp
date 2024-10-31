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
    explicit LoxFunction(FunStmtRef declaration, EnvironmentRef closure,
                         bool isInitializer)
        : m_declaration(declaration), m_closure(closure),
          m_isInitializer(isInitializer) {};

    auto bind(LoxInstanceRef instance) -> LoxFunctionRef;

    auto call(InterpreterRef interpreter, std::vector<ObjectRef> arguments)
        -> ObjectRef override;

    auto arity() -> int override;

    auto toString() -> std::string;

    auto getDeclaration() { return m_declaration; }

  private:
    FunStmtRef m_declaration;
    EnvironmentRef m_closure;
    bool m_isInitializer;
};

} // namespace lox
