#pragma once
#include "../../program_knowledge_base/PKB.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"

class QueryEvaluator {
 private:
  PKB& pkb;
 public:
  QueryEvaluator(PKB& pkb);
  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      SelectClause select_clause);
  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      SelectClause select_clause, SuchThatClause such_that_clause);



};