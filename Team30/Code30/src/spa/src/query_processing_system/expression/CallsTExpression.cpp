#include <string>

#include "CallsTExpression.h"

CallsTExpression::CallsTExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2) {}

void CallsTExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<CallsTExpression>(shared_from_this()));
}
