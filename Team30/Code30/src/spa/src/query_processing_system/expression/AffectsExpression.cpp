#include "AffectsExpression.h"

AffectsExpression::AffectsExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void AffectsExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<AffectsExpression>(shared_from_this()));
}
