#include "QueryEvaluator.h"

#include <algorithm>
#include <iterator>

QueryEvaluator::QueryEvaluator(PkbApi& pkb) : pkb(pkb) {}

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
  std::unique_ptr<ClauseResult> such_that_result =
      such_that_clause->evaluate(pkb);
  PqlDeclaration selected_declaration = *(select_clause->getDeclaration());

  if (such_that_result->isBooleanResult()) {
    if (such_that_result->getBooleanClauseValue()) {
      return select_result->getValues(selected_declaration);
      // maybe make it so that no need declaration when unambiguous
    } else {
      return std::make_unique<std::vector<std::string>>();  // empty
    }
  }

  if (such_that_result->contains(selected_declaration)) {
    auto decl_values_1 = select_result->getValues(selected_declaration);
    auto decl_values_2 = such_that_result->getValues(selected_declaration);
    return getIntersectingValues(*decl_values_1, *decl_values_2);
  } else {
    return select_result->getValues(selected_declaration);
  }
}

std::unique_ptr<std::vector<std::string>> QueryEvaluator::getIntersectingValues(
    std::vector<std::string> arr_1, std::vector<std::string> arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::vector<std::string> intersecting_values;
  std::set_intersection(arr_1.begin(), arr_1.end(), arr_2.begin(), arr_2.end(),
                        std::back_inserter(intersecting_values));
  return std::make_unique<std::vector<std::string>>(intersecting_values);
}
