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

} // namespace lox
