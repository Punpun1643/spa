#include "RelationalTable.h"

#include "ArrayUtility.h"
#include <unordered_set>

RelationalTable::RelationalTable(const PqlDeclaration & d,
  const std::vector<std::string> & values) {
  column_mapping[d] = 0;
  for (auto &value: values) {
    table.push_back({value});
  }
}

RelationalTable::RelationalTable(const PqlDeclaration& d1, const PqlDeclaration& d2,
const std::vector<std::string> &d1_values,
const std::vector<std::string> &d2_values) {
  assert(d1_values.size() == d2_values.size());

  column_mapping[d1] = 0;
  column_mapping[d2] = 1;

  for (auto i = 0; i < d1_values.size(); i++) {
    table.push_back({d1_values[i], d2_values[i]});
  }
}

std::vector<std::string> RelationalTable::getTableCol(const PqlDeclaration& d) {
  assert(column_mapping.count(d) == 1);
  int col_idx = column_mapping[d];

  std::vector<std::string> output = {};
  for (auto& row: table) {
    output.push_back(row[col_idx]);
  }
  return output;
}

std::vector<PqlDeclaration> RelationalTable::getSharedColumns(RelationalTable& other_table) {
  std::vector<PqlDeclaration> shared_cols = {};
  for (const auto& [key, value] : other_table.column_mapping) {
    if (column_mapping.count(key) == 1) {
      shared_cols.push_back(key);
    }
  }
  return shared_cols;
}

void RelationalTable::sortTableRows(const PqlDeclaration & sort_col) {
  /**
   * Sorts the table in place by the specified column.
   */
  assert(column_mapping.count(sort_col) == 1);
  int col_idx = column_mapping[sort_col];

  std::sort(table.begin(), table.end(),
            [&](std::vector<std::string> row_a, std::vector<std::string> row_b) {
        return row_a[col_idx] < row_b[col_idx];});
}

int RelationalTable::getNumCols() {
  return (int) column_mapping.size();
}


void RelationalTable::join(RelationalTable& other_table) {
  /**
   * Joins this table with the given table by all shared columns
   */
  // get shared columns
  auto shared_cols = getSharedColumns(other_table);
  assert(!shared_cols.empty());  // block doing a cross-product

  std::vector<std::vector<std::string>> new_table;
  for (auto this_row : table) {
    for (auto other_row : other_table.table) {
      // check that all the shared_cols match
      bool all_shared_cols_match = true;
      for (auto& shared_col : shared_cols) {
        auto this_table_col_idx = column_mapping[shared_col];
        auto other_table_col_idx = other_table.column_mapping[shared_col];
        if (this_row[this_table_col_idx] != other_row[other_table_col_idx]) {
          all_shared_cols_match = false;
          break;
        }
      }
      if (!all_shared_cols_match) {
        continue;
      }
      // Create new joined row
      std::vector<std::string> new_row = {};
      // Add all values from existing row
      new_row.insert(new_row.begin(), this_row.begin(), this_row.end());
      // Add values from other table's row that don't overlap with this row's
      std::unordered_set<int> rows_to_avoid;
      for (auto& shared_col : shared_cols) {
        rows_to_avoid.insert(column_mapping[shared_col]);
      }
      for (auto i = 0; i < other_row.size(); i++) {
        if (rows_to_avoid.count(i) == 1) {
          continue;
        }
        new_row.push_back(other_row[i]);
      }
      new_table.push_back(new_row);
    }
  }

  // Update column mappings
  auto renumbered_cols = other_table.getRenumberedColsAfterRemoval(shared_cols);
  int original_num_cols = getNumCols();
  for (auto& pair : renumbered_cols) {
    PqlDeclaration decl = pair.first;
    int idx = pair.second;
    column_mapping[decl] = original_num_cols + idx;
  }
}

std::vector<std::pair<PqlDeclaration, int>>
    RelationalTable::getRenumberedColsAfterRemoval(const std::vector<PqlDeclaration>& to_remove) {
  std::vector<std::pair<PqlDeclaration, int>> table_mapping_vector;
  for (auto& [key, index]: column_mapping) {
    if (std::find(to_remove.begin(), to_remove.end(), key) == to_remove.end()) {
      table_mapping_vector.push_back(std::make_pair(key, index));
    }
  }
  // sort table_mapping_vector by idx
  std::sort(table_mapping_vector.begin(), table_mapping_vector.end(),
            [&](std::pair<PqlDeclaration, int> a, std::pair<PqlDeclaration, int> b)
            {return a.second < b.second;});

  // renumber in order
  int i = 0;
  for (auto &pair: table_mapping_vector) {
    pair.second = i;
    i++;
  }
  return table_mapping_vector;
}

bool RelationalTable::hasNoResults() {
  return table.empty();
}
