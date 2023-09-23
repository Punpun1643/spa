#include "ParentTExpression.h"

ParentTExpression::ParentTExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void ParentTExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
