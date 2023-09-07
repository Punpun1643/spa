#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb): pkb(pkb) {}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    SelectClause select_clause) {
  return std::unique_ptr<std::vector<std::string>>();
}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    SelectClause select_clause, SuchThatClause such_that_clause) {
  return std::unique_ptr<std::vector<std::string>>();
}
