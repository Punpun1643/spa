#pragma once

#include "../expression/QueryExpression.h"
#include "QpParser.h"

class ExpressionTreeBuilder : public QpParser {
 public:
  ExpressionTreeBuilder(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;
  std::unique_ptr<QueryExpression> GetQueryExpression();

  ~ExpressionTreeBuilder() = default;

 private:
  std::unique_ptr<QueryExpression> query_expression;
  std::unique_ptr<SelectExpression> CreateSelectExpression();
  std::unique_ptr<DeclarationListExpression> CreateDeclarationListExpression();
  std::unique_ptr<SuchThatListExpression> CreateSuchThatListExpression();
  void AddSuchThatExpression(std::vector<std::shared_ptr<SuchThatExpression>>& such_that_expression_list);
  void AddDeclarationExpression(std::vector<std::shared_ptr<DeclarationExpression>>& declaration_expression_list);
};
