#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "AExpression.h"

class PatternExpression : public AExpression {
 public:
  PatternExpression(std::string syn_assign, std::string pattern);
  void acceptInterpreter(QueryInterpreter& interpreter) override;
  std::string GetPattern();
  std::string GetSynAssign();

 private:
  std::string pattern;
  std::string syn_assign;
};
