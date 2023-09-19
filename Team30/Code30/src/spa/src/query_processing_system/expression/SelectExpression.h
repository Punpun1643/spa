#pragma once

#include "AExpression.h"

class SelectExpression : public AExpression {
 public:
  SelectExpression(std::string synonym);
  std::string GetSynonym();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~SelectExpression() = default;

 private:
  std::string synonym;
};