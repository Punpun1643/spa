#include "IntermediateResultsTable.h"

#include <cassert>

#include "ArrayUtility.h"

IntermediateResultsTable::IntermediateResultsTable() = default;

void IntermediateResultsTable::addClauseResult(
    ClauseResult const& clause_result) {
  if (has_no_results) {
    return;  // don't bother, stuck at false already
  }

  switch (clause_result.getNumDeclarations()) {
    case (0): {
      bool result = clause_result.getBooleanClauseValue();
      addBooleanClauseResult(result);
      break;
    }
    case (1): {
      auto declaration = clause_result.getDeclarations().front();
      auto values = clause_result.getValues(declaration);
      addSingleDeclaration(declaration, *values);
      break;
    }
    case (2): {
      auto declarations = clause_result.getDeclarations();
      assert(declarations.size() == 2);
      auto d1 = declarations[0];
      auto d2 = declarations[1];
      auto d1_values = clause_result.getValues(d1);
      auto d2_values = clause_result.getValues(d2);
      addPairedDeclarations(d1, d2, *d1_values, *d2_values);
      break;
    }
    default: {
      break;
    }
  }
}

bool IntermediateResultsTable::hasNoResults() const { return has_no_results; }

std::vector<std::vector<std::string>> IntermediateResultsTable::getValuesGivenDeclarations(
    std::vector<PqlDeclaration> const& decls) {
  assert(!decls.empty());

  // This check must come first, because decls are technically
  // not added to a table that already has no results.
  if (has_no_results) {
    return {};  // empty values
  }

  for (auto& decl: decls) {
    assert(hasDeclaration(decl));
  }

  // Merge all the declarations into a single table: possible source of future optimisation.
  int table_idx = table_mapping[decls[0]];
  for (auto& decl: decls) {
    if (table_mapping[decl] != table_idx) {
      mergeExistingTables(table_idx, table_mapping[decl], true);
    }
  }

  // Retrieve the paired-return from that single table.
  RelationalTable relevant_table = tables[table_idx];
  return relevant_table.getTableCols(decls);
}

void IntermediateResultsTable::addBooleanClauseResult(bool result) {
  if (!result) {
    has_no_results = true;
  }  // otherwise, do nothing
}

void IntermediateResultsTable::addSingleDeclaration(
    PqlDeclaration const& d, std::vector<std::string> const& values) {
  auto new_table = RelationalTable(d, values);
  // Declaration doesn't exist
  if (table_mapping.count(d) == 0) {
    tables.push_back(new_table);
    table_mapping[d] = (int)tables.size() - 1;
  } else {
    int table_idx = table_mapping[d];
    tables[table_idx].join(new_table);
    if (tables[table_idx].hasNoResults()) {
      has_no_results = true;
      return;
    }
  }
}

void IntermediateResultsTable::addPairedDeclarations(
    PqlDeclaration const& d1, PqlDeclaration const& d2,
    std::vector<std::string> const& new_d1_values,
    std::vector<std::string> const& new_d2_values) {
  RelationalTable new_table =
      RelationalTable(d1, d2, new_d1_values, new_d2_values);

  // Both declarations not in table
  if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 0) {
    tables.push_back(new_table);
    table_mapping[d1] = (int)tables.size() - 1;
    table_mapping[d2] = (int)tables.size() - 1;

  } else if (table_mapping.count(d1) == 1 && table_mapping.count(d2) == 0) {
    int d1_table_idx = table_mapping[d1];
    tables[d1_table_idx].join(new_table);
    table_mapping[d2] = d1_table_idx;

  } else if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 1) {
    int d2_table_idx = table_mapping[d2];
    tables[d2_table_idx].join(new_table);
    table_mapping[d1] = d2_table_idx;

  } else {
    // case 1: d1 and d2 in same table already
    if (table_mapping[d1] == table_mapping[d2]) {
      int table_idx = table_mapping[d1];
      tables[table_idx].join(new_table);
    } else {
      // case 2: d1 and d2 are in different tables
      int d1_table_idx = table_mapping[d1];
      int d2_table_idx = table_mapping[d2];
      tables[d1_table_idx].join(new_table);
      mergeExistingTables(d1_table_idx, d2_table_idx);
    }
  }
  assert(table_mapping.count(d1) ==
         1);  // otherwise, doing [] will insert a default key-value pair.
  if (tables[table_mapping[d1]].hasNoResults()) {
    has_no_results = true;
    return;
  }
}

bool IntermediateResultsTable::hasDeclaration(PqlDeclaration const& declaration) const{
  return table_mapping.count(declaration) == 1;
}

void IntermediateResultsTable::mergeExistingTables(int table_to_keep_id, int table_to_merge_id, bool allow_cross_product) {
  tables[table_to_keep_id].join(tables[table_to_merge_id], allow_cross_product);
  // Update table_mappings lazily (without removing old table)
  // May want to remove old table in the future to reduce mem usage
  for (auto const& name : tables[table_to_merge_id].getTableColNames()) {
    table_mapping[name] = table_to_keep_id;
  }
}
