#pragma once

#include "AExpression.h"
#include "SuchThatExpression.h"

class SuchThatListExpression : public AExpression {
 public:
  SuchThatListExpression(
      std::vector<std::shared_ptr<SuchThatExpression>> such_that_list_);
  std::unique_ptr<std::vector<std::shared_ptr<SuchThatExpression>>>
  GetSuchThatExpressions();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~SuchThatListExpression() = default;

 private:
  std::vector<std::shared_ptr<SuchThatExpression>> such_that_list;
};
