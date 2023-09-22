#include "QueryEvaluator.h"

#include <algorithm>
#include <iterator>

#include "IntermediateResultsTable.h"

QueryEvaluator::QueryEvaluator(PkbApi& pkb) : pkb(pkb) {}


std::vector<std::string> QueryEvaluator::evaluateQuery(std::unique_ptr<SelectClause> select_clause,
                                                       std::vector<Clause> other_clauses) {
  assert(select_clause != nullptr);

  auto target_declaration = select_clause->getDeclaration();
  auto select_clause_result = select_clause->evaluate(pkb);

  IntermediateResultsTable table = IntermediateResultsTable();
  table.addClauseResult(*select_clause_result);

  for (auto & other_clause : other_clauses) {
    auto clause_result = other_clause.evaluate(pkb);
    table.addClauseResult(*clause_result);
    if (table.hasNoResults()) { // shortcut
      return {};
    }
  }
  return table.getValuesGivenDeclaration(*target_declaration);
}
