#include "LoxFunction.h"
#include "Environment.h"
#include "Object.h"
#include "Return.h"
#include <cstddef>
#include <memory>

namespace lox {

auto LoxFunction::bind(LoxInstanceRef instance) -> LoxFunctionRef {
    auto env = std::make_shared<Environment>(m_closure);
    auto instance_obj = Object::make_instance_obj(instance);
    auto instance_ref = std::make_shared<Object>(instance_obj);
    env->define("this", instance_ref);
    auto res =
        std::make_shared<LoxFunction>(m_declaration, env, m_isInitializer);
    return res;
}

auto LoxFunction::call(InterpreterRef interpreter,
                       std::vector<ObjectRef> arguments) -> ObjectRef {

    auto environment = std::make_shared<Environment>(m_closure);

    for (auto i = 0; (size_t)i < m_declaration->getParams().size(); i++) {
        environment->define(m_declaration->getParams().at(i)->getLexeme(),
                            arguments.at(i));
    }

    try {
        interpreter->executeBlock(m_declaration->getBody(), environment);
    } catch (ReturnError return_value) {
        if (m_isInitializer) {
            return m_closure->getAt(0, "this");
        }
        return std::make_shared<Object>(return_value.value);
    }
    if (m_isInitializer) {
        return m_closure->getAt(0, "this");
    }

    return nullptr;
}

auto LoxFunction::arity() -> int { return m_declaration->getParams().size(); }

auto LoxFunction::toString() -> std::string {
    return "<fn " + m_declaration->getName()->getLexeme() + ">";
}

} // namespace lox
