#pragma once

#include "Object.h"
#include "Token.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace lox {

class Environment;
using EnvironmentRef = std::shared_ptr<Environment>;

class Environment {
  public:
    Environment();
    Environment(EnvironmentRef enclosing);

    auto define(std::string name, ObjectRef value) -> void;
    auto get(TokenRef name) -> ObjectRef;
    auto assign(TokenRef name, ObjectRef value) -> void;

  private:
    std::unordered_map<std::string, ObjectRef> m_values;
    EnvironmentRef m_enclosing;
};

} // namespace lox
