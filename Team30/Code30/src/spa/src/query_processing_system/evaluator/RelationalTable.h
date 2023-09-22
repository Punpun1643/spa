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
  std::vector<std::pair<PqlDeclaration, int>> getRenumberedColsAfterRemoval(
      std::vector<PqlDeclaration> const& to_remove);

 public:
  RelationalTable(PqlDeclaration const& d,
                  std::vector<std::string> const& values);
  RelationalTable(PqlDeclaration const& d1, PqlDeclaration const& d2,
                  std::vector<std::string> const& d1_values,
                  std::vector<std::string> const& d2_values);

  std::vector<std::string> getTableCol(PqlDeclaration const& d);

  int getNumCols();

  bool hasNoResults();

  void join(RelationalTable& other_table);
};