#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PkbApi& pkb): pkb(pkb) {}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    std::unique_ptr<SelectClause> select_clause) {
  std::unique_ptr<ClauseResult> result = select_clause->evaluate(pkb);
  PqlDeclaration selected_declaration = *(select_clause->getDeclaration());
  return result->getValues(selected_declaration);
}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    std::unique_ptr<SelectClause> select_clause,
    std::unique_ptr<SuchThatClause> such_that_clause) {
  std::unique_ptr<ClauseResult> select_result = select_clause->evaluate(pkb);
  std::unique_ptr<ClauseResult> such_that_result = such_that_clause->evaluate(pkb);

  select_result->combineResults(*such_that_result);
  PqlDeclaration selected_declaration = *(select_clause->getDeclaration());
  return select_result->getValues(selected_declaration);
}
