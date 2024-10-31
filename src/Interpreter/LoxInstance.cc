#include "Interpreter/LoxInstance.h"
#include "Interpreter/Object.h"
#include "Interpreter/RuntimeError.h"
#include "Interpreter/Token.h"

#include <memory>

namespace lox {

auto LoxInstance::get(TokenRef name) -> ObjectRef {
    if (m_fields.find(name->getLexeme()) != m_fields.end()) {
        return m_fields.at(name->getLexeme());
    }
    auto method = m_class->findMethod(name->getLexeme());

    if (method != nullptr) {
        auto res = Object::make_fun_obj(
            std::dynamic_pointer_cast<LoxCallable>(method));
        return std::make_shared<Object>(res);
    }
    throw RuntimeError(name, "Undefined property '" + name->getLexeme() + "'.");
}

auto LoxInstance::set(TokenRef name, ObjectRef value) -> void {
    m_fields.insert({name->getLexeme(), value});
}

} // namespace lox
