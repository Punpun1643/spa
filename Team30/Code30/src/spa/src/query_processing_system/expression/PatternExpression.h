#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "ClauseExpression.h"

class PatternExpression : public ClauseExpression {
 public:
  PatternExpression(std::string syn_assign, std::string arg1, std::string arg2);
  void acceptInterpreter(QueryInterpreter& interpreter) override;
  std::string GetSynAssign();

  ~PatternExpression() = default;

 private:
  std::string syn_assign;
};
