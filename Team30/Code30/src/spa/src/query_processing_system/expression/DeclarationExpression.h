#pragma once

#include <string>

#include "AExpression.h"
#include "../interpreter/QueryInterpreter.h"


class DeclarationExpression : public AExpression {
  public:
    void acceptInterpreter(QueryInterpreter &interpreter) override;
    std::string getDesignEntity();
    std::string getSynonym();
  private:
    std::string designEntity;
    std::string synonym;
};
