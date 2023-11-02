#include "QueryEvaluator.h"

#include <algorithm>
#include <stdexcept>

#include "shared/ArrayUtility.h"

QueryEvaluator::QueryEvaluator(PKBQPSInterface& pkb) : pkb(pkb) {}

void QueryEvaluator::PopulateIntermediateResultsTable(
    IntermediateResultsTable& table, ClauseList clauses) {
  for (auto& clause : clauses) {
    auto clause_result = clause->Evaluate(pkb);
    if (clause->IsNotClause()) {
      auto decls = clause_result->GetDeclarations();
      FillMissingDecls(table, decls);
      table.RemoveClauseResult(*clause_result);
    } else {
      table.AddClauseResult(*clause_result);
    }
    if (table.HasNoResults()) {
      break;  // no point continuing
    }
  }
}

bool QueryEvaluator::EvaluateQuery(ClauseList clauses) {
  auto table = IntermediateResultsTable();
  PopulateIntermediateResultsTable(table, clauses);
  return !table.HasNoResults();
}

std::vector<PqlDeclaration> QueryEvaluator::UnwrapAttrRefVector(
    std::vector<AttrRef> const& attr_refs) {
  std::vector<PqlDeclaration> decls = {};
  for (auto const& attr_ref : attr_refs) {
    decls.push_back(attr_ref.GetDecl());
  }
  return decls;
}

void QueryEvaluator::FillMissingDecls(
    IntermediateResultsTable& table,
    std::vector<PqlDeclaration> const& decls_to_check) {
  std::vector<std::shared_ptr<Clause>> missing_decl_clauses = {};
  for (auto const& decl : decls_to_check) {
    if (!table.HasDeclaration(decl)) {
      auto select_all_clause = std::make_shared<SelectAllClause>(decl);
      missing_decl_clauses.push_back(select_all_clause);
    }
  }
  PopulateIntermediateResultsTable(table, missing_decl_clauses);
}

bool QueryEvaluator::UpdateResultUsingAttrTypes(
    std::vector<std::vector<std::string>>& values,
    std::vector<AttrRef> const& attr_refs) {
  /**
   * Returns true if the 2D-vector was modified, and false otherwise
   */
  if (!values.empty() && values[0].size() != attr_refs.size()) {
    throw std::invalid_argument(
        "Number of attr_refs does not match number of values");
  }

  std::vector<int> aliased_idx = {};
  for (int i = 0; i < attr_refs.size(); i++) {
    if (attr_refs[i].IsAttrTypeAnAlias()) {
      aliased_idx.push_back(i);
    }
  }

  if (aliased_idx.empty() || values.empty()) {
    return false;
  }

  for (auto& row : values) {
    for (auto i : aliased_idx) {
      AttrRef const& attr_ref = attr_refs[i];
      row[i] = attr_ref.GetAliasFromDefault(pkb, row[i]);
    }
  }
  return true;
}

std::vector<std::vector<std::string>> QueryEvaluator::EvaluateQuery(
    std::vector<AttrRef> const& selected_attr_refs, ClauseList clauses) {
  /**
   * Returns the possible values that each of the selected declarations can take
   * in the form of a 2D vector. Each row represents one set of possible values
   * (e.g. a = 2, b = 3, c = 4 will be {{"2", "3", "4"}})
   */
  if (selected_attr_refs.empty()) {
    throw std::invalid_argument("AttrRef vector cannot be empty.");
  }
  auto table = IntermediateResultsTable();
  PopulateIntermediateResultsTable(table, clauses);

  std::vector<PqlDeclaration> selected_decls =
      UnwrapAttrRefVector(selected_attr_refs);
  FillMissingDecls(table, selected_decls);
  auto values = table.GetValuesGivenDeclarations(selected_decls);

  // Some of the attr_refs might be asking for alternative attrTypes
  bool is_modified = UpdateResultUsingAttrTypes(values, selected_attr_refs);
  if (is_modified) {
    ArrayUtility::RemoveDuplicates(values);
  }
  return values;
}
