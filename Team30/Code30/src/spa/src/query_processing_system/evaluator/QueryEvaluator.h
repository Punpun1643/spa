#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PkbApi.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "IntermediateResultsTable.h"
#include "query_processing_system/common/PatternClause.h"

class QueryEvaluator {
  typedef std::vector<std::shared_ptr<Clause>> const& ClauseList;
 private:
  PkbApi& pkb;

  void populateIntermediateResultsTable(IntermediateResultsTable& table, ClauseList clauses);

 public:

  explicit QueryEvaluator(PkbApi& pkb);

  bool evaluateQuery(ClauseList clauses);

  std::vector<std::vector<std::string>> evaluateQuery(std::vector<PqlDeclaration> const& chosen_decls,
                                                      ClauseList clauses);

  // legacy method, remove when QPS Parser is updated
  std::vector<std::string> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      ClauseList other_clauses);

  /*
   * Approach to handle multi-select.
   * Overload the query evaluator. Do away with SelectClauses entirely because
   * they don't really do anything (just wrap around), and for the reason below:
   *
   * A source of optimisation is to check if the desired declaration is
   * already in the table. if it is, we don't even need to 'evaluate' a SelectClause,
   * because select clause will always encompass all possible values of that type.
   *
   * Would be annoying to make SelectClause be able to handle bools, singles, and multiple return values.
   *
   * Only if that decl has not shown up at all yet do we need to retrieve all values of that type.
   *
   * SelectClause therefore evaluated last. Rename selectClause to get all values later.
   */
};
