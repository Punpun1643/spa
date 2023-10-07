#pragma once

#include <optional>

#include "../expression/ClauseExpression.h"
#include "../expression/QueryExpression.h"
#include "QpParser.h"

class ExpressionTreeBuilder : public QpParser {
 public:
  ExpressionTreeBuilder(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;
  /* std::unique_ptr<QueryExpression> GetQueryExpression(); */
  std::shared_ptr<AExpression> GetExpressionTree();

  ~ExpressionTreeBuilder() = default;

 private:
  /* std::unique_ptr<QueryExpression> query_expression; */
  std::shared_ptr<AExpression> expression_tree;

  std::optional<std::shared_ptr<ClauseExpression>> CreateClauseExpression();
  /* std::unique_ptr<PatternExpression> CreatePatternExpression(); */
  std::shared_ptr<SelectExpression> CreateSelectExpression();

  /* void
   * AddSuchThatExpression(std::vector<std::shared_ptr<SuchThatExpression>>& */
  /*                                such_that_expression_list); */
  /* void AddDeclarationExpression( */
  /*     std::vector<std::shared_ptr<DeclarationExpression>>& */
  /*         declaration_expression_list); */
};
