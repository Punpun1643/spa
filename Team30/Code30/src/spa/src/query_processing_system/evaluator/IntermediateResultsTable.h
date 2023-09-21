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

  static std::unique_ptr<std::vector<std::string>> intersectLists(
      std::vector<std::string> arr_1, std::vector<std::string> arr_2);

 public:
  IntermediateResultsTable();

  static std::unique_ptr<std::unordered_map<std::string, int>> getValueCounts(const std::vector<std::string>& arr);

  void addClauseResult(const ClauseResult& clause_result);

  static std::unique_ptr<std::vector<int>> getIndicesToKeep(std::vector<std::string> arr, std::vector<std::string> overlap_arr);

  std::unique_ptr<std::vector<std::string>> getValuesGivenDeclaration(const PqlDeclaration &declaration);
};
