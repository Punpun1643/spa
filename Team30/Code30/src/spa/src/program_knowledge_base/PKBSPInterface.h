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

#include "shared/types/AttrType.h"
#include "shared/types/EntityType.h"
#include "shared/types/MatchType.h"
#include "shared/types/RelationType.h"

class PKBSPInterface {
 public:
  virtual void InsertEntity(EntityType type, std::string const& value) = 0;
  virtual void InsertEntity(EntityType type, AttrType attr_type,
                            std::string const& entity,
                            std::string const& attribute) = 0;
  virtual void InsertRelation(RelationType rel_type,
                              std::string const& s1_line_num,
                              std::string const& s2_line_num) = 0;
  virtual void InsertAssignPattern(std::string const& statement_number,
                                   std::string const& lhs,
                                   std::shared_ptr<TreeNode> const& rhs) = 0;
  virtual void InsertCondVarPattern(EntityType type,
                                    std::string const& statement_number,
                                    std::string const& var) = 0;

  virtual std::unordered_set<std::string> GetProcedureModifies(
      std::string const& proc_name) = 0;
  virtual std::unordered_set<std::string> GetProcedureUses(
      std::string const& proc_name) = 0;
  virtual std::unordered_set<std::string> GetStatementModifies(
      std::string const& stmt) = 0;
  virtual std::unordered_set<std::string> GetStatementUses(
      std::string const& stmt) = 0;
  virtual void InsertCFGNode(std::string const& statement_num,
                             std::shared_ptr<CFGNode> const& node) = 0;
};
