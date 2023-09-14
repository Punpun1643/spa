#include "QueryExpression.h"

std::unique_ptr<DeclarationListExpression> QueryExpression::GetDeclarationListExpression() {
  return std::move(this->declaration_expressions);
}

std::unique_ptr<SelectExpression> QueryExpression::GetSelectExpression() {
  return std::move(this->select_expression);
}

/* std::unique_ptr<SuchThatExpressionList> QueryExpression::GetSuchThatExpressions() { */
/*   return std::move(this->such_that_expressions); */
/* } */

void QueryExpression::acceptInterpreter(QueryInterpreter &interpreter) {
  interpreter.Interpret(*this);
}
