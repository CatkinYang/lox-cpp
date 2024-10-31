#pragma once

#include "LoxCallable.h"
#include "LoxClass.h"
#include "Object.h"
#include "Token.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace lox {

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
  public:
    explicit LoxInstance(LoxClassRef klass) : m_class(klass) {};

    auto get(TokenRef name) -> ObjectRef;
    auto set(TokenRef name, ObjectRef value) -> void;

    auto toString() -> std::string { return m_class->getName() + " instance"; }

    auto getFields() { return m_fields; }
    auto getClass() { return m_class; }

  private:
    LoxClassRef m_class;
    std::unordered_map<std::string, ObjectRef> m_fields;
};

} // namespace lox
