#pragma once

#include "LoxCallable.h"
#include "LoxFunction.h"
#include "Object.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace lox {

class LoxClass;
using LoxClassRef = std::shared_ptr<LoxClass>;

class LoxClass : public LoxCallable,
                 public std::enable_shared_from_this<LoxClass> {
  public:
    explicit LoxClass(std::string name, LoxClassRef super,
                      std::unordered_map<std::string, LoxFunctionRef> methods)
        : m_name(name), m_super(super), m_methods(methods) {};

    auto findMethod(std::string name) -> LoxFunctionRef;
    auto call(InterpreterRef interpreter, std::vector<ObjectRef> arguments)
        -> ObjectRef override;
    auto arity() -> int override;

    auto toString() -> std::string { return m_name; }
    auto getName() -> std::string { return m_name; }
    auto getMethods() { return m_methods; }

  private:
    std::string m_name;
    LoxClassRef m_super;
    std::unordered_map<std::string, LoxFunctionRef> m_methods;
};

} // namespace lox
