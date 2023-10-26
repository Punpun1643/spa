#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../shared/parser/node/TreeNode.h"
#include "shared/types/RelationType.h"
#include "tables/RelationTable.h"
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

  void Insert(std::string line_num, std::string lhs,
              std::shared_ptr<TreeNode> rhs);
  std::unordered_set<std::string> GetStatementNumbersGivenLHS(std::string v);
  std::unordered_map<std::string,
                     std::pair<std::string, std::shared_ptr<TreeNode>>>
  GetAssignmentPatterns();
};
