#pragma once

#include <shared/parser/node/TreeNode.h>

#include <memory>
#include <unordered_map>

#include "PatternType.h"
#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"

class PatternDatabase {
 private:
  // {statement_number, (lhs, rhs)}
  std::unordered_map<std::string,
                     std::pair<std::string, std::shared_ptr<TreeNode>>>
      assignments;

  // {lhs variable : statement_number}, used for optimisation
  std::unordered_map<std::string, std::unordered_set<std::string>>
      lhs_assignments = {};

 public:
  PatternDatabase();
  ~PatternDatabase() = default;

  void insert(PatternType type, std::string line_num, std::string lhs,
              std::shared_ptr<TreeNode> rhs);
  std::unordered_set<std::string> getStatementNumbersGivenLHS(std::string v);
  std::unordered_map<std::string,
                     std::pair<std::string, std::shared_ptr<TreeNode>>>
  getAssignmentPatterns();
};