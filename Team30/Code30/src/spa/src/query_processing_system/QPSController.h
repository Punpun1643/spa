#pragma once

#include <list>

#include "common/Clause.h"
#include "context/Context.h"
#include "evaluator/QueryEvaluator.h"
#include "expression/AExpression.h"
#include "parser/ExpressionTreeBuilder.h"
#include "parser/SyntaxChecker.h"

class QPSController {
 public:
  QPSController();

  void HandleQuery(std::string& query, std::list<std::string>& results,
                   std::shared_ptr<QueryEvaluator> query_evaluator);
  std::pair<std::vector<PqlDeclaration>, std::vector<std::shared_ptr<Clause>>>
  TokensToClauses(std::vector<std::shared_ptr<Token>> tokens);

 private:
  void CheckSyntax(std::vector<std::shared_ptr<Token>> tokens);
  std::shared_ptr<Context> FormContext(
      std::vector<std::shared_ptr<Token>> tokens);
  std::shared_ptr<AExpression> FormExpressionTree(
      std::vector<std::shared_ptr<Token>> tokens,
      std::shared_ptr<Context> context);
  void InterpretContext(std::shared_ptr<Context> context,
                        std::shared_ptr<AExpression> expression_tree);
  std::vector<std::shared_ptr<Token>> TokenizeQuery(std::string& query);
};
