#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "AExpression.h"
#include "DeclarationListExpression.h"
#include "SelectExpression.h"
#include "SuchThatListExpression.h"

class QueryExpression : public AExpression {
 public:
  QueryExpression() {}
  QueryExpression(std::unique_ptr<SelectExpression> select_expression);
  QueryExpression(
      std::unique_ptr<DeclarationListExpression> declaration_list_expression,
      std::unique_ptr<SelectExpression> select_expression);
  QueryExpression(
      std::unique_ptr<DeclarationListExpression> declaration_list_expression,
      std::unique_ptr<SelectExpression> select_expression,
      std::unique_ptr<SuchThatListExpression> such_that_list_expression);
  std::unique_ptr<DeclarationListExpression> GetDeclarationListExpression();
  std::unique_ptr<SelectExpression> GetSelectExpression();
  std::unique_ptr<SuchThatListExpression> GetSuchThatListExpression();
  bool HasDeclarationListExpression();
  bool HasSelectExpression();
  bool HasSuchThatListExpression();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~QueryExpression() = default;

 private:
  std::unique_ptr<DeclarationListExpression> declaration_list_expression;
  std::unique_ptr<SelectExpression> select_expression;
  std::unique_ptr<SuchThatListExpression> such_that_list_expression;
};
