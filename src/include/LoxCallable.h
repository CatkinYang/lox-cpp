#pragma once

#include "Object.h"
#include <memory>
#include <string>
#include <vector>
namespace lox {

class LoxCallable;
using LoxCallableRef = std::shared_ptr<LoxCallable>;
class Interpreter;
using InterpreterRef = std::shared_ptr<Interpreter>;

class LoxCallable {
  public:
    virtual ~LoxCallable() = default;
    virtual auto call(InterpreterRef interpreter,
                      std::vector<ObjectRef> arguments) -> ObjectRef = 0;
    virtual auto arity() -> int = 0;
    virtual auto toString() -> std::string = 0;
};

} // namespace lox
