#pragma once

#include "Object.h"
#include "Token.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace lox {

class Environment;
using EnvironmentRef = std::shared_ptr<Environment>;

class Environment : public std::enable_shared_from_this<Environment> {
  public:
    Environment();
    Environment(EnvironmentRef enclosing);

    auto getEnclosing() -> EnvironmentRef { return m_enclosing; }

    auto define(std::string name, ObjectRef value) -> void;

    auto get(TokenRef name) -> ObjectRef;
    auto getAt(int distance, std::string name) -> ObjectRef;

    auto assign(TokenRef name, ObjectRef value) -> void;
    auto assignAt(int distance, TokenRef name, ObjectRef value) -> void;

    auto ancestor(int distance) -> EnvironmentRef;

  private:
    std::unordered_map<std::string, ObjectRef> m_values;
    EnvironmentRef m_enclosing;
};

} // namespace lox
