#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    std::unique_ptr<SelectClause> select_clause) {
  return std::unique_ptr<std::vector<std::string>>();
}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    std::unique_ptr<SelectClause> select_clause,
    std::unique_ptr<SuchThatClause> such_that_clause) {
  return std::unique_ptr<std::vector<std::string>>();
}
