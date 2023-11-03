#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <unordered_set>

#include "RelationalTable.h"
#include "query_processing_system/clauses/ClauseResult.h"
class IntermediateResultsTable {
  /**
   * Encapsulates the values that different declarations
   * can take, and helps with combining multiple ClauseResults.
   */
 private:
  bool has_no_results = false;
  std::vector<RelationalTable> tables;
  std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> table_mapping;

  void UpdateNoResultsFlag(int table_idx);

  void AddBooleanClauseResult(bool result);
  void AddSingleDeclaration(PqlDeclaration const& d,
                            std::vector<std::string> const& values);
  void AddPairedDeclarations(PqlDeclaration const& d1, PqlDeclaration const& d2,
                             std::vector<std::string> const& new_d1_values,
                             std::vector<std::string> const& new_d2_values);

  void RemoveSingleDeclaration(PqlDeclaration const& d,
                               std::vector<std::string> const& values);
  void RemovePairedDeclaration(PqlDeclaration const& d1,
                               PqlDeclaration const& d2,
                               std::vector<std::string> const& d1_values,
                               std::vector<std::string> const& d2_values);

  void MergeExistingTables(int table_to_keep_id, int table_to_merge_id,
                           bool allow_cross_product = false);

 public:
  IntermediateResultsTable();

  void AddClauseResult(ClauseResult const& clause_result, bool is_negated);

  bool HasNoResults() const;

  bool HasDeclaration(PqlDeclaration const& declaration) const;

  std::vector<std::vector<std::string>> GetValuesGivenDeclarations(
      std::vector<PqlDeclaration> const& decls);
};
