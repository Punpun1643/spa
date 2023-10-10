#include "SelectExpression.h"

SelectExpression::SelectExpression(std::string synonym) : synonym(synonym){};

std::string SelectExpression::GetSynonym() { return this->synonym; }

void SelectExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<SelectExpression>(shared_from_this()));
}
