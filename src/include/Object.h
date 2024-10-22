#pragma once
#include <string>

namespace lox {
class Object {
  public:
    enum Object_type {
        Object_str,
        Object_num,
        Object_bool,
        Object_nil,
        Object_fun,
        Object_instance,
        Object_class,
    };

    std::string toString();
    // shared_ptr<LoxCallable> function;
    // shared_ptr<LoxInstance> instance;
    // shared_ptr<LoxClass> lox_class;
    static Object make_num_obj(double num);
    static Object make_str_obj(std::string str);
    static Object make_bool_obj(bool boolean);
    static Object make_nil_obj();
    // static Object make_fun_obj(shared_ptr<LoxCallable> function_);
    // static Object make_instance_obj(shared_ptr<LoxInstance> instance_);
    // static Object make_class_obj(shared_ptr<LoxClass> lox_class_);
  private:
    std::string m_str;
    double m_num;
    bool m_boolean;
    int *m_nil;
    Object_type m_type;
};
} // namespace lox
