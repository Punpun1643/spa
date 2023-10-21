#include "RelDatabase.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <algorithm>

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

  relatedTables = {{RelationType::FOLLOWS, {RelationType::FOLLOWS_STAR}},
                   {RelationType::PARENT, {RelationType::PARENT_STAR}},
                   {RelationType::CALLS, {RelationType::CALLS_STAR}},
                   {RelationType::NEXT, {RelationType::NEXT_STAR}}};

  cfgRelations = {RelationType::NEXT, RelationType::NEXT_STAR,
                  RelationType::AFFECTS};

  size = 0;
}

bool RelDatabase::IsValidStatementNumber(std::string val) {
  int v = std::stoi(val);
  return 0 < v && v < size + 1;
}

bool RelDatabase::IsCFGRelation(RelationType type) {
  return cfgRelations.find(type) != cfgRelations.end();
}

bool RelDatabase::isEmptyCFG(RelationType type) {
  // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR, AFFECTS
  for (auto pair : cfgNodes) {
    if (hasRelations(type, pair.first)) {
      return false;
    }
  }
  return true;
}

bool RelDatabase::isRelatedCFG(RelationType type, std::string val1,
                               std::string val2) {
  if (!IsValidStatementNumber(val1) || !IsValidStatementNumber(val2)) {
    return false;
  }

  std::shared_ptr<CFGNode> node1 = cfgNodes[val1];
  std::shared_ptr<CFGNode> node2 = cfgNodes[val2];

  if (type == RelationType::NEXT) {
    return CFGNode::HasImmediatePath(node1, node2);
  } else if (type == RelationType::NEXT_STAR) {
    return CFGNode::HasPath(node1, node2);
  } else {
    return CFGNode::HasAffectsPath(node1, node2);
  }
}

bool RelDatabase::hasRelationsCFG(RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return false;
  }

  std::shared_ptr<CFGNode> node = cfgNodes[val];
  if (type == RelationType::NEXT || type == RelationType::NEXT_STAR) {
    return !node->GetOutgoingNodes().empty();
  }

  // TODO(@tyanhan): Optimise for AFFECTS
  for (auto pair : cfgNodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (CFGNode::HasAffectsPath(node, n)) {
      return true;
    }
  }
  return false;
}

bool RelDatabase::hasInverseRelationsCFG(RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return false;
  }

  std::shared_ptr<CFGNode> node = cfgNodes[val];
  if (type == RelationType::NEXT || type == RelationType::NEXT_STAR) {
    return !node->GetIncomingNodes().empty();
  }

  // TODO(@tyanhan): Optimise for AFFECTS
  for (auto pair : cfgNodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (CFGNode::HasAffectsPath(n, node)) {
      return true;
    }
  }
  return false;
}

std::unordered_set<std::string> RelDatabase::getAllWithRelationsCFG(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR and AFFECTS
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (hasRelations(type, val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::getAllWithInverseRelationsCFG(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR and AFFECTS
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (hasInverseRelations(type, val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllWithPathFrom(
    std::shared_ptr<CFGNode> node) {
  std::unordered_set<std::string> output;
  for (auto pair : cfgNodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (CFGNode::HasPath(node, n)) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllWithAffectsPathFrom(
    std::shared_ptr<CFGNode> node) {
  std::unordered_set<std::string> output;
  for (auto pair : cfgNodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (CFGNode::HasAffectsPath(node, n)) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::getAllRelatedToValueCFG(
    RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return std::unordered_set<std::string>();
  }

  std::shared_ptr<CFGNode> node = cfgNodes[val];

  if (type == RelationType::NEXT) {
    std::unordered_set<std::string> output;
    for (auto n : node->GetOutgoingNodes()) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
    return output;
  } else if (type == RelationType::NEXT_STAR) {
    // TODO(@tyanhan): Optimise for NEXT_STAR and AFFECTS
    return GetAllWithPathFrom(node);
  } else {
    return GetAllWithAffectsPathFrom(node);
  }
}

std::unordered_set<std::string> RelDatabase::GetAllWithPathTo(
    std::shared_ptr<CFGNode> node) {
  std::unordered_set<std::string> output;
  for (auto pair : cfgNodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (CFGNode::HasPath(n, node)) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllWithAffectsPathTo(
    std::shared_ptr<CFGNode> node) {
  std::unordered_set<std::string> output;
  for (auto pair : cfgNodes) {
    std::shared_ptr<CFGNode> n = pair.second;
    if (CFGNode::HasAffectsPath(n, node)) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::getAllInverseRelatedToValueCFG(
    RelationType type, std::string val) {
  if (!IsValidStatementNumber(val)) {
    return std::unordered_set<std::string>();
  }

  std::shared_ptr<CFGNode> node = cfgNodes[val];

  if (type == RelationType::NEXT) {
    std::unordered_set<std::string> output;
    for (auto n : node->GetIncomingNodes()) {
      output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
    }
    return output;
  } else if (type == RelationType::NEXT_STAR) {
    // TODO(@tyanhan): Optimise for NEXT_STAR and AFFECTS
    return GetAllWithPathTo(node);
  } else {
    return GetAllWithAffectsPathTo(node);
  }
}

void RelDatabase::insert(RelationType type, std::string val1,
                         std::string val2) {
  relationships[type]->insert(val1, val2);
  for (RelationType rt : relatedTables[type]) {
    relationships[rt]->insert(val1, val2);
  }
}

bool RelDatabase::isEmpty(RelationType type) {
  if (IsCFGRelation(type)) {
    return isEmptyCFG(type);
  }
  return relationships[type]->isEmpty();
}

bool RelDatabase::isRelated(RelationType type, std::string val1,
                            std::string val2) {
  if (IsCFGRelation(type)) {
    return isRelatedCFG(type, val1, val2);
  }
  return relationships[type]->isRelated(val1, val2);
}

bool RelDatabase::hasRelations(RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return hasRelationsCFG(type, val);
  }
  return relationships[type]->hasRelations(val);
}

bool RelDatabase::hasInverseRelations(RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return hasInverseRelationsCFG(type, val);
  }
  return relationships[type]->hasInverseRelations(val);
}

std::unordered_set<std::string> RelDatabase::getAllWithRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  if (IsCFGRelation(type)) {
    return getAllWithRelationsCFG(type, vals);
  }
  return relationships[type]->getAllWithRelations(vals);
}

std::unordered_set<std::string> RelDatabase::getAllWithInverseRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  if (IsCFGRelation(type)) {
    return getAllWithInverseRelationsCFG(type, vals);
  }
  return relationships[type]->getAllWithInverseRelations(vals);
}

std::unordered_set<std::string> RelDatabase::getAllRelatedToValue(
    RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return getAllRelatedToValueCFG(type, val);
  }
  return relationships[type]->getAllRelatedToValue(val);
}

std::unordered_set<std::string> RelDatabase::getAllInverseRelatedToValue(
    RelationType type, std::string val) {
  if (IsCFGRelation(type)) {
    return getAllInverseRelatedToValueCFG(type, val);
  }
  return relationships[type]->getAllInverseRelatedToValue(val);
}

void RelDatabase::insertCFGNode(std::string statement_num,
                                std::shared_ptr<CFGNode> node) {
  size = std::max(size, std::stoi(statement_num));
  cfgNodes.insert({statement_num, node});
}
