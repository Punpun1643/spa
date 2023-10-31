#include "NextExpression.h"

#include <string>

NextExpression::NextExpression(std::string arg1, std::string arg2, bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void NextExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<NextExpression>(shared_from_this()));
}
