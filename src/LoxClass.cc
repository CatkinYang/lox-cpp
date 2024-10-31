#include "LoxClass.h"
#include "LoxInstance.h"
#include "Object.h"
#include <memory>

namespace lox {

auto LoxClass::findMethod(std::string name) -> LoxFunctionRef {
    if (m_methods.find(name) != m_methods.end()) {
        return m_methods.at(name);
    }
    return nullptr;
}

auto LoxClass::call(InterpreterRef interpreter,
                    std::vector<ObjectRef> arguments) -> ObjectRef {
    auto instance = std::make_shared<LoxInstance>(shared_from_this());
    auto instance_obj = Object::make_instance_obj(instance);
    auto instance_ref = std::make_shared<Object>(instance_obj);

    auto initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance_ref;
}

auto LoxClass::arity() -> int {
    auto initializer = findMethod("init");
    if (initializer == nullptr)
        return 0;
    return initializer->arity();
}

} // namespace lox
