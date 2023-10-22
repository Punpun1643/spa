#include "WithExpression.h"

#include <string>

WithExpression::WithExpression(std::string first_ref, std::string second_ref)
    : ClauseExpression(first_ref, second_ref) {}

void WithExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<WithExpression>(shared_from_this()));
}
