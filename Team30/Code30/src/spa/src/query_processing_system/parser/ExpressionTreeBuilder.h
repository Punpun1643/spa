#pragma once

#include <optional>

#include "../context/Context.h"
#include "../expression/ClauseExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/SuchThatExpression.h"
#include "../expression/WithExpression.h"
#include "QpParser.h"

class ExpressionTreeBuilder : public QpParser {
 public:
  ExpressionTreeBuilder(std::vector<std::shared_ptr<Token>> tokens,
                        std::shared_ptr<Context> context);

  void parse() override;
  std::shared_ptr<AExpression> GetExpressionTree();

  ~ExpressionTreeBuilder() = default;

 private:
  std::shared_ptr<AExpression> expression_tree;
  std::shared_ptr<Context> context;

  std::optional<std::shared_ptr<ClauseExpression>> CreateClauseExpressionHead();
  std::shared_ptr<PatternExpression> CreatePatternExpressionHead();
  std::optional<std::shared_ptr<SelectExpression>> CreateSelectExpression();
  std::shared_ptr<SelectExpression> CreateSelectExpressionHead();
  std::shared_ptr<SuchThatExpression> CreateSuchThatExpression();
  std::shared_ptr<WithExpression> CreateWithExpressionHead();
};
