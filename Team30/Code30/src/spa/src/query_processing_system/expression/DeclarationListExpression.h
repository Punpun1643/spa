#pragma once

#include "AExpression.h"
#include "DeclarationExpression.h"

class DeclarationListExpression : public AExpression {
 public:
  DeclarationListExpression(
      std::vector<std::shared_ptr<DeclarationExpression>> declaration_list);
  std::unique_ptr<std::vector<std::shared_ptr<DeclarationExpression>>>
  GetDeclarations();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

 private:
  std::vector<std::shared_ptr<DeclarationExpression>> declaration_list;
};
