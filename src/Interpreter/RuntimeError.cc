#include "Interpreter/RuntimeError.h"
#include <string>

namespace lox {

auto RuntimeError::getToken() -> TokenRef { return m_token; }
auto RuntimeError::getMessage() -> std::string { return m_message; }

} // namespace lox
