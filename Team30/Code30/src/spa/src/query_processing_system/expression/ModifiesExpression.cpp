#include "ModifiesExpression.h"

ModifiesExpression::ModifiesExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void ModifiesExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<ModifiesExpression>(shared_from_this()));
}
