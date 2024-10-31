#pragma once
#include <memory>
#include <string>

namespace lox {

class Object;
using ObjectRef = std::shared_ptr<Object>;

class LoxCallable;
using LoxCallableRef = std::shared_ptr<LoxCallable>;

class LoxClass;
using LoxClassRef = std::shared_ptr<LoxClass>;

class LoxInstance;
using LoxInstanceRef = std::shared_ptr<LoxInstance>;

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
    static Object make_num_obj(double num);
    static Object make_str_obj(std::string str);
    static Object make_bool_obj(bool boolean);
    static Object make_nil_obj();
    static Object make_fun_obj(LoxCallableRef function_);
    static Object make_instance_obj(LoxInstanceRef instance);
    static Object make_class_obj(LoxClassRef klass);
    auto getType() -> Object_type { return m_type; }
    auto getBool() -> bool { return m_boolean; }
    auto getNum() -> double { return m_num; }
    auto getStr() -> std::string { return m_str; };
    auto getFun() -> LoxCallableRef { return m_function; };
    auto getInstance() -> LoxInstanceRef { return m_instance; }

  private:
    std::string m_str;
    double m_num;
    bool m_boolean;
    int *m_nil;
    Object_type m_type;

    LoxCallableRef m_function;
    LoxClassRef m_class;
    LoxInstanceRef m_instance;
};

} // namespace lox
