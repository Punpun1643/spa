#include "SelectExpression.h"

SelectExpression::SelectExpression(std::string synonym) : synonym(synonym){};

std::string SelectExpression::GetSynonym() { return this->synonym; }

void SelectExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
