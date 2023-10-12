#include "QueryEvaluator.h"
#include "ArrayUtility.h"

#include <algorithm>
#include <cassert>

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

std::vector<PqlDeclaration> QueryEvaluator::unwrapAttrRefVector(std::vector<AttrRef> const& attr_refs) {
  std::vector<PqlDeclaration> decls = {};
  for (auto const& attr_ref: attr_refs) {
    decls.push_back(attr_ref.getDecl());
  }
  return decls;
}

void QueryEvaluator::fillMissingDecls(IntermediateResultsTable& table, std::vector<PqlDeclaration> const& decls_to_check) {
  std::vector<std::shared_ptr<Clause>> missing_decl_clauses = {};
  for (auto const& decl : decls_to_check) {
    if (!table.hasDeclaration(decl)) {
      auto select_all_clause = std::make_shared<SelectAllClause>(decl);
      missing_decl_clauses.push_back(select_all_clause);
    }
  }
  populateIntermediateResultsTable(table, missing_decl_clauses);
}

bool QueryEvaluator::updateResultUsingAttrTypes(std::vector<std::vector<std::string>> &values, std::vector<AttrRef> const& attr_refs) {
  /**
   * Returns true if the 2D-vector was modified, and false otherwise
   */
  if (!values.empty()) {
    assert(values[0].size() == attr_refs.size()); // assume not ragged 2D vec
  }

  std::vector<int> aliased_idx = {};
  for (int i = 0; i < attr_refs.size(); i++) {
    if (attr_refs[i].isAttrTypeAnAlias()) {
      aliased_idx.push_back(i);
    }
  }

  if (aliased_idx.empty() || values.empty()) {
    return false;
  }

  for (auto& row: values) {
    for (auto i : aliased_idx) {
      const AttrRef& attr_ref = attr_refs[i];
      row[i] = pkb.convertEntityAttribute(row[i], attr_ref.getEntityType(),
                                          attr_ref.getDefaultAttrType(),
                                          attr_ref.getAttrType());
    }
  }
  return true;
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

  std::vector<PqlDeclaration> selected_decls = unwrapAttrRefVector(selected_attr_refs);
  fillMissingDecls(table, selected_decls);
  auto values = table.getValuesGivenDeclarations(selected_decls);

  // Some of the attr_refs might be asking for alternative attrTypes
  bool is_modified = updateResultUsingAttrTypes(values, selected_attr_refs);
  if (is_modified) {
    ArrayUtility::removeDuplicates(values);
  }
  return values;
}
