#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../source_processor/node/util_node/CFGNode.h"
#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "tables/LinkedListTable.h"

class RelDatabase {
 private:
  std::unordered_map<RelationType, std::shared_ptr<BaseTable>> relationships =
      {};
  std::unordered_map<RelationType, std::vector<RelationType>> relatedTables;

  std::unordered_map<std::string, std::shared_ptr<CFGNode>> cfgNodes;
  std::unordered_set<RelationType> cfgRelations;

  int size;

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

  bool isEmptyCFG(RelationType type);
  bool isRelatedCFG(RelationType type, std::string val1, std::string val2);
  bool hasRelationsCFG(RelationType type, std::string val);
  bool hasInverseRelationsCFG(RelationType type, std::string val);

  std::unordered_set<std::string> getAllWithRelationsCFG(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);
  std::unordered_set<std::string> getAllWithInverseRelationsCFG(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);

  std::unordered_set<std::string> getAllRelatedToValueCFG(RelationType type,
                                                          std::string val);
  std::unordered_set<std::string> getAllInverseRelatedToValueCFG(
      RelationType type, std::string val);

 public:
  RelDatabase();
  ~RelDatabase() = default;
  void insert(RelationType type, std::string val1, std::string val2);

  bool isEmpty(RelationType type);
  bool isRelated(RelationType type, std::string val1, std::string val2);
  bool hasRelations(RelationType type, std::string val);
  bool hasInverseRelations(RelationType type, std::string val);

  std::unordered_set<std::string> getAllWithRelations(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);
  std::unordered_set<std::string> getAllWithInverseRelations(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);

  std::unordered_set<std::string> getAllRelatedToValue(RelationType type,
                                                       std::string val);
  std::unordered_set<std::string> getAllInverseRelatedToValue(RelationType type,
                                                              std::string val);

  void insertCFGNode(std::string statement_num, std::shared_ptr<CFGNode> node);
};
