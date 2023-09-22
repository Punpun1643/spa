#pragma once
#include <iostream>
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
  // void filterTableRows(const PqlDeclaration& sort_col);
  // void sortTableRows(const PqlDeclaration& sort_col);
  std::vector<std::pair<PqlDeclaration, int>> getRenumberedColsAfterRemoval(
      std::vector<PqlDeclaration> const& to_remove);

 public:
  // Constructor with 1 value, or two values
  RelationalTable(PqlDeclaration const& d,
                  std::vector<std::string> const& values);
  RelationalTable(PqlDeclaration const& d1, PqlDeclaration const& d2,
                  std::vector<std::string> const& d1_values,
                  std::vector<std::string> const& d2_values);

  std::vector<std::string> getTableCol(PqlDeclaration const& d);

  int getNumCols();
  //
  //  void filter(const PqlDeclaration& d,
  //              const std::vector<std::string>& values);
  //
  void join(RelationalTable& other_table);

  bool hasNoResults();

  void printTable() {
    for (auto row : table) {
      std::cout << "Row:";
      for (auto elem : row) {
        std::cout << elem << "    ";
      }
      std::cout << "\n";
    }
  }
};