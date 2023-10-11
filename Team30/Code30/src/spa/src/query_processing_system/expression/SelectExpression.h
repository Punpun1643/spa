#pragma once

/* #include "AExpression.h" */
#include "SuchThatExpression.h"

class SelectExpression : public AExpression {
 public:
  SelectExpression(std::string synonym, bool is_boolean);
  std::string GetSynonym();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~SelectExpression() = default;

 private:
  bool is_boolean;
  std::string synonym;
};
