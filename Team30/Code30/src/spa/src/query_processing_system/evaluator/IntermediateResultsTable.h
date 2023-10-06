#pragma once

#include <unordered_map>

#include "RelationalTable.h"
#include "query_processing_system/common/ClauseResult.h"
class IntermediateResultsTable {
  /**
   * Encapsulates the values that different declarations
   * can take, and helps with combining multiple ClauseResults.
   */
 private:
  bool has_no_results = false;
  std::vector<RelationalTable> tables;
  std::unordered_map<PqlDeclaration, int, PqlDeclarationHash> table_mapping;

  void addBooleanClauseResult(bool result);
  void addSingleDeclaration(PqlDeclaration const& d,
                            std::vector<std::string> const& values);
  void addPairedDeclarations(PqlDeclaration const& d1, PqlDeclaration const& d2,
                             std::vector<std::string> const& new_d1_values,
                             std::vector<std::string> const& new_d2_values);
  void mergeExistingTables(int table_to_keep_id, int table_to_merge_id);

 public:
  IntermediateResultsTable();

  void addClauseResult(ClauseResult const& clause_result);

  bool hasNoResults() const;

  bool hasDeclaration(PqlDeclaration const& declaration);

//  std::vector<std::string> getValuesGivenDeclaration(
//      PqlDeclaration const& declaration);

  std::vector<std::vector<std::string>> getValuesGivenDeclarations(
      std::vector<PqlDeclaration> const& decls);

};
