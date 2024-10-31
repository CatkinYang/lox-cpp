#pragma once

#include "Object.h"
namespace lox {

using std::runtime_error;

class ReturnError : public runtime_error {
  public:
    Object value;
    explicit ReturnError(Object value_) : runtime_error(""), value(value_) {}
};

} // namespace lox
