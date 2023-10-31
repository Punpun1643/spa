#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "../context/Context.h"
#include "../expression/ClauseExpression.h"
#include "../expression/PatternAssignExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/PatternIfExpression.h"
#include "../expression/PatternWhileExpression.h"
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
  std::shared_ptr<PatternAssignExpression> CreatePatternAssign(
      std::string synonym, bool is_not);
  std::shared_ptr<PatternIfExpression> CreatePatternIf(std::string synonym,
                                                       bool is_not);
  std::shared_ptr<PatternWhileExpression> CreatePatternWhile(
      std::string synonym, bool is_not);
  std::optional<std::shared_ptr<SelectExpression>> CreateSelectExpression();
  std::shared_ptr<SelectExpression> CreateSelectExpressionHead();
  std::shared_ptr<SuchThatExpression> CreateSuchThatExpressionHead();
  std::shared_ptr<WithExpression> CreateWithExpressionHead();
};
