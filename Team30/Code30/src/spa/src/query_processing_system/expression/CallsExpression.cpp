#include "CallsExpression.h"

#include <string>

CallsExpression::CallsExpression(std::string arg1, std::string arg2,
                                 bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void CallsExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<CallsExpression>(shared_from_this()));
}
