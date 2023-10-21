#include <string>

#include "UsesExpression.h"

UsesExpression::UsesExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2) {}

void UsesExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<UsesExpression>(shared_from_this()));
}
