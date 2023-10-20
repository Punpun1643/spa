#include <string>

#include "CallsExpression.h"

CallsExpression::CallsExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void CallsExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<CallsExpression>(shared_from_this()));
}
