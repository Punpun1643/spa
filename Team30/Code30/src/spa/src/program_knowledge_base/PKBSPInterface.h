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

#include "PatternType.h"
#include "shared/types/AttrType.h"
#include "shared/types/EntityType.h"
#include "shared/types/MatchType.h"
#include "shared/types/RelationType.h"

class PKBSPInterface {
 public:
  virtual void InsertEntity(EntityType type, std::string value) = 0;
  virtual void InsertEntity(EntityType type, AttrType attr_type,
                            std::string statement_number,
                            std::string attribute) = 0;
  virtual void InsertRelation(RelationType rel_type, std::string s1_line_num,
                              std::string s2_line_num) = 0;
  virtual void InsertAssignPattern(std::string statement_number,
                             std::string lhs,
                             std::shared_ptr<TreeNode> rhs) = 0;
  virtual std::unordered_set<std::string> GetProcedureModifies(
      std::string proc_name) = 0;
  virtual std::unordered_set<std::string> GetProcedureUses(
      std::string proc_name) = 0;
  virtual std::unordered_set<std::string> GetStatementModifies(
      std::string stmt) = 0;
  virtual std::unordered_set<std::string> GetStatementUses(
      std::string stmt) = 0;
  virtual void InsertCFGNode(std::string statement_num,
                             std::shared_ptr<CFGNode> node) = 0;
};
