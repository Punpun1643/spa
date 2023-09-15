#pragma once

#include "../expression/QueryExpression.h"
#include "QpParser.h"

class ExpressionTreeBuilder : public QpParser {
 public:
  ExpressionTreeBuilder(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;
  std::unique_ptr<QueryExpression> GetQueryExpression();
  std::unique_ptr<SelectExpression> CreateSelectExpression();
  std::unique_ptr<DeclarationListExpression> CreateDeclarationListExpression();

 private:
  std::unique_ptr<QueryExpression> query_expression;
};
