#include "IntermediateResultsTable.h"

#include <cassert>
#include <string>
#include <vector>

#include "shared/ArrayUtility.h"

IntermediateResultsTable::IntermediateResultsTable() = default;

void IntermediateResultsTable::AddClauseResult(
    ClauseResult const& clause_result) {
  if (has_no_results) {
    return;  // don't bother, stuck at false already
  }

  switch (clause_result.GetNumDeclarations()) {
    case (0): {
      bool result = clause_result.GetBooleanClauseValue();
      AddBooleanClauseResult(result);
      break;
    }
    case (1): {
      auto declaration = clause_result.GetDeclarations().front();
      auto values = clause_result.GetValues(declaration);
      AddSingleDeclaration(declaration, values);
      break;
    }
    case (2): {
      auto declarations = clause_result.GetDeclarations();
      assert(declarations.size() == 2);
      auto d1 = declarations[0];
      auto d2 = declarations[1];
      auto d1_values = clause_result.GetValues(d1);
      auto d2_values = clause_result.GetValues(d2);
      AddPairedDeclarations(d1, d2, d1_values, d2_values);
      break;
    }
    default: {
      break;
    }
  }
}

bool IntermediateResultsTable::HasNoResults() const {
  return has_no_results;
}

std::vector<std::vector<std::string>>
IntermediateResultsTable::GetValuesGivenDeclarations(
    std::vector<PqlDeclaration> const& decls) {
  assert(!decls.empty());

  // This check must come first, because decls are technically
  // not added to a table that already has no results.
  if (has_no_results) {
    return {};  // empty values
  }

  for (auto& decl : decls) {
    assert(HasDeclaration(decl));
  }

  // Merge all the declarations into a single table: possible source of future
  // optimisation.
  int table_idx = table_mapping[decls[0]];
  for (auto& decl : decls) {
    if (table_mapping[decl] != table_idx) {
      MergeExistingTables(table_idx, table_mapping[decl], true);
    }
  }

  // Retrieve the paired-return from that single table.
  RelationalTable relevant_table = tables[table_idx];
  return relevant_table.GetTableCols(decls);
}

void IntermediateResultsTable::AddBooleanClauseResult(bool result) {
  if (!result) {
    has_no_results = true;
  }  // otherwise, do nothing
}

void IntermediateResultsTable::AddSingleDeclaration(
    PqlDeclaration const& d, std::vector<std::string> const& values) {
  auto new_table = RelationalTable(d, values);
  // Declaration doesn't exist
  if (table_mapping.count(d) == 0) {
    tables.push_back(new_table);
    table_mapping[d] = static_cast<int>(tables.size()) - 1;
  } else {
    int table_idx = table_mapping[d];
    tables[table_idx].Join(new_table);
    if (tables[table_idx].HasNoResults()) {
      has_no_results = true;
      return;
    }
  }
}

void IntermediateResultsTable::AddPairedDeclarations(
    PqlDeclaration const& d1, PqlDeclaration const& d2,
    std::vector<std::string> const& new_d1_values,
    std::vector<std::string> const& new_d2_values) {
  RelationalTable new_table =
      RelationalTable(d1, d2, new_d1_values, new_d2_values);

  // Both declarations not in table
  if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 0) {
    tables.push_back(new_table);
    table_mapping[d1] = static_cast<int>(tables.size()) - 1;
    table_mapping[d2] = static_cast<int>(tables.size()) - 1;

  } else if (table_mapping.count(d1) == 1 && table_mapping.count(d2) == 0) {
    int d1_table_idx = table_mapping[d1];
    tables[d1_table_idx].Join(new_table);
    table_mapping[d2] = d1_table_idx;

  } else if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 1) {
    int d2_table_idx = table_mapping[d2];
    tables[d2_table_idx].Join(new_table);
    table_mapping[d1] = d2_table_idx;

  } else {
    // case 1: d1 and d2 in same table already
    if (table_mapping[d1] == table_mapping[d2]) {
      int table_idx = table_mapping[d1];
      tables[table_idx].Join(new_table);
    } else {
      // case 2: d1 and d2 are in different tables
      int d1_table_idx = table_mapping[d1];
      int d2_table_idx = table_mapping[d2];
      tables[d1_table_idx].Join(new_table);
      MergeExistingTables(d1_table_idx, d2_table_idx);
    }
  }
  assert(table_mapping.count(d1) ==
         1);  // otherwise, doing [] will insert a default key-value pair.
  if (tables[table_mapping[d1]].HasNoResults()) {
    has_no_results = true;
    return;
  }
}

bool IntermediateResultsTable::HasDeclaration(
    PqlDeclaration const& declaration) const {
  return table_mapping.count(declaration) == 1;
}

void IntermediateResultsTable::MergeExistingTables(int table_to_keep_id,
                                                   int table_to_merge_id,
                                                   bool allow_cross_product) {
  tables[table_to_keep_id].Join(tables[table_to_merge_id], allow_cross_product);
  // Update table_mappings lazily (without removing old table)
  // May want to remove old table in the future to reduce mem usage
  for (auto const& name : tables[table_to_merge_id].GetTableColNames()) {
    table_mapping[name] = table_to_keep_id;
  }
}
