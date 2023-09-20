#pragma once

#include <list>

#include "common/Clause.h"
#include "expression/QueryExpression.h"
#include "parser/ExpressionTreeBuilder.h"
#include "parser/SyntaxChecker.h"
#include "evaluator/QueryEvaluator.h"

class QPSController {
 public:
  QPSController();

  void HandleQuery(std::string& query, std::list<std::string>& results, std::shared_ptr<QueryEvaluator> query_evaluator);
  std::vector<std::unique_ptr<Clause>> ParseAndGetClauses(std::vector<std::shared_ptr<Token>>& tokens);

 private:
  std::vector<std::shared_ptr<Token>> TokenizeQuery(std::string& query);
  void CheckSyntax(std::vector<std::shared_ptr<Token>> tokens);
  std::unique_ptr<QueryExpression> FormQueryExpression(std::vector<std::shared_ptr<Token>> tokens);
  std::vector<std::unique_ptr<Clause>> InterpretQueryExpression(std::unique_ptr<QueryExpression> query_expression);
};
