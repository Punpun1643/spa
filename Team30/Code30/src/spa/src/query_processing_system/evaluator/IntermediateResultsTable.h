#pragma once

#include "query_processing_system/common/ClauseResult.h"
class IntermediateResultsTable {
  /**
   * Encapsulates the values that different declarations
   * can take, and helps with combining multiple ClauseResults.
   */
 private:
  bool has_no_results = false;

  std::unordered_map<PqlDeclaration, std::vector<std::string>,
                     PqlDeclarationHash>
      value_map;

  std::unordered_map<PqlDeclaration,
                     std::unordered_set<PqlDeclaration, PqlDeclarationHash>,
                     PqlDeclarationHash>
      linked_declarations;

  void addBooleanClauseResult(bool result);
  void addSingleDeclaration(const PqlDeclaration& d,
                            std::unique_ptr<std::vector<std::string>> values);
  void addPairedDeclarations(const PqlDeclaration& d1, const PqlDeclaration& d2,
                             std::unique_ptr<std::vector<std::string>> d1_values,
                             std::unique_ptr<std::vector<std::string>> d2_values);

 public:
  IntermediateResultsTable();


  void addClauseResult(const ClauseResult& clause_result);


  std::unique_ptr<std::vector<std::string>> getValuesGivenDeclaration(const PqlDeclaration &declaration);
};
