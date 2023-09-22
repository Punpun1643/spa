#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "query_processing_system/common/PqlDeclaration.h"

class RelationalTable {
  /**
   * Encapsulates a 2D table of data, with labels and values.
   */
 private:
  std::vector<std::vector<std::string>> table;
  std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> column_mapping;

  std::vector<PqlDeclaration> getSharedColumns(RelationalTable& other_table);
  void filterTableRows(const PqlDeclaration& sort_col);
  void sortTableRows(const PqlDeclaration& sort_col);

 public:
  // Constructor with 1 value, or two values
  RelationalTable(const PqlDeclaration& d,
                            const std::vector<std::string>& values);
  RelationalTable(const PqlDeclaration& d1, const PqlDeclaration& d2,
                  const std::vector<std::string> &d1_values,
                  const std::vector<std::string> &d2_values);

  std::vector<std::string> getTableCol(const PqlDeclaration &d);

  int getNumCols();
//
//  void filter(const PqlDeclaration& d,
//              const std::vector<std::string>& values);
//
  void join(RelationalTable& other_table, const PqlDeclaration& join_col);

  void join(RelationalTable& other_table, const PqlDeclaration& join_col_1,
            const PqlDeclaration& join_col_2);

  bool hasNoResults();




};