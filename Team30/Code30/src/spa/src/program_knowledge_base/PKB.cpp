#include "PKB.h"

#include <string>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "PkbApi.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

using namespace std;

PKB::PKB() : PkbApi() {
  entData = make_unique<EntityDatabase>(EntityDatabase());
  relData = make_unique<RelDatabase>(RelDatabase());

  // TODO: Find a cleaner way to insert to multiple tables simultaneously
  relatedTables = {
      {RelationType::FOLLOWS,
       {RelationType::FOLLOWS, RelationType::FOLLOWS_STAR}},

      {RelationType::PARENT, {RelationType::PARENT, RelationType::PARENT_STAR}},

      {RelationType::USES, {RelationType::USES}},
      {RelationType::MODIFIES, {RelationType::MODIFIES}}};

  };

// ---------- INSERTIONS ----------
void PKB::insertEntity(EntityType type, std::string entity) {
  entData->insert(type, entity);
};

void PKB::insertRelation(RelationType type, string stmt1, string stmt2) {
  // Inserts into more than 1 table simultaneously
  // Add all related tables to relatedTables
  for (RelationType rt : relatedTables[type]) {
    shared_ptr<BaseTable> table = relData->getTable(rt);
    table->insert(stmt1, stmt2);
  }
};

// ---------- RETRIEVE ENTITIES ----------
unique_ptr<vector<string>> PKB::getEntitiesWithType(EntityType type) {
  shared_ptr<unordered_set<string>> e = entData->get(type);
  vector<string> v(e->begin(), e->end());
  return make_unique<vector<string>>(v);
};

// ---------- RETRIEVE RELATIONS ----------
// 0 Declarations
// example Follows(1, 3)
bool PKB::isRelationTrue(string value_1, string value_2,
                         RelationType rel_type) {
  return relData->getTable(rel_type)->isRelated(value_1, value_2);
};

// example Follows(1, _)
bool PKB::isRelationTrueGivenFirstValue(std::string value,
                                        RelationType rel_type) {
  shared_ptr<unordered_set<string>> ents = entData->get(EntityType::STMT);
  shared_ptr<BaseTable> table = relData->getTable(rel_type);

  // TODO: Optimise
  for (string ent : *ents) {
    if (table->isRelated(value, ent)) {
      return true;
    }
  }
  return false;
}

// example Follows(_, 1)
bool PKB::isRelationTrueGivenSecondValue(std::string value,
                                         RelationType rel_type) {
  shared_ptr<unordered_set<string>> ents = entData->get(EntityType::STMT);
  shared_ptr<BaseTable> table = relData->getTable(rel_type);

  // TODO: Optimise
  for (string ent : *ents) {
    if (table->isRelated(ent, value)) {
      return true;
    }
  }
  return false;
}

// example Follows(_, _)
bool PKB::isRelationTrueForAny(RelationType relation_type) {
  shared_ptr<unordered_set<string>> ents = entData->get(EntityType::STMT);
  shared_ptr<BaseTable> table = relData->getTable(relation_type);

  // TODO: Optimise
  for (string ent1 : *ents) {
    for (string ent2 : *ents) {
      if (table->isRelated(ent1, ent2)) {
        return true;
      }
    }
  }
  return false;
}

// 1 Declarations
// example Parent(s, _), ParentsStar(s, _)
unique_ptr<vector<string>> PKB::getRelationValuesGivenFirstType(
    EntityType entity_type, RelationType rel_type) {
  unordered_set<string> output;
  shared_ptr<BaseTable> table = relData->getTable(rel_type);
  shared_ptr<unordered_set<string>> ents1 = entData->get(entity_type);
  shared_ptr<unordered_set<string>> ents2;

  if (rel_type == RelationType::USES || rel_type == RelationType::MODIFIES) {
    ents2 = entData->get(EntityType::VARIABLE);
  } else {
    ents2 = entData->get(EntityType::STMT);
  }
  

  // TODO: Optimise
  for (string ent1 : *ents1) {
    for (string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.insert(ent1);
      }
    }
  }

  vector<string> output_vector;
  output_vector.assign(output.begin(), output.end());
  return make_unique<vector<string>>(output_vector);
}

// example Follows(_, 3), FolowsStar(_, 3)
unique_ptr<vector<string>> PKB::getRelationValuesGivenSecondType(
    EntityType entity_type, RelationType rel_type) {
  unordered_set<string> output;
  shared_ptr<BaseTable> table = relData->getTable(rel_type);
  shared_ptr<unordered_set<string>> ents1 = entData->get(EntityType::STMT);
  shared_ptr<unordered_set<string>> ents2 = entData->get(entity_type);

  // TODO: Optimise
  for (string ent1 : *ents1) {
    for (string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.insert(ent2);
      }
    }
  }

  vector<string> output_vector;
  output_vector.assign(output.begin(), output.end());
  return make_unique<vector<string>>(output_vector);
};

// example Follows(s, 3), FolowsStar(s, 3)
unique_ptr<vector<string>> PKB::getRelationValues(EntityType entity_type,
                                                  string value,
                                                  RelationType rel_type) {
  unordered_set<string> output;
  shared_ptr<unordered_set<string>> ents = entData->get(entity_type);
  shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (string ent : *ents) {
    if (table->isRelated(ent, value)) {
      output.insert(ent);
    }
  }

  vector<string> output_vector;
  output_vector.assign(output.begin(), output.end());
  return make_unique<vector<string>>(output_vector);
}

// example Follows(3, s)
unique_ptr<vector<string>> PKB::getRelationValues(string value,
                                                  EntityType entity_type,
                                                  RelationType rel_type) {
  unordered_set<string> output;
  shared_ptr<unordered_set<string>> ents = entData->get(entity_type);
  shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (string ent : *ents) {
    if (table->isRelated(value, ent)) {
      output.insert(ent);
    }
  }

  vector<string> output_vector;
  output_vector.assign(output.begin(), output.end());
  return make_unique<vector<string>>(output_vector);
}

//// 2 Declarations
// example Follows(s1, s2), FolowsStar(s1, s2)
unique_ptr<vector<pair<string, string>>> PKB::getRelationValues(
    EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type) {
  vector<pair<string, string>> output;
  shared_ptr<unordered_set<string>> ents1 = PKB::entData->get(entity_type_1);
  shared_ptr<unordered_set<string>> ents2 = entData->get(entity_type_2);
  shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (string ent1 : *ents1) {
    for (string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.push_back(make_pair(ent1, ent2));
      }
    }
  }
  return make_unique<vector<pair<string, string>>>(output);
};

// --------------------------------------- DELETE AFTER MIGRATION
// -------------------------------------------------------
std::optional<std::pair<int, int>> PKB::getFollows(int s1_line_num,
                                                   EntityType s2_type) {
  // Code logic to be implemented here
  std::pair<int, int> matchingPair = std::make_pair(1, 2);
  std::optional<std::pair<int, int>> returnPair = matchingPair;
  return returnPair;
}

std::optional<std::pair<int, int>> PKB::getFollows(EntityType s1_type,
                                                   int s2_line_num) {
  // Database logic to be added here
  std::pair<int, int> matchPair = std::make_pair(3, 4);
  std::optional<std::pair<int, int>> returnPair = matchPair;
  return returnPair;
}

std::unique_ptr<std::vector<std::pair<int, int>>> PKB::getFollows(
    EntityType s1_type, EntityType s2_type) {
  // Database logic to be added here
  std::pair<int, int> matchPair = std::make_pair(5, 6);
  std::vector<std::pair<int, int>> pairs = {matchPair};
  return std::make_unique<std::vector<std::pair<int, int>>>(pairs);
}

// int PKB::setProcToAST(PROC p, TNode* r) { return 0; }

// TNode* PKB::getRootAST(PROC p) { return nullptr; }
