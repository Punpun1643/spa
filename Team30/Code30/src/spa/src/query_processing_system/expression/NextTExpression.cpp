#include <string>

#include "NextTExpression.h"

NextTExpression::NextTExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2) {}

void NextTExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<NextTExpression>(shared_from_this()));
}
