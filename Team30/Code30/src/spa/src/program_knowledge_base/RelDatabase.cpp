#include "RelDatabase.h"

#include <iostream>
#include <string>
#include <unordered_set>

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
}

void RelDatabase::insert(RelationType type, std::string val1,
                         std::string val2) {
  assert(cfgRelations.find(type) == cfgRelations.end());

  relationships[type]->insert(val1, val2);
  for (RelationType rt : relatedTables[type]) {
    relationships[rt]->insert(val1, val2);
  }
}

bool RelDatabase::isEmpty(RelationType type) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
    // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR, AFFECTS
    for (auto pair : cfgNodes) {
      if (hasRelations(type, pair.first)) {
        return true;
      }
    }
    return false;
  }

  return relationships[type]->isEmpty();
}

bool RelDatabase::isRelated(RelationType type, std::string val1,
                            std::string val2) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
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

  return relationships[type]->isRelated(val1, val2);
}

bool RelDatabase::hasRelations(RelationType type, std::string val) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
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

  return relationships[type]->hasRelations(val);
}

bool RelDatabase::hasInverseRelations(RelationType type, std::string val) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
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
  return relationships[type]->hasInverseRelations(val);
}

std::unordered_set<std::string> RelDatabase::getAllWithRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
    // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR and AFFECTS
    std::unordered_set<std::string> output;
    for (std::string val : *vals) {
      if (hasRelations(type, val)) {
        output.insert(val);
      }
    }
    return output;
  }
  return relationships[type]->getAllWithRelations(vals);
}

std::unordered_set<std::string> RelDatabase::getAllWithInverseRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
    // TODO(@tyanhan): Optimise for NEXT, NEXT_STAR and AFFECTS
    std::unordered_set<std::string> output;
    for (std::string val : *vals) {
      if (hasInverseRelations(type, val)) {
        output.insert(val);
      }
    }
    return output;
  }
  return relationships[type]->getAllWithInverseRelations(vals);
}

std::unordered_set<std::string> RelDatabase::getAllRelatedToValue(
    RelationType type, std::string val) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
    std::shared_ptr<CFGNode> node = cfgNodes[val];
    std::unordered_set<std::string> output;

    if (type == RelationType::NEXT) {
      for (auto n : node->GetOutgoingNodes()) {
        output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
      }
    } else if (type == RelationType::NEXT_STAR) {  // TODO(@tyanhan): Optimise
                                                   // for NEXT_STAR and AFFECTS
      for (auto pair : cfgNodes) {
        std::shared_ptr<CFGNode> n = pair.second;
        if (CFGNode::HasPath(node, n)) {
          output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
        }
      }
    } else {
      for (auto pair : cfgNodes) {
        std::shared_ptr<CFGNode> n = pair.second;
        if (CFGNode::HasAffectsPath(node, n)) {
          output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
        }
      }
    }

    return output;
  }

  return relationships[type]->getAllRelatedToValue(val);
}

std::unordered_set<std::string> RelDatabase::getAllInverseRelatedToValue(
    RelationType type, std::string val) {
  if (cfgRelations.find(type) != cfgRelations.end()) {
    std::shared_ptr<CFGNode> node = cfgNodes[val];
    std::unordered_set<std::string> output;

    if (type == RelationType::NEXT) {
      for (auto n : node->GetIncomingNodes()) {
        output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
      }
    } else if (type == RelationType::NEXT_STAR) {  // TODO(@tyanhan): Optimise
                                                   // for NEXT_STAR and AFFECTS
      for (auto pair : cfgNodes) {
        std::shared_ptr<CFGNode> n = pair.second;
        if (CFGNode::HasPath(n, node)) {
          output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
        }
      }
    } else {
      for (auto pair : cfgNodes) {
        std::shared_ptr<CFGNode> n = pair.second;
        if (CFGNode::HasAffectsPath(n, node)) {
          output.insert(std::to_string(n->GetNode()->GetStmtIndex()));
        }
      }
    }

    return output;
  }

  return relationships[type]->getAllInverseRelatedToValue(val);
}

void RelDatabase::insertCFGNode(std::string statement_num,
                                std::shared_ptr<CFGNode> node) {
  cfgNodes.insert({statement_num, node});
}
