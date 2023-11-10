#include "RelationalTable.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <utility>

#include "shared/ArrayUtility.h"

RelationalTable::RelationalTable(PqlDeclaration const& d,
                                 std::vector<std::string> const& values) {
  column_mapping[d] = 0;
  for (auto& value : values) {
    table.push_back({value});
  }
}

RelationalTable::RelationalTable(
    PqlDeclaration const& d1, PqlDeclaration const& d2,
    std::vector<std::pair<std::string, std::string>> const& paired_values) {
  if (d1 == d2) {
    throw std::invalid_argument("Error: Declarations are identical");
  }
  column_mapping[d1] = 0;
  column_mapping[d2] = 1;

  for (auto const& paired_value : paired_values) {
    table.push_back({paired_value.first, paired_value.second});
  }
}

std::vector<std::vector<std::string>> RelationalTable::GetTableCols(
    std::vector<PqlDeclaration> const& decls) const {
  if (decls.empty()) {
    return {};
  }
  for (auto& d : decls) {
    if (column_mapping.count(d) == 0) {
      throw std::invalid_argument("Given declaration not in table.");
    }
  }
  std::vector<std::vector<std::string>> output = {};
  // Construct row by row
  for (auto const& row : table) {
    std::vector<std::string> output_row = {};
    for (auto& d : decls) {
      int col_idx = column_mapping.at(d);
      output_row.push_back(row[col_idx]);
    }
    output.push_back(output_row);
  }
  ArrayUtility::RemoveDuplicates(output);
  return output;
}

std::vector<PqlDeclaration> RelationalTable::GetSharedColumns(
    RelationalTable& other_table) const {
  std::vector<PqlDeclaration> shared_cols = {};
  for (auto const& [key, value] : other_table.column_mapping) {
    if (column_mapping.count(key) == 1) {
      shared_cols.push_back(key);
    }
  }
  return shared_cols;
}

int RelationalTable::GetNumCols() const {
  return static_cast<int>(column_mapping.size());
}

bool RelationalTable::CheckIfRowsMatch(
    std::vector<std::string> const& row,
    std::vector<std::string> const& other_row,
    std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> const&
        other_row_idx_mappings,
    std::vector<PqlDeclaration> const& values_to_compare) const {
  bool all_match = true;
  for (auto& value : values_to_compare) {
    auto row_idx = column_mapping.at(value);
    auto other_row_idx = other_row_idx_mappings.at(value);
    all_match = all_match && (row[row_idx] == other_row[other_row_idx]);
  }
  return all_match;
}

std::vector<std::string> RelationalTable::GetCombinedRows(
    std::vector<std::string> const& row_1,
    std::vector<std::string> const& row_2,
    std::unordered_set<int> const& skipped_idx_in_row_2) {
  /**
   * Returns a row combining the values from two rows, with the given indices
   * in row_2 being skipped. The combined row will maintain the ordering of the
   * original rows, and contain the values from row_1 before the values in
   * row_2.
   */
  std::vector<std::string> new_row = {};
  // Add all values from row 1
  new_row.insert(new_row.begin(), row_1.begin(), row_1.end());
  // Add selected values from row 2
  for (auto i = 0; i < row_2.size(); i++) {
    if (skipped_idx_in_row_2.count(i) == 0) {
      new_row.push_back(row_2[i]);
    }
  }
  return new_row;
}

std::vector<std::pair<PqlDeclaration, int>>
RelationalTable::GetRenumberedColsAfterRemoval(
    std::vector<PqlDeclaration> const& to_remove) const {
  /**
   * Removes the given columns from the table, and returns the newly renumbered
   * column indices
   */
  std::vector<std::pair<PqlDeclaration, int>> table_mapping_vector;
  for (auto& [key, index] : column_mapping) {
    if (std::find(to_remove.begin(), to_remove.end(), key) == to_remove.end()) {
      table_mapping_vector.emplace_back(key, index);
    }
  }
  // sort table_mapping_vector by idx
  std::sort(table_mapping_vector.begin(), table_mapping_vector.end(),
            [&](std::pair<PqlDeclaration, int> const& a,
                std::pair<PqlDeclaration, int> const& b) {
              return a.second < b.second;
            });

  // renumber in order
  int i = 0;
  for (auto& pair : table_mapping_vector) {
    pair.second = i;
    i++;
  }
  return table_mapping_vector;
}

void RelationalTable::Join(RelationalTable& other_table,
                           bool allow_cross_product) {
  /**
   * Joins this table with the given table by all shared columns
   */
  // get shared columns
  auto shared_cols = GetSharedColumns(other_table);
  if (!allow_cross_product && shared_cols.empty()) {
    throw std::invalid_argument(
        "Given table has no shared cols with existing table and cross-products "
        "are not allowed");
  }

  std::vector<std::vector<std::string>> new_table;
  // For every pair of rows
  for (auto const& this_row : table) {
    for (auto const& other_row : other_table.table) {
      bool all_match = CheckIfRowsMatch(
          this_row, other_row, other_table.column_mapping, shared_cols);
      if (!all_match) {
        continue;  // skip this one
      }
      // Add values from other table's row that don't overlap with this row's
      std::unordered_set<int> row_idx_to_avoid;
      for (auto& shared_col : shared_cols) {
        row_idx_to_avoid.insert(other_table.column_mapping[shared_col]);
      }
      auto new_row = GetCombinedRows(this_row, other_row, row_idx_to_avoid);
      new_table.push_back(new_row);
    }
  }
  table = new_table;

  // Update column mappings
  auto renumbered_cols = other_table.GetRenumberedColsAfterRemoval(shared_cols);
  int original_num_cols = GetNumCols();
  for (auto& [decl, idx] : renumbered_cols) {
    column_mapping[decl] = original_num_cols + idx;
  }
}

void RelationalTable::Delete(PqlDeclaration const& decl,
                             std::unordered_set<std::string> const& values) {
  if (column_mapping.count(decl) == 0) {
    throw std::invalid_argument(
        "Declaration with values to be deleted is not present in table.");
  }
  int target_col = column_mapping.at(decl);
  // erase-remove idiom
  table.erase(std::remove_if(table.begin(), table.end(),
                             [&](std::vector<std::string> const& row) {
                               return values.count(row.at(target_col)) == 1;
                             }),
              table.end());
}

void RelationalTable::Delete(
    PqlDeclaration const& d1, PqlDeclaration const& d2,
    std::unordered_set<std::pair<std::string, std::string>, PairHash> const&
        values) {
  if (column_mapping.count(d1) == 0 || column_mapping.count(d2) == 0) {
    throw std::invalid_argument(
        "Declaration with values to be deleted is not present in table.");
  }
  int target_col_1 = column_mapping.at(d1);
  int target_col_2 = column_mapping.at(d2);
  table.erase(std::remove_if(table.begin(), table.end(),
                             [&](std::vector<std::string> const& row) {
                               auto pair = std::make_pair(row.at(target_col_1),
                                                          row.at(target_col_2));
                               return values.count(pair) == 1;
                             }),
              table.end());
}

void RelationalTable::Filter(std::unordered_set<PqlDeclaration, PqlDeclarationHash> const& decls_to_keep) {
  // Not all decls_to_keep are in the table.
  std::vector<PqlDeclaration> decls_to_remove;
  std::vector<int> column_idx_to_keep;
  for (auto const& [decl, index]: column_mapping) {
    if (decls_to_keep.count(decl) == 0) {
      decls_to_remove.push_back(decl);
    } else {
      column_idx_to_keep.push_back(index);
    }
  }
  std::sort(column_idx_to_keep.begin(), column_idx_to_keep.end());

  if (decls_to_remove.empty()) {
    return;
  }

  // Recreate the table with those decls removed. Update decls.
  std::vector<std::vector<std::string>> new_table;
  for (auto& row: table) {
    std::vector<std::string> new_row = {};
    for (int i: column_idx_to_keep) {
      new_row.push_back(row[i]);
    }
    new_table.push_back(new_row);
  }

  // Remove duplicates in the table.
  ArrayUtility::RemoveDuplicates(new_table);
  table = new_table;

  // Update col mapping in table.
  auto renumbered_cols = GetRenumberedColsAfterRemoval(decls_to_remove);
  column_mapping.clear();
  for (auto& [decl, idx] : renumbered_cols) {
    column_mapping[decl] = idx;
  }
}

bool RelationalTable::HasNoResults() const {
  return table.empty();
}

std::vector<PqlDeclaration> RelationalTable::GetTableColNames() const {
  std::vector<PqlDeclaration> table_col_names;
  for (auto& [key, value] : column_mapping) {
    table_col_names.push_back(key);
  }
  return table_col_names;
}

void RelationalTable::Clear() {
  table.clear();
  column_mapping.clear();
}
