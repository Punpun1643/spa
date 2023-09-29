#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "AExpression.h"

class PatternExpression : public AExpression {
 public:
  PatternExpression(std::string syn_assign, std::string arg1, std::string arg2);
  void acceptInterpreter(QueryInterpreter& interpreter) override;
  std::string GetArg1();
  std::string GetArg2();
  std::string GetSynAssign();

 private:
  std::string arg1;
  std::string arg2;
  std::string syn_assign;
};
