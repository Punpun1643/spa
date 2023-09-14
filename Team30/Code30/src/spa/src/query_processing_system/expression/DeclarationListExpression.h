#pragma once

#include "AExpression.h"
#include "DeclarationExpression.h"

class DeclarationListExpression : public AExpression {
  public:
    std::unique_ptr<std::vector<DeclarationExpression>> GetDeclarations();
    void acceptInterpreter(QueryInterpreter &interpreter) override;
  private:
    std::vector<DeclarationExpression> declaration_list;
};
