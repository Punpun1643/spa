#include "RelDatabase.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>

#include "../../source_processor/util/GraphRelationTraverser.h"

RelDatabase::RelDatabase() {
  relationships[RelationType::PARENT] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS_STAR] =
      std::make_shared<LinkedListTable>(LinkedListTable());
  relationships[RelationType::PARENT_STAR] =
      std::make_shared<LinkedListTable>(LinkedListTable());

  relationships[RelationType::USES_S] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::USES_P] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::MODIFIES_S] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::MODIFIES_P] =
      std::make_shared<DictionaryTable>(DictionaryTable());

  relationships[RelationType::CALLS] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::CALLS_STAR] =
      std::make_shared<LinkedListTable>(LinkedListTable());

  related_tables = {{RelationType::FOLLOWS, {RelationType::FOLLOWS_STAR}},
                    {RelationType::PARENT, {RelationType::PARENT_STAR}},
                    {RelationType::CALLS, {RelationType::CALLS_STAR}},
                    {RelationType::NEXT, {RelationType::NEXT_STAR}}};

  cfg_relations = {RelationType::NEXT, RelationType::NEXT_STAR,
                   RelationType::AFFECTS};
}

bool RelDatabase::IsValidStatementNumber(std::string val) {
  return cfg_nodes.find(val) != cfg_nodes.end();
}

bool RelDatabase::IsCFGRelation(RelationType type) {
  return cfg_relations.find(type) != cfg_relations.end();
}

bool RelDatabase::IsEmptyCFG(RelationType type) {
  // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR, AFFECTS
  for (auto pair : cfg_nodes) {
    if (HasRelations(type, pair.first)) {
      return false;
    }
  }
  return true;
}

bool RelDatabase::IsRelatedCFG(RelationType type, std::string val1,
                               std::string val2) {
  if (!IsValidStatementNumber(val1) || !IsValidStatementNumber(val2)) {
    return false;
  }

  std::shared_ptr<CFGNode> node1 = cfg_nodes.at(val1);
  std::shared_ptr<CFGNode> node2 = cfg_nodes.at(val2);

  if (type == RelationType::NEXT) {
    return GraphRelationTraverser::HasImmediatePath(node1, node2);
  } else if (type == RelationType::NEXT_STAR) {
    return GraphRelationTraverser::HasPath(node1, node2);
  } else {
    return GraphRelationTraverser::HasAffectsPath(node1, node2);
  }
}

bool RelDatabase::HasRelationsCFG(RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return false;
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);
  if (type == RelationType::NEXT || type == RelationType::NEXT_STAR) {
    return !node->GetOutgoingNodes().empty();
  }

  // TODO(@tyanhan): Optimise for AFFECTS
  for (auto pair : cfg_nodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (GraphRelationTraverser::HasAffectsPath(node, n)) {
      return true;
    }
  }
  return false;
}

bool RelDatabase::HasInverseRelationsCFG(RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return false;
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);
  if (type == RelationType::NEXT || type == RelationType::NEXT_STAR) {
    return !node->GetIncomingNodes().empty();
  }

  // TODO(@tyanhan): Optimise for AFFECTS
  for (auto pair : cfg_nodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (GraphRelationTraverser::HasAffectsPath(n, node)) {
      return true;
    }
  }
  return false;
}

std::unordered_set<std::string> RelDatabase::GetAllWithRelationsCFG(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR and AFFECTS
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (HasRelations(type, val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllWithInverseRelationsCFG(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR and AFFECTS
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (HasInverseRelations(type, val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllRelatedToValueCFG(
    RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return std::unordered_set<std::string>();
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);

  if (type == RelationType::NEXT) {
    std::unordered_set<std::string> output;
    for (auto n : node->GetOutgoingNodes()) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
    return output;
  } else if (type == RelationType::NEXT_STAR) {
    return GraphRelationTraverser::GetAllStmtsWithPathFrom(node);
  } else {
    return GraphRelationTraverser::GetAllStmtsWithAffectsPathFrom(node);
  }
}

std::unordered_set<std::string> RelDatabase::GetAllWithAffectsPathTo(
    std::shared_ptr<CFGNode> node) {
  std::unordered_set<std::string> output;
  for (auto pair : cfg_nodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (GraphRelationTraverser::HasAffectsPath(n, node)) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllInverseRelatedToValueCFG(
    RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return std::unordered_set<std::string>();
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);

  if (type == RelationType::NEXT) {
    std::unordered_set<std::string> output;
    for (auto n : node->GetIncomingNodes()) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
    return output;
  } else if (type == RelationType::NEXT_STAR) {
    return GraphRelationTraverser::GetAllStmtsWithPathTo(node);
  } else {
    return GetAllWithAffectsPathTo(node);
  }
}

void RelDatabase::Insert(RelationType type, std::string val1,
                         std::string val2) {
  relationships.at(type)->Insert(val1, val2);
  for (RelationType rt : related_tables[type]) {
    relationships.at(rt)->Insert(val1, val2);
  }
}

bool RelDatabase::IsEmpty(RelationType type) {
  if (IsCFGRelation(type)) {
    return IsEmptyCFG(type);
  }
  return relationships.at(type)->IsEmpty();
}

bool RelDatabase::IsRelated(RelationType type, std::string val1,
                            std::string val2) {
  if (IsCFGRelation(type)) {
    return IsRelatedCFG(type, val1, val2);
  }
  return relationships.at(type)->IsRelated(val1, val2);
}

bool RelDatabase::HasRelations(RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return HasRelationsCFG(type, val);
  }
  return relationships.at(type)->HasRelations(val);
}

bool RelDatabase::HasInverseRelations(RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return HasInverseRelationsCFG(type, val);
  }
  return relationships.at(type)->HasInverseRelations(val);
}

std::unordered_set<std::string> RelDatabase::GetAllWithRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  if (IsCFGRelation(type)) {
    return GetAllWithRelationsCFG(type, vals);
  }
  return relationships.at(type)->GetAllWithRelations(vals);
}

std::unordered_set<std::string> RelDatabase::GetAllWithInverseRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  if (IsCFGRelation(type)) {
    return GetAllWithInverseRelationsCFG(type, vals);
  }
  return relationships.at(type)->GetAllWithInverseRelations(vals);
}

std::unordered_set<std::string> RelDatabase::GetAllRelatedToValue(
    RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return GetAllRelatedToValueCFG(type, val);
  }
  return relationships.at(type)->GetAllRelatedToValue(val);
}

std::unordered_set<std::string> RelDatabase::GetAllInverseRelatedToValue(
    RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return GetAllInverseRelatedToValueCFG(type, val);
  }
  return relationships.at(type)->GetAllInverseRelatedToValue(val);
}

void RelDatabase::InsertCFGNode(std::string statement_num,
                                std::shared_ptr<CFGNode> node) {
  cfg_nodes.insert({statement_num, node});
}
