#pragma once

#include <optional>

#include "../context/Context.h"
#include "../expression/ClauseExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/SuchThatExpression.h"
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

  std::optional<std::shared_ptr<ClauseExpression>> CreateClauseExpression();
  std::shared_ptr<PatternExpression> CreatePatternExpression();
  std::shared_ptr<SelectExpression> CreateSelectExpression();
  std::shared_ptr<SuchThatExpression> CreateSuchThatExpression();
};
