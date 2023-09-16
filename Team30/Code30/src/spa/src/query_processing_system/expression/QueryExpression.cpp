#include "QueryExpression.h"

QueryExpression::QueryExpression(
    std::unique_ptr<SelectExpression> select_expression)
    : select_expression(std::move(select_expression)) {}

QueryExpression::QueryExpression(
    std::unique_ptr<DeclarationListExpression> declaration_list_expression,
    std::unique_ptr<SelectExpression> select_expression)
    : declaration_list_expression(std::move(declaration_list_expression)),
      select_expression(std::move(select_expression)) {}

std::unique_ptr<DeclarationListExpression>
QueryExpression::GetDeclarationListExpression() {
  return std::move(this->declaration_list_expression);
}

std::unique_ptr<SelectExpression> QueryExpression::GetSelectExpression() {
  return std::move(this->select_expression);
}

/* std::unique_ptr<SuchThatExpressionList>
 * QueryExpression::GetSuchThatExpressions() { */
/*   return std::move(this->such_that_expressions); */
/* } */

void QueryExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
