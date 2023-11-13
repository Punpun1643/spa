#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../source_processor/node/util_node/CFGNode.h"
#include "program_knowledge_base/utils/AffectsCache.h"
#include "shared/types/RelationType.h"
#include "tables/DictionaryTable.h"
#include "tables/LinkedListTable.h"
#include "tables/RelationTable.h"

class RelDatabase {
 private:
  std::unordered_map<RelationType, std::shared_ptr<RelationTable>>
      relationships = {};
  std::unordered_map<RelationType, std::vector<RelationType>> related_tables;

  std::unordered_map<std::string, std::shared_ptr<CFGNode>> cfg_nodes;
  std::unordered_set<RelationType> cfg_relations;
  std::shared_ptr<AffectsCache> affects_cache =
      std::make_shared<AffectsCache>();

  // helper functions
  bool IsCFGRelation(RelationType type);
  bool IsValidStatementNumber(std::string const& val);

  bool IsEmptyCFG(RelationType type);
  bool IsRelatedCFG(RelationType type, std::string const& val1,
                    std::string const& val2);
  bool HasRelationsCFG(RelationType type, std::string const& val);
  bool HasInverseRelationsCFG(RelationType type, std::string const& val);

  std::unordered_set<std::string> GetAllWithRelationsCFG(
      RelationType type, std::unordered_set<std::string> const& vals);
  std::unordered_set<std::string> GetAllWithInverseRelationsCFG(
      RelationType type, std::unordered_set<std::string> const& vals);

  std::unordered_set<std::string> GetAllRelatedToValueCFG(
      RelationType type, std::string const& val);
  std::unordered_set<std::string> GetAllInverseRelatedToValueCFG(
      RelationType type, std::string const& val);

 public:
  RelDatabase();
  ~RelDatabase() = default;
  void Insert(RelationType type, std::string const& val1,
              std::string const& val2);

  bool IsEmpty(RelationType type);
  bool IsRelated(RelationType type, std::string const& val1,
                 std::string const& val2);
  bool HasRelations(RelationType type, std::string const& val);
  bool HasInverseRelations(RelationType type, std::string const& val);

  std::unordered_set<std::string> GetAllWithRelations(
      RelationType type, std::unordered_set<std::string> const& vals);
  std::unordered_set<std::string> GetAllWithInverseRelations(
      RelationType type, std::unordered_set<std::string> const& vals);

  std::unordered_set<std::string> GetAllRelatedToValue(RelationType type,
                                                       std::string const& val);
  std::unordered_set<std::string> GetAllInverseRelatedToValue(
      RelationType type, std::string const& val);

  void InsertCFGNode(std::string const& statement_num,
                     std::shared_ptr<CFGNode> const& node);
};
