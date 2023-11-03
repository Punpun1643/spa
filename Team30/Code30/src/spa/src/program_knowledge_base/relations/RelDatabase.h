#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../source_processor/node/util_node/CFGNode.h"
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

  // helper functions
  bool IsCFGRelation(RelationType type);
  bool IsValidStatementNumber(std::string val);
  std::unordered_set<std::string> GetAllWithPathFrom(
      std::shared_ptr<CFGNode> node);
  std::unordered_set<std::string> GetAllWithAffectsPathFrom(
      std::shared_ptr<CFGNode> node);
  std::unordered_set<std::string> GetAllWithPathTo(
      std::shared_ptr<CFGNode> node);
  std::unordered_set<std::string> GetAllWithAffectsPathTo(
      std::shared_ptr<CFGNode> node);

  bool IsEmptyCFG(RelationType type);
  bool IsRelatedCFG(RelationType type, std::string val1, std::string val2);
  bool HasRelationsCFG(RelationType type, std::string val);
  bool HasInverseRelationsCFG(RelationType type, std::string val);

  std::unordered_set<std::string> GetAllWithRelationsCFG(
      RelationType type, std::unordered_set<std::string> vals);
  std::unordered_set<std::string> GetAllWithInverseRelationsCFG(
      RelationType type, std::unordered_set<std::string> vals);

  std::unordered_set<std::string> GetAllRelatedToValueCFG(RelationType type,
                                                          std::string val);
  std::unordered_set<std::string> GetAllInverseRelatedToValueCFG(
      RelationType type, std::string val);

 public:
  RelDatabase();
  ~RelDatabase() = default;
  void Insert(RelationType type, std::string val1, std::string val2);

  bool IsEmpty(RelationType type);
  bool IsRelated(RelationType type, std::string val1, std::string val2);
  bool HasRelations(RelationType type, std::string val);
  bool HasInverseRelations(RelationType type, std::string val);

  std::unordered_set<std::string> GetAllWithRelations(
      RelationType type, std::unordered_set<std::string> vals);
  std::unordered_set<std::string> GetAllWithInverseRelations(
      RelationType type, std::unordered_set<std::string> vals);

  std::unordered_set<std::string> GetAllRelatedToValue(RelationType type,
                                                       std::string val);
  std::unordered_set<std::string> GetAllInverseRelatedToValue(RelationType type,
                                                              std::string val);

  void InsertCFGNode(std::string statement_num, std::shared_ptr<CFGNode> node);
};
