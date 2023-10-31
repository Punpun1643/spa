#include "PatternIfExpression.h"

#include <memory>
#include <string>

PatternIfExpression::PatternIfExpression(std::string synonym, std::string arg1)
    : PatternExpression(synonym, arg1, "") {}

void PatternIfExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<PatternIfExpression>(shared_from_this()));
}
