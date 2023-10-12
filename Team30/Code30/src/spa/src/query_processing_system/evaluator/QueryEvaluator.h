#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "../common/SelectAllClause.h"
#include "../common/SuchThatClause.h"
#include "IntermediateResultsTable.h"
#include "query_processing_system/common/AttrRef.h"
#include "query_processing_system/common/PatternClause.h"

class QueryEvaluator {
  typedef std::vector<std::shared_ptr<Clause>> const& ClauseList;

 private:
  PKBQPSInterface& pkb;

  void populateIntermediateResultsTable(IntermediateResultsTable& table,
                                        ClauseList clauses);

  static std::vector<PqlDeclaration> unwrapAttrRefVector(std::vector<AttrRef> const& attr_refs);

  void fillMissingDecls(IntermediateResultsTable& table, std::vector<PqlDeclaration> const& decls_to_check);

  bool updateResultUsingAttrTypes(std::vector<std::vector<std::string>> &values, std::vector<AttrRef> const& attr_refs);

 public:
  explicit QueryEvaluator(PKBQPSInterface& pkb);

  bool evaluateQuery(ClauseList clauses);

  std::vector<std::vector<std::string>> evaluateQuery(
      std::vector<AttrRef> const& selected_attr_refs, ClauseList clauses);
};
