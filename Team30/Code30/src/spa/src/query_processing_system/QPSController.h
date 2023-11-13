#pragma once

#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "clauses/Clause.h"
#include "context/Context.h"
#include "evaluator/QueryEvaluator.h"
#include "expression/AExpression.h"
#include "parser/ExpressionTreeBuilder.h"
#include "parser/SyntaxChecker.h"
#include "query_processing_system/references/AttrRef.h"

class QPSController {
 public:
  QPSController();

  void HandleQuery(std::string& query, std::list<std::string>& results,
                   std::shared_ptr<QueryEvaluator> query_evaluator);
  std::pair<std::vector<AttrRef>, std::vector<std::shared_ptr<Clause>>>
  TokensToClauses(std::vector<std::shared_ptr<Token>> tokens);

 private:
  static std::string const BOOLEAN_TRUE_RESULT;
  static std::string const BOOLEAN_FALSE_RESULT;
  static std::string const RESULT_STRING_DELIMITER;

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
