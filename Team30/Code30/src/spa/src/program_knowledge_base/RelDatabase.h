#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <string>

#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "tables/LinkedListTable.h"
#include "../source_processor/node/util_node/CFGNode.h"

class RelDatabase {
 private:
  std::unordered_map<RelationType, std::shared_ptr<BaseTable>> relationships =
      {};
  std::unordered_map<RelationType, std::vector<RelationType>> relatedTables;

  std::unordered_map<std::string, std::shared_ptr<CFGNode>> cfgNodes;
  std::unordered_set<RelationType> cfgRelations;

  bool isRelatedCFG(RelationType type, std::string val1, std::string val2);
  bool hasRelationsCFG(RelationType type, std::string val);
  bool hasInverseRelationsCFG(RelationType type, std::string val);

  std::unordered_set<std::string> getAllWithRelationsCFG(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);
  std::unordered_set<std::string> getAllWithInverseRelationsCFG(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);

  std::unordered_set<std::string> getAllRelatedToValueCFG(RelationType type,
                                                       std::string val);
  std::unordered_set<std::string> getAllInverseRelatedToValueCFG(RelationType type,
                                                              std::string val);

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
