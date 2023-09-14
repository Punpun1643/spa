#pragma once

#include "AExpression.h"

class SelectExpression : public AExpression {
  public:
    std::string GetSynonym();
    void acceptInterpreter(QueryInterpreter &interpreter) override;
  private:
    std::string synonym;
};
