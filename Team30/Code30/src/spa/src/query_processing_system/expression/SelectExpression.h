#pragma once

/* #include "AExpression.h" */
#include <cassert>

#include "SuchThatExpression.h"

class SelectExpression : public AExpression {
 public:
  SelectExpression(std::string synonym, bool is_boolean);
  std::string GetSynonym();
  bool IsBoolean();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~SelectExpression() = default;

 private:
  bool is_boolean;
  std::string synonym;
};
