#include "DeclarationExpression.h"


void DeclarationExpression::acceptInterpreter(QueryInterpreter &interpreter) {
  interpreter.InterpretDeclarations(*this);
}

std::string DeclarationExpression::getDesignEntity() {
  return this->designEntity;
}

std::string DeclarationExpression::getSynonym() {
  return this->synonym;
}
