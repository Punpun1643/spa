#include "SuchThatListExpression.h"

SuchThatListExpression::SuchThatListExpression(
    std::vector<std::shared_ptr<SuchThatExpression>> such_that_list)
  : such_that_list(such_that_list){};

std::unique_ptr<std::vector<std::shared_ptr<SuchThatExpression>>> SuchThatListExpression::GetSuchThatExpressions() {
  return std::make_unique<std::vector<std::shared_ptr<SuchThatExpression>>>(this->such_that_list);
}

void SuchThatListExpression::acceptInterpreter(
    QueryInterpreter &interpreter) {
  interpreter.Interpret(*this);
}
