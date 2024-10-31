add_library(
  lox_interpreter OBJECT
  AstPrinter.cc
  Environment.cc
  Expression.cc
  Interpreter.cc
  Lox.cc
  LoxClass.cc
  LoxFunction.cc
  LoxInstance.cc
  Object.cc
  Parser.cc
  Resolver.cc
  RuntimeError.cc
  Scanner.cc
  Statements.cc
  Token.cc
  TokenType.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lox_interpreter>
    PARENT_SCOPE)
