#include "CallsTExpression.h"

#include <string>

CallsTExpression::CallsTExpression(std::string arg1, std::string arg2,
                                   bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void CallsTExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<CallsTExpression>(shared_from_this()));
}
