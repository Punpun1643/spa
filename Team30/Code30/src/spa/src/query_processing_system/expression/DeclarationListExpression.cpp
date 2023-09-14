#include "DeclarationListExpression.h"

std::unique_ptr<std::vector<DeclarationExpression>> DeclarationListExpression::GetDeclarations() {
  return make_unique<std::vector<DeclarationExpression>>(this->declaration_list);
}

void DeclarationListExpression::acceptInterpreter(QueryInterpreter &interpreter) {
  interpreter.Interpret(*this);
}
