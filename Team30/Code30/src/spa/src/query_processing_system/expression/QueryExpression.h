#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "AExpression.h"
#include "DeclarationListExpression.h"
#include "SelectExpression.h"

class QueryExpression : public AExpression {
 public:
  QueryExpression(
      std::unique_ptr<DeclarationListExpression> declaration_list_expression,
      std::unique_ptr<SelectExpression> select_expression);
  std::unique_ptr<DeclarationListExpression> GetDeclarationListExpression();
  std::unique_ptr<SelectExpression> GetSelectExpression();
  /* std::unique_ptr<SuchThatExpressionList> GetSuchThatExpressions(); */
  void acceptInterpreter(QueryInterpreter& interpreter) override;

 private:
  std::unique_ptr<DeclarationListExpression> declaration_list_expression;
  std::unique_ptr<SelectExpression> select_expression;
  /* std::unique_ptr<SuchThatExpressionList> such_that_expressions; */
};
