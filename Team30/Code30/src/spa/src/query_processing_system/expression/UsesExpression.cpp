#include "UsesExpression.h"

#include <string>

UsesExpression::UsesExpression(std::string arg1, std::string arg2, bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void UsesExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<UsesExpression>(shared_from_this()));
}
