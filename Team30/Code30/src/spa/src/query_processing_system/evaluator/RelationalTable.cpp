#include "RelationalTable.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "ArrayUtility.h"

RelationalTable::RelationalTable(PqlDeclaration const& d,
                                 std::vector<std::string> const& values) {
  column_mapping[d] = 0;
  for (auto& value : values) {
    table.push_back({value});
  }
}

RelationalTable::RelationalTable(PqlDeclaration const& d1,
                                 PqlDeclaration const& d2,
                                 std::vector<std::string> const& d1_values,
                                 std::vector<std::string> const& d2_values) {
  assert(!(d1 == d2));
  assert(d1_values.size() == d2_values.size());

  column_mapping[d1] = 0;
  column_mapping[d2] = 1;

  for (auto i = 0; i < d1_values.size(); i++) {
    table.push_back({d1_values[i], d2_values[i]});
  }
}

std::vector<std::string> RelationalTable::getTableCol(PqlDeclaration const& d) {
  assert(column_mapping.count(d) == 1);
  int col_idx = column_mapping[d];

  std::vector<std::string> output = {};
  for (auto& row : table) {
    output.push_back(row[col_idx]);
  }
  return output;
}

std::vector<PqlDeclaration> RelationalTable::getSharedColumns(
    RelationalTable& other_table) {
  std::vector<PqlDeclaration> shared_cols = {};
  for (auto const& [key, value] : other_table.column_mapping) {
    if (column_mapping.count(key) == 1) {
      shared_cols.push_back(key);
    }
  }
  return shared_cols;
}

int RelationalTable::getNumCols() { return (int)column_mapping.size(); }

bool RelationalTable::checkIfRowsMatch(
    std::vector<std::string> const& row,
    std::vector<std::string> const& other_row,
    std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> const&
        other_table_idx_mappings,
    std::vector<PqlDeclaration> const& cols_to_compare) {
  bool all_match = true;
  for (auto& value : cols_to_compare) {
    auto row_idx = column_mapping.at(value);
    auto other_row_idx = other_table_idx_mappings.at(value);
    all_match = all_match && (row[row_idx] == other_row[other_row_idx]);
  }
  return all_match;
}

std::vector<std::string> RelationalTable::getCombinedRows(
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
RelationalTable::getRenumberedColsAfterRemoval(
    std::vector<PqlDeclaration> const& to_remove) {
  /**
   * Removes the given columns from the table, and returns the newly renumbered
   * column indices
   */
  std::vector<std::pair<PqlDeclaration, int>> table_mapping_vector;
  for (auto& [key, index] : column_mapping) {
    if (std::find(to_remove.begin(), to_remove.end(), key) == to_remove.end()) {
      table_mapping_vector.push_back(std::make_pair(key, index));
    }
  }
  // sort table_mapping_vector by idx
  std::sort(
      table_mapping_vector.begin(), table_mapping_vector.end(),
      [&](std::pair<PqlDeclaration, int> a, std::pair<PqlDeclaration, int> b) {
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

void RelationalTable::join(RelationalTable& other_table) {
  /**
   * Joins this table with the given table by all shared columns
   */
  // get shared columns
  auto shared_cols = getSharedColumns(other_table);
  assert(!shared_cols.empty());  // block doing a cross-product

  std::vector<std::vector<std::string>> new_table;
  // For every pair of rows
  for (auto const& this_row : table) {
    for (auto const& other_row : other_table.table) {
      bool all_match = checkIfRowsMatch(
          this_row, other_row, other_table.column_mapping, shared_cols);
      if (!all_match) {
        continue;  // skip this one
      }
      // Add values from other table's row that don't overlap with this row's
      std::unordered_set<int> row_idx_to_avoid;
      for (auto& shared_col : shared_cols) {
        row_idx_to_avoid.insert(other_table.column_mapping[shared_col]);
      }
      auto new_row = getCombinedRows(this_row, other_row, row_idx_to_avoid);
      new_table.push_back(new_row);
    }
  }
  table = new_table;

  // Update column mappings
  auto renumbered_cols = other_table.getRenumberedColsAfterRemoval(shared_cols);
  int original_num_cols = getNumCols();
  for (auto& [decl, idx] : renumbered_cols) {
    column_mapping[decl] = original_num_cols + idx;
  }
}

bool RelationalTable::hasNoResults() { return table.empty(); }

std::vector<PqlDeclaration> RelationalTable::getTableColNames() {
  std::vector<PqlDeclaration> table_col_names;
  for (auto& [key, value] : column_mapping) {
    table_col_names.push_back(key);
  }
  return table_col_names;
}
