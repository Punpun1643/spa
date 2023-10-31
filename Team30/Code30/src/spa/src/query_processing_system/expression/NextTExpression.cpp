#include "NextTExpression.h"

#include <string>

NextTExpression::NextTExpression(std::string arg1, std::string arg2,
                                 bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void NextTExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<NextTExpression>(shared_from_this()));
}
