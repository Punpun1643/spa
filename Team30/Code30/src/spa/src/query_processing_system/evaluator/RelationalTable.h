#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "program_knowledge_base/utils/PairHashValueValue.h"
#include "query_processing_system/references/PqlDeclaration.h"

class RelationalTable {
  /**
   * Encapsulates a 2D table of data, with labels and values.
   */
 private:
  std::vector<std::vector<std::string>> table;
  std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> column_mapping;

  std::vector<PqlDeclaration> GetSharedColumns(
      RelationalTable& other_table) const;
  std::vector<std::pair<PqlDeclaration, int>> GetRenumberedColsAfterRemoval(
      std::vector<PqlDeclaration> const& to_remove) const;

  bool CheckIfRowsMatch(
      std::vector<std::string> const& row,
      std::vector<std::string> const& other_row,
      std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> const&
          other_row_idx_mappings,
      std::vector<PqlDeclaration> const& values_to_compare) const;

  static std::vector<std::string> GetCombinedRows(
      std::vector<std::string> const& row_1,
      std::vector<std::string> const& row_2,
      std::unordered_set<int> const& skipped_idx_in_row_2);

  void Filter(std::vector<PqlDeclaration> const& decls_to_remove,
              std::vector<int>& column_idx_to_keep);

 public:
  RelationalTable(PqlDeclaration const& d,
                  std::vector<std::string> const& values);
  RelationalTable(
      PqlDeclaration const& d1, PqlDeclaration const& d2,
      std::vector<std::pair<std::string, std::string>> const& paired_values);

  std::vector<std::vector<std::string>> GetTableCols(
      std::vector<PqlDeclaration> const& decls) const;

  std::unordered_set<std::string> GetCol(PqlDeclaration const& decl) const;

  bool HasNoResults() const;

  std::vector<PqlDeclaration> GetTableColNames() const;

  int GetNumCols() const;

  void Join(RelationalTable& other_table, bool allow_cross_product = false);

  void Filter(std::unordered_set<PqlDeclaration, PqlDeclarationHash> const&
                  decls_to_keep);

  void Delete(PqlDeclaration const& decl,
              std::unordered_set<std::string> const& values);

  void Delete(PqlDeclaration const& d1, PqlDeclaration const& d2,
              std::unordered_set<std::pair<std::string, std::string>,
                                 PairHashValueValue> const& values);

  void Clear();
};
