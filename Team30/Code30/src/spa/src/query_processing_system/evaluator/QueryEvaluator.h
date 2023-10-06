#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "IntermediateResultsTable.h"
#include "query_processing_system/common/PatternClause.h"

class QueryEvaluator {
  typedef std::vector<std::shared_ptr<Clause>> const& ClauseList;
 private:
  PKBQPSInterface& pkb;

  void populateIntermediateResultsTable(IntermediateResultsTable& table, ClauseList clauses);

 public:
  explicit QueryEvaluator(PKBQPSInterface& pkb);

  bool evaluateQuery(ClauseList clauses);

  std::vector<std::vector<std::string>> evaluateQuery(std::vector<PqlDeclaration> const& chosen_decls,
                                                      ClauseList clauses);

  // legacy method, remove when QPS Parser is updated
  std::vector<std::string> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      ClauseList other_clauses);
};
