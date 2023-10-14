#pragma once
#include <source_processor/node/stmt_node/AssignNode.h>
#include <source_processor/node/stmt_node/StmtNode.h>
#include <source_processor/node/util_node/CFGNode.h>

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "PatternType.h"
#include "RelationType.h"
#include "query_processing_system/common/MatchType.h"

class PKBSPInterface {
 public:
  virtual void insertEntity(EntityType type, std::string entity) = 0;
  virtual void insertRelation(RelationType rel_type, std::string s1_line_num,
                              std::string s2_line_num) = 0;
  virtual void insertPattern(PatternType type, std::string statement_number,
                             std::string lhs,
                             std::shared_ptr<TreeNode> rhs) = 0;
  virtual std::unordered_set<std::string> getProcedureModifies(
      std::string procName) = 0;
  virtual std::unordered_set<std::string> getProcedureUses(
      std::string procName) = 0;
  virtual void insertCFGNode(std::string statement_num,
                             std::shared_ptr<CFGNode> node) = 0;
};
