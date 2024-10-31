#include "Object.h"

namespace lox {

std::string Object::toString() {
    switch (m_type) {
    case Object_nil:
        return "nil";
    case Object_bool:
        return m_boolean ? "1" : "0";
    case Object_str:
        return m_str;
    case Object_fun:
        return "function";
    default:
        return std::to_string(m_num);
    }
}

Object Object::make_num_obj(double num) {
    Object num_obj;
    num_obj.m_type = Object_num;
    num_obj.m_num = num;
    return num_obj;
}

Object Object::make_str_obj(std::string str) {
    Object str_obj;
    str_obj.m_type = Object_str;
    str_obj.m_str = str;
    return str_obj;
}

Object Object::make_bool_obj(bool boolean) {
    Object bool_obj;
    bool_obj.m_type = Object_bool;
    bool_obj.m_boolean = boolean;
    return bool_obj;
}

Object Object::make_nil_obj() {
    Object nil_obj;
    nil_obj.m_type = Object_nil;
    nil_obj.m_nil = nullptr;
    return nil_obj;
}

Object Object::make_fun_obj(LoxCallableRef function) {
    Object fun_obj;
    fun_obj.m_type = Object_fun;
    fun_obj.m_function = function;
    return fun_obj;
}

Object Object::make_instance_obj(LoxInstanceRef instance) {
    Object instance_obj;
    instance_obj.m_type = Object_instance;
    instance_obj.m_instance = instance;
    return instance_obj;
}

Object Object::make_class_obj(LoxClassRef klass) {
    Object klass_obj;
    klass_obj.m_type = Object_class;
    klass_obj.m_class = klass;
    return klass_obj;
}

} // namespace lox
