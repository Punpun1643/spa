#include "NextExpression.h"

NextExpression::NextExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void NextExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<NextExpression>(shared_from_this()));
}
