#pragma once
#include "../../program_knowledge_base/PkbApi.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"

class QueryEvaluator {
 private:
  PkbApi& pkb;

 public:
  explicit QueryEvaluator(PkbApi& pkb);

  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause);

  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      std::unique_ptr<SuchThatClause> such_that_clause);
};
