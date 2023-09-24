#include "QueryExpression.h"

#include <iostream>

QueryExpression::QueryExpression(
    std::unique_ptr<SelectExpression> select_expression)
    : select_expression(std::move(select_expression)),
      declaration_list_expression(nullptr),
      such_that_list_expression(nullptr),
      pattern_expression(nullptr) {}

QueryExpression::QueryExpression(
    std::unique_ptr<DeclarationListExpression> declaration_list_expression,
    std::unique_ptr<SelectExpression> select_expression)
    : declaration_list_expression(std::move(declaration_list_expression)),
      select_expression(std::move(select_expression)),
      such_that_list_expression(nullptr),
      pattern_expression(nullptr) {}

QueryExpression::QueryExpression(
    std::unique_ptr<DeclarationListExpression> declaration_list_expression,
    std::unique_ptr<SelectExpression> select_expression,
    std::unique_ptr<SuchThatListExpression> such_that_list_expression)
    : declaration_list_expression(std::move(declaration_list_expression)),
      select_expression(std::move(select_expression)),
      such_that_list_expression(std::move(such_that_list_expression)),
      pattern_expression(nullptr) {}

QueryExpression::QueryExpression(
    std::unique_ptr<DeclarationListExpression> declaration_list_expression,
    std::unique_ptr<SelectExpression> select_expression,
    std::unique_ptr<SuchThatListExpression> such_that_list_expression,
    std::unique_ptr<PatternExpression> pattern_expression)
    : declaration_list_expression(std::move(declaration_list_expression)),
      select_expression(std::move(select_expression)),
      such_that_list_expression(std::move(such_that_list_expression)),
      pattern_expression(std::move(pattern_expression)) {}

std::unique_ptr<DeclarationListExpression>
QueryExpression::GetDeclarationListExpression() {
  return std::move(this->declaration_list_expression);
}

std::unique_ptr<PatternExpression> QueryExpression::GetPatternExpression() {
  return std::move(this->pattern_expression);
}

std::unique_ptr<SelectExpression> QueryExpression::GetSelectExpression() {
  return std::move(this->select_expression);
}

std::unique_ptr<SuchThatListExpression>
QueryExpression::GetSuchThatListExpression() {
  return std::move(this->such_that_list_expression);
}

bool QueryExpression::HasDeclarationListExpression() {
  return (this->declaration_list_expression.get() != nullptr);
}

bool QueryExpression::HasPatternExpression() {
  return (this->pattern_expression.get() != nullptr);
}

bool QueryExpression::HasSelectExpression() {
  return (this->select_expression.get() != nullptr);
}

bool QueryExpression::HasSuchThatListExpression() {
  return (this->such_that_list_expression.get() != nullptr);
}

void QueryExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
