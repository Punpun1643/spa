#include "WithExpression.h"

#include <string>

#include "../interpreter/QueryInterpreter.h"

WithExpression::WithExpression(std::string first_ref, std::string second_ref,
                               bool is_not)
    : ClauseExpression(first_ref, second_ref, is_not) {}

void WithExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<WithExpression>(shared_from_this()));
}
