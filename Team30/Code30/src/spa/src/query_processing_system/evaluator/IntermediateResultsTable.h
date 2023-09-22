#pragma once

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


  // one more data structure mapping pql to col_idx i think.
  // DATA STRUCTURE NEEDS TO BE 2D LIST? table format, inner joiN?
//  std::unordered_map<PqlDeclaration, std::vector<std::string>,
//                     PqlDeclarationHash>
//      value_map;

//  std::unordered_map<PqlDeclaration,
//                     std::unordered_set<PqlDeclaration, PqlDeclarationHash>,
//                     PqlDeclarationHash>
//      linked_declarations;

  void addBooleanClauseResult(bool result);
  void addSingleDeclaration(const PqlDeclaration& d,
                            const std::vector<std::string> &values);
  void addPairedDeclarations(const PqlDeclaration& d1, const PqlDeclaration& d2,
                             const std::vector<std::string> & new_d1_values,
                             const std::vector<std::string> & new_d2_values);

  std::vector<std::string> getTableCol(const PqlDeclaration &d);



 public:
  IntermediateResultsTable();

  void addClauseResult(const ClauseResult& clause_result);

  bool hasNoResults() const;

  std::vector<std::string> getValuesGivenDeclaration(const PqlDeclaration &declaration);
};
