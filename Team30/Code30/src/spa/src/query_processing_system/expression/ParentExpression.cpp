#include "ParentExpression.h"

ParentExpression::ParentExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void ParentExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}