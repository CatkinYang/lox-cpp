#pragma once

#include "Interpreter.h"
#include "Object.h"
#include <memory>
#include <vector>
namespace lox {

class LoxCallable;
using LoxCallableRef = std::shared_ptr<LoxCallable>;

class LoxCallable {
  public:
    virtual ~LoxCallable() = default;
    virtual auto call(InterpreterRef interpreter,
                      std::vector<ObjectRef> arguments) -> ObjectRef = 0;
    virtual auto arity() -> int = 0;
};

} // namespace lox
