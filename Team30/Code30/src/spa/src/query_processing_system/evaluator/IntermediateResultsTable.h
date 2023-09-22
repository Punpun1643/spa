#pragma once

#include "RelationalTable.h"
#include "query_processing_system/common/ClauseResult.h"
#include <unordered_map>
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
  void addSingleDeclaration(const PqlDeclaration& d,
                            const std::vector<std::string> &values);
  void addPairedDeclarations(const PqlDeclaration& d1, const PqlDeclaration& d2,
                             const std::vector<std::string> & new_d1_values,
                             const std::vector<std::string> & new_d2_values);

 public:
  IntermediateResultsTable();

  void addClauseResult(const ClauseResult& clause_result);

  bool hasNoResults() const;

  std::vector<std::string> getValuesGivenDeclaration(const PqlDeclaration &declaration);
};
