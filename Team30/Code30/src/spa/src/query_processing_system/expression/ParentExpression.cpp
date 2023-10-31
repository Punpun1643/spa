#include "ParentExpression.h"

#include <string>

ParentExpression::ParentExpression(std::string arg1, std::string arg2,
                                   bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void ParentExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<ParentExpression>(shared_from_this()));
}
