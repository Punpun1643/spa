#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../../program_knowledge_base/PKB.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"

class QueryEvaluator {
 private:
  PKB& pkb;
 public:
  QueryEvaluator(PKB& pkb);

  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause);

  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      std::unique_ptr<SuchThatClause> such_that_clause);
};
