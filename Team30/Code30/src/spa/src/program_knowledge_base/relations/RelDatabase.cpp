#include "RelDatabase.h"

#include <algorithm>
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

bool RelDatabase::IsValidStatementNumber(std::string const& val) {
  return cfg_nodes.find(val) != cfg_nodes.end();
}

bool RelDatabase::IsCFGRelation(RelationType type) {
  return cfg_relations.find(type) != cfg_relations.end();
}

bool RelDatabase::IsEmptyCFG(RelationType type) {
  for (auto const& pair : cfg_nodes) {
    if (HasRelations(type, pair.first)) {
      return false;
    }
  }
  return true;
}

bool RelDatabase::IsRelatedCFG(RelationType type, std::string const& val1,
                               std::string const& val2) {
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
    return GraphRelationTraverser::HasAffectsPath(node1, node2, affects_cache);
  }
}

bool RelDatabase::HasRelationsCFG(RelationType type, std::string const& val) {
  if (!IsValidStatementNumber(val)) {
    return false;
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);
  if (type == RelationType::NEXT || type == RelationType::NEXT_STAR) {
    return !node->HasNoOutgoingNodes();
  }

  return GraphRelationTraverser::HasAnyAffectsPathFrom(node, affects_cache);
}

bool RelDatabase::HasInverseRelationsCFG(RelationType type,
                                         std::string const& val) {
  if (!IsValidStatementNumber(val)) {
    return false;
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);
  if (type == RelationType::NEXT || type == RelationType::NEXT_STAR) {
    return !node->HasNoIncomingNodes();
  }

  return GraphRelationTraverser::HasAnyAffectsPathTo(node, affects_cache);
}

std::unordered_set<std::string> RelDatabase::GetAllWithRelationsCFG(
    RelationType type, std::unordered_set<std::string> const& vals) {
  std::unordered_set<std::string> output;
  for (std::string const& val : vals) {
    if (HasRelations(type, val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllWithInverseRelationsCFG(
    RelationType type, std::unordered_set<std::string> const& vals) {
  std::unordered_set<std::string> output;
  for (std::string const& val : vals) {
    if (HasInverseRelations(type, val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> RelDatabase::GetAllRelatedToValueCFG(
    RelationType type, std::string const& val) {
  if (!IsValidStatementNumber(val)) {
    return {};
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);

  if (type == RelationType::NEXT) {
    std::unordered_set<std::string> output;
    for (auto const& n : node->GetOutgoingNodes()) {
      output.insert(std::to_string(n->GetNodeStmtIndex()));
    }
    return output;
  } else if (type == RelationType::NEXT_STAR) {
    return GraphRelationTraverser::GetAllStmtsWithPathFrom(node);
  } else {
    return GraphRelationTraverser::GetAllStmtsWithAffectsPathFrom(
        node, affects_cache);
  }
}

std::unordered_set<std::string> RelDatabase::GetAllInverseRelatedToValueCFG(
    RelationType type, std::string const& val) {
  if (!IsValidStatementNumber(val)) {
    return {};
  }

  std::shared_ptr<CFGNode> node = cfg_nodes.at(val);

  if (type == RelationType::NEXT) {
    std::unordered_set<std::string> output;
    for (auto const& n : node->GetIncomingNodes()) {
      output.insert(std::to_string(n->GetNodeStmtIndex()));
    }
    return output;
  } else if (type == RelationType::NEXT_STAR) {
    return GraphRelationTraverser::GetAllStmtsWithPathTo(node);
  } else {
    return GraphRelationTraverser::GetAllStmtsWithAffectsPathTo(node,
                                                                affects_cache);
  }
}

void RelDatabase::Insert(RelationType type, std::string const& val1,
                         std::string const& val2) {
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

bool RelDatabase::IsRelated(RelationType type, std::string const& val1,
                            std::string const& val2) {
  if (IsCFGRelation(type)) {
    return IsRelatedCFG(type, val1, val2);
  }
  return relationships.at(type)->IsRelated(val1, val2);
}

bool RelDatabase::HasRelations(RelationType type, std::string const& val) {
  if (IsCFGRelation(type)) {
    return HasRelationsCFG(type, val);
  }
  return relationships.at(type)->HasRelations(val);
}

bool RelDatabase::HasInverseRelations(RelationType type,
                                      std::string const& val) {
  if (IsCFGRelation(type)) {
    return HasInverseRelationsCFG(type, val);
  }
  return relationships.at(type)->HasInverseRelations(val);
}

std::unordered_set<std::string> RelDatabase::GetAllWithRelations(
    RelationType type, std::unordered_set<std::string> const& vals) {
  if (IsCFGRelation(type)) {
    return GetAllWithRelationsCFG(type, vals);
  }
  return relationships.at(type)->GetAllWithRelations(vals);
}

std::unordered_set<std::string> RelDatabase::GetAllWithInverseRelations(
    RelationType type, std::unordered_set<std::string> const& vals) {
  if (IsCFGRelation(type)) {
    return GetAllWithInverseRelationsCFG(type, vals);
  }
  return relationships.at(type)->GetAllWithInverseRelations(vals);
}

std::unordered_set<std::string> RelDatabase::GetAllRelatedToValue(
    RelationType type, std::string const& val) {
  if (IsCFGRelation(type)) {
    return GetAllRelatedToValueCFG(type, val);
  }
  return relationships.at(type)->GetAllRelatedToValue(val);
}

std::unordered_set<std::string> RelDatabase::GetAllInverseRelatedToValue(
    RelationType type, std::string const& val) {
  if (IsCFGRelation(type)) {
    return GetAllInverseRelatedToValueCFG(type, val);
  }
  return relationships.at(type)->GetAllInverseRelatedToValue(val);
}

void RelDatabase::InsertCFGNode(std::string const& statement_num,
                                std::shared_ptr<CFGNode> const& node) {
  cfg_nodes.insert({statement_num, node});
}
