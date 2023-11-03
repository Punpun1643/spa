#include "IntermediateResultsTable.h"

#include <stdexcept>
#include <string>
#include <vector>

#include "program_knowledge_base/utils/PairHash.h"

IntermediateResultsTable::IntermediateResultsTable() = default;

void IntermediateResultsTable::UpdateNoResultsFlag(int table_idx) {
  if (tables.at(table_idx).HasNoResults()) {
    has_no_results = true;
  }
}

void IntermediateResultsTable::AddClauseResult(
    ClauseResult const& clause_result, bool is_negated) {
  if (has_no_results) {
    return;  // don't bother, stuck at false already
  }

  switch (clause_result.GetNumDeclarations()) {
    case (0): {
      bool result = clause_result.GetBooleanClauseValue();
      if (is_negated) {
        result = !result;
      }
      AddBooleanClauseResult(result);
      break;
    }
    case (1): {
      auto declaration = clause_result.GetDeclarations().front();
      auto values = clause_result.GetValues(declaration);
      if (is_negated) {
        RemoveSingleDeclaration(declaration, values);
      } else {
        AddSingleDeclaration(declaration, values);
      }
      UpdateNoResultsFlag(table_mapping.at(declaration));
      break;
    }
    case (2): {
      auto declarations = clause_result.GetDeclarations();
      auto d1 = declarations[0];
      auto d2 = declarations[1];
      auto d1_values = clause_result.GetValues(d1);
      auto d2_values = clause_result.GetValues(d2);
      if (d1_values.size() != d2_values.size()) {
        throw std::invalid_argument("Decls have differently sized vectors.");
      }
      if (is_negated) {
        RemovePairedDeclaration(d1, d2, d1_values, d2_values);
      } else {
        AddPairedDeclarations(d1, d2, d1_values, d2_values);
      }
      UpdateNoResultsFlag(table_mapping.at(d1));
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
  // This check must come first, because decls are technically
  // not added to a table that already has no results.
  if (decls.empty() || has_no_results) {
    return {};  // empty values
  }

  for (auto& decl : decls) {
    if (!HasDeclaration(decl)) {
      throw std::invalid_argument("Given decl not in table.");
    }
  }

  // Merge all the declarations into a single table: possible source of future
  // optimisation.
  int table_idx = table_mapping.at(decls[0]);
  for (auto& decl : decls) {
    if (table_mapping.at(decl) != table_idx) {
      MergeExistingTables(table_idx, table_mapping.at(decl), true);
    }
  }

  // Retrieve the paired-return from that single table.
  RelationalTable relevant_table = tables[table_idx];
  return relevant_table.GetTableCols(decls);
}

void IntermediateResultsTable::AddBooleanClauseResult(bool result) {
  if (!result) {
    has_no_results = true;
  }
}

void IntermediateResultsTable::AddSingleDeclaration(
    PqlDeclaration const& d, std::vector<std::string> const& values) {
  auto new_table = RelationalTable(d, values);
  // Declaration doesn't exist
  if (table_mapping.count(d) == 0) {
    tables.push_back(new_table);
    table_mapping[d] = static_cast<int>(tables.size()) - 1;
  } else {
    int table_idx = table_mapping.at(d);
    tables[table_idx].Join(new_table);
  }
}

void IntermediateResultsTable::RemoveSingleDeclaration(PqlDeclaration const& d,
                                                       std::vector<std::string> const& values) {
  if (table_mapping.count(d) == 0) {
    throw std::logic_error("Cannot remove values from a declaration that isn't already present");
  }
  int table_idx = table_mapping.at(d);
  std::unordered_set<std::string> value_set(values.begin(), values.end());
  tables[table_idx].Delete(d, value_set);
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
    int d1_table_idx = table_mapping.at(d1);
    tables[d1_table_idx].Join(new_table);
    table_mapping[d2] = d1_table_idx;

  } else if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 1) {
    int d2_table_idx = table_mapping.at(d2);
    tables[d2_table_idx].Join(new_table);
    table_mapping[d1] = d2_table_idx;

  } else {
    // case 1: d1 and d2 in same table already
    if (table_mapping.at(d1) == table_mapping.at(d2)) {
      int table_idx = table_mapping.at(d1);
      tables[table_idx].Join(new_table);
    } else {
      // case 2: d1 and d2 are in different tables
      int d1_table_idx = table_mapping.at(d1);
      int d2_table_idx = table_mapping.at(d2);
      tables[d1_table_idx].Join(new_table);
      MergeExistingTables(d1_table_idx, d2_table_idx);
    }
  }
}

void IntermediateResultsTable::RemovePairedDeclaration(PqlDeclaration const& d1, PqlDeclaration const& d2,
                                                       std::vector<std::string> const& d1_values,
                                                       std::vector<std::string> const& d2_values) {
  if (table_mapping.count(d1) == 0 || table_mapping.count(d2) == 0) {
    throw std::logic_error("Cannot remove values from declarations that aren't already present");
  }

  if (table_mapping.at(d1) != table_mapping.at(d2)) {
    MergeExistingTables(table_mapping.at(d1), table_mapping.at(d2), true);
  }
  std::unordered_set<std::pair<std::string, std::string>, PairHash> value_set;
  for (int i = 0; i < d1_values.size(); i++) {
    value_set.emplace(d1_values[i], d2_values[i]);
  }
  int table_idx = table_mapping.at(d1);
  tables[table_idx].Delete(d1, d2, value_set);
}

bool IntermediateResultsTable::HasDeclaration(
    PqlDeclaration const& declaration) const {
  return table_mapping.count(declaration) == 1;
}

void IntermediateResultsTable::MergeExistingTables(int table_to_keep_id,
                                                   int table_to_merge_id,
                                                   bool allow_cross_product) {
  tables[table_to_keep_id].Join(tables[table_to_merge_id], allow_cross_product);
  // Update table_mappings lazily (without fully removing old table)
  for (auto const& name : tables[table_to_merge_id].GetTableColNames()) {
    table_mapping[name] = table_to_keep_id;
  }
  tables[table_to_merge_id].Clear(); // to save memory
}
