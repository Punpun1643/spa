#include "QueryEvaluator.h"

#include <algorithm>
#include <cassert>
#include <iterator>

QueryEvaluator::QueryEvaluator(PKBQPSInterface& pkb) : pkb(pkb) {}

void QueryEvaluator::populateIntermediateResultsTable(
    IntermediateResultsTable& table, ClauseList clauses) {
  for (auto& clause : clauses) {
    auto clause_result = clause->evaluate(pkb);
    table.addClauseResult(*clause_result);
    if (table.hasNoResults()) {
      break;  // no point continuing
    }
  }
}

bool QueryEvaluator::evaluateQuery(ClauseList clauses) {
  auto table = IntermediateResultsTable();
  populateIntermediateResultsTable(table, clauses);
  return !table.hasNoResults();
}

std::vector<std::vector<std::string>> QueryEvaluator::evaluateQuery(
    std::vector<AttrRef> const& selected_attr_refs, ClauseList clauses) {
  /**
   * Returns the possible values that each of the selected declarations can take
   * in the form of a 2D vector. Each row represents one set of possible values
   * (e.g. a = 2, b = 3, c = 4 will be {{"2", "3", "4"}})
   */
  assert(!selected_attr_refs.empty());
  auto table = IntermediateResultsTable();
  populateIntermediateResultsTable(table, clauses);

  // Get declaration vector
  std::vector<PqlDeclaration> selected_decls = {};
  for (auto const& attr_ref: selected_attr_refs) {
    selected_decls.push_back(attr_ref.getDecl());
  }

  // Fill in missing declarations
  std::vector<std::shared_ptr<Clause>> missing_decl_clauses = {};
  for (auto const& decl : selected_decls) {
    if (!table.hasDeclaration(decl)) {
      auto select_all_clause = std::make_shared<SelectAllClause>(decl);
      missing_decl_clauses.push_back(select_all_clause);
    }
  }
  populateIntermediateResultsTable(table, missing_decl_clauses);

  auto values = table.getValuesGivenDeclarations(selected_decls);
  // Convert values if need be
  for (auto& row: values) {
    for (int i=0; i < row.size(); i++) {
      const AttrRef& attr_ref = selected_attr_refs[i];
      if (attr_ref.isAttrTypeAnAlias()) {
        std::string new_value = pkb.convertEntityAttribute(row[i],
                                                           attr_ref.getEntityType(),
                                                           attr_ref.getDefaultAttrType(),
                                                           attr_ref.getAttrType());
        row[i] = new_value;
      }
    }
  }
  return values;
}
