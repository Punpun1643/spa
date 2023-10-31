#include "ParentTExpression.h"

#include <string>

ParentTExpression::ParentTExpression(std::string arg1, std::string arg2,
                                     bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void ParentTExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<ParentTExpression>(shared_from_this()));
}
