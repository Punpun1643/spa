#pragma once

#include <string>

#include "AExpression.h"


class QueryInterpreter;

class DeclarationExpression : public AExpression {
  public:
    void acceptInterpreter(std::unique_ptr<QueryInterpreter> interpreter) override;
    std::string getDesignEntity();
    std::string getSynonym();
  private:
    std::string designEntity;
    std::string synonym;
};
