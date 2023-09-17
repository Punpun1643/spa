#include "DeclarationListExpression.h"

DeclarationListExpression::DeclarationListExpression(
    std::vector<std::shared_ptr<DeclarationExpression>> declaration_list)
    : declaration_list(declaration_list){};

std::unique_ptr<std::vector<std::shared_ptr<DeclarationExpression>>>
DeclarationListExpression::GetDeclarations() {
  return std::make_unique<std::vector<std::shared_ptr<DeclarationExpression>>>(
      this->declaration_list);
}

void DeclarationListExpression::acceptInterpreter(
    QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
