#pragma once

#include <memory>
#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "ClauseExpression.h"

class PatternExpression : public ClauseExpression {
 public:
  PatternExpression(std::string synonym, std::string arg1, std::string arg2, bool is_not);

  std::string GetSynonym();

  ~PatternExpression() = default;

 private:
  std::string synonym;
};
