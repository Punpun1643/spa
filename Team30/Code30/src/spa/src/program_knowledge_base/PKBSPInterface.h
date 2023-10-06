#pragma once
#include <source_processor/node/stmt_node/AssignNode.h>
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "RelationType.h"
#include "query_processing_system/common/MatchType.h"

class PkbApi {
 public:
  virtual void insertEntity(EntityType type, std::string entity) = 0;
  virtual void insertRelation(RelationType rel_type, std::string s1_line_num,
                              std::string s2_line_num) = 0;
  virtual void insertPattern(std::string statement_number, std::string lhs,
                             std::unordered_set<std::string> rhs) = 0;
  virtual std::unordered_set<std::string> getProcedureModifies(
      std::string procName) = 0;
  virtual std::unordered_set<std::string> getProcedureUses(
      std::string procName) = 0;
};
