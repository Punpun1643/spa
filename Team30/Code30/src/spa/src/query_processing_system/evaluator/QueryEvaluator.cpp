#include "QueryEvaluator.h"

#include <algorithm>
#include <cassert>
#include <iterator>

QueryEvaluator::QueryEvaluator(PkbApi& pkb)
    : pkb(pkb) {}

void QueryEvaluator::populateIntermediateResultsTable(IntermediateResultsTable& table, ClauseList clauses) {
  for (auto& clause : clauses) {
    auto clause_result = clause->evaluate(pkb);
    table.addClauseResult(*clause_result);
    if (table.hasNoResults()) {
      break; // no point continuing
    }
  }
}

bool QueryEvaluator::evaluateQuery(ClauseList clauses) {
  auto table = IntermediateResultsTable();
  populateIntermediateResultsTable(table, clauses);
  return !table.hasNoResults();
}

std::vector<std::vector<std::string>> QueryEvaluator::evaluateQuery(std::vector<PqlDeclaration> const& selected_decls, ClauseList clauses) {
  /**
   * Returns the possible values that each of the selected declarations can take in the form of a 2D vector.
   * Each internal vector represents the set of possible values (shit but IRT returns no duplicates)
   */
  assert(!selected_decls.empty());
  auto table = IntermediateResultsTable();
  populateIntermediateResultsTable(table, clauses);

  auto missing_decl_clauses = std::vector<std::shared_ptr<Clause>>();

  for (auto const& decl: selected_decls) {
    if (!table.hasDeclaration(decl)) {
      auto all_clause = std::make_shared<SelectClause>(decl);
      missing_decl_clauses.push_back(all_clause);
    }
  }
  populateIntermediateResultsTable(table, missing_decl_clauses);
  return table.getValuesGivenDeclarations(selected_decls);
}

// Legacy method: To be deleted after QP Parser migrates over to new API
std::vector<std::string> QueryEvaluator::evaluateQuery(
    std::unique_ptr<SelectClause> select_clause,
    std::vector<std::shared_ptr<Clause>> const& other_clauses) {
  assert(select_clause != nullptr);

  auto target_declaration = select_clause->getDeclaration();

  auto new_output = evaluateQuery({target_declaration}, other_clauses);

  // Flatten and return
  std::vector<std::string> old_output = {};
  for (auto& row: new_output) {
    assert(row.size() == 1);
    old_output.push_back(row[0]);
  }
  return old_output;
}
