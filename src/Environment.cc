#include "Environment.h"
#include "RuntimeError.h"
#include "Token.h"
#include <memory>

namespace lox {

Environment::Environment() { m_enclosing = nullptr; }

Environment::Environment(EnvironmentRef enclosing) { m_enclosing = enclosing; }

auto Environment::define(std::string name, ObjectRef value) -> void {
    m_values.insert({name, value});
}

auto Environment::get(TokenRef name) -> ObjectRef {
    if (m_values.find(name->getLexeme()) != m_values.end()) {
        return m_values.at(name->getLexeme());
    }
    if (m_enclosing != nullptr)
        return m_enclosing->get(name);
    throw RuntimeError(name, "Undefined variable '" + name->getLexeme() + "'.");
}

auto Environment::getAt(int distance, std::string name) -> ObjectRef {
    return ancestor(distance)->m_values[name];
}

auto Environment::assign(TokenRef name, ObjectRef value) -> void {
    if (m_values.find(name->getLexeme()) != m_values.end()) {
        m_values.insert({name->getLexeme(), value});
        return;
    }
    if (m_enclosing != nullptr) {
        m_enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name->getLexeme() + "'.");
}

auto Environment::assignAt(int distance, TokenRef name, ObjectRef value)
    -> void {
    ancestor(distance)->m_values[name->getLexeme()] = value;
}

auto Environment::ancestor(int distance) -> EnvironmentRef {
    EnvironmentRef environment = shared_from_this();
    for (int i = 0; i < distance; i++) {
        environment = environment->getEnclosing();
    }
    return environment;
}

} // namespace lox
