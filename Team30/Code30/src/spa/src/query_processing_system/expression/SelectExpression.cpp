#include "SelectExpression.h"

std::string SelectExpression::GetSynonym() {
  return this->synonym;
}

void SelectExpression::acceptInterpreter(QueryInterpreter &interpreter) {
  interpreter.Interpret(*this);
}
