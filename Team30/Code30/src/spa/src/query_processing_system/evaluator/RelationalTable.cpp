#include "RelationalTable.h"

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

void RelationalTable::join(RelationalTable& other_table, const PqlDeclaration & join_col) {
  /**
   * Joins this table with the given table by the specified column.
   */
  // verify that the given col is the only shared column
  auto shared_cols = getSharedColumns(other_table);
  assert(shared_cols.size() == 1 && shared_cols.front() == join_col);

  // sort each table by the given join_column.
  sortTableRows(join_col);
  other_table.sortTableRows(join_col);

  // filter the tables by the join_col
  auto this_table_join_col = getTableCol(join_col);
  auto other_table_join_col = other_table.getTableCol(join_col);
  ArrayUtility::getIndicesToKeep(existing_values, values);

  filter

  // join the tables by the given column
  std::vector<std::vector<std::string>> new_table;


  int this_idx = 0;
  int other_idx = 0;
  while (this_idx < table.size() && other_idx < other_table.table.size()) {
    if (this_table_join_col[this_idx] == other_table_join_col[other_idx]) {

    } else if (this_table_join_col[this_idx] < other_table_join_col[other_idx]) {

    } else {

    }
  }
}

void RelationalTable::join(RelationalTable& other_table, const PqlDeclaration & join_col_1,
                           const PqlDeclaration& join_col_2) {

}

bool RelationalTable::hasNoResults() {

}