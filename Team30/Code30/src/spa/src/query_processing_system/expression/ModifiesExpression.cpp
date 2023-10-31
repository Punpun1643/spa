#include "ModifiesExpression.h"

#include <string>

ModifiesExpression::ModifiesExpression(std::string arg1, std::string arg2, bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void ModifiesExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<ModifiesExpression>(shared_from_this()));
}
