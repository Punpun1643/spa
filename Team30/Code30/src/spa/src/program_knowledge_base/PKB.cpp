#include "PKB.h"

#include <string>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "PkbApi.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

PKB::PKB() : PkbApi() {
  entData = std::make_unique<EntityDatabase>(EntityDatabase());
  relData = std::make_unique<RelDatabase>(RelDatabase());

  // TODO: Find a cleaner way to insert to multiple tables simultaneously
  relatedTables = {
      {RelationType::FOLLOWS,
       {RelationType::FOLLOWS, RelationType::FOLLOWS_STAR}},

      {RelationType::PARENT, {RelationType::PARENT, RelationType::PARENT_STAR}},

      {RelationType::USES_S, {RelationType::USES_S}},
      {RelationType::USES_P, {RelationType::USES_P}},

      {RelationType::MODIFIES_S, {RelationType::MODIFIES_S}},
      {RelationType::MODIFIES_P, {RelationType::MODIFIES_P}}
  };

  };


// ---------- INSERTIONS ----------
void PKB::insertEntity(EntityType type, std::string entity) {
  entData->insert(type, entity);
};

void PKB::insertRelation(RelationType type, std::string input1,
                         std::string input2) {
  // Inserts into more than 1 table simultaneously
  // Add all related tables to relatedTables

  // For MODIFIES and USES, Check the type of the first string input1
  // If first char is an int then it is a stmt_num and it needs to be placed in _S table
  // else it is a procedure and should be placed in _P table
  RelationType target = type;
  char firstChar = input1.at(0);

  if (target == RelationType::USES) {
    if (isdigit(firstChar)) { //Stmt Num
      target = RelationType::USES_S;
    } else { //procedure
      target = RelationType::USES_P;
    }
  }

  if (target == RelationType::MODIFIES) {
    if (isdigit(firstChar)) {  // Stmt Num
      target = RelationType::MODIFIES_S;
    } else { //procedure
      target = RelationType::MODIFIES_P;
    }
  }


  for (RelationType rt : relatedTables[target]) {
    std::shared_ptr<BaseTable> table = relData->getTable(rt);
    table->insert(input1, input2);

  }
};

void PKB::insertRelation(RelationType rel_type, std::string s_line_num,
                         EntityType ent_type, std::string enity){};
void PKB::insertRelation(RelationType rel_type, EntityType ent_type,
                         std::string entity, std::string s_line_num){};
void PKB::insertRelation(RelationType rel_type, EntityType ent_type1,
                         std::string entity1, EntityType ent_type2,
                         std::string entity2){};

// ---------- RETRIEVE ENTITIES ----------
std::unique_ptr<std::vector<std::string>> PKB::getEntitiesWithType(
    EntityType type) {
  std::shared_ptr<std::unordered_set<std::string>> e = entData->get(type);
  return std::make_unique<std::vector<std::string>>(e->begin(), e->end());
};

// ---------- RETRIEVE RELATIONS ----------
// 0 Declarations
// example Follows(1, 3)
bool PKB::isRelationTrue(std::string value_1, std::string value_2,
                         RelationType rel_type) {
  return relData->getTable(rel_type)->isRelated(value_1, value_2);
};

// example Follows(1, _)
bool PKB::isRelationTrueGivenFirstValue(std::string value,
                                        RelationType rel_type) {
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(EntityType::STMT); // TODO: FIX THIS
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);

  // TODO: Optimise
  for (std::string ent : *ents) {
    if (table->isRelated(value, ent)) {
      return true;
    }
  }
  return false;
}

// example Follows(_, 1)
bool PKB::isRelationTrueGivenSecondValue(std::string value,
                                         RelationType rel_type) {
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(EntityType::STMT);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);

  // TODO: Optimise
  for (std::string ent : *ents) {
    if (table->isRelated(ent, value)) {
      return true;
    }
  }
  return false;
}

// example Follows(_, _)
bool PKB::isRelationTrueForAny(RelationType relation_type) {
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(EntityType::STMT); // TODO: FIX FOR USES/MODIFIES
  std::shared_ptr<BaseTable> table = relData->getTable(relation_type);
  return ents->size() > 0;
}

// 1 Declarations
// example Parent(s, _), ParentsStar(s, _)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValuesGivenFirstType(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      entData->get(entity_type);
  std::shared_ptr<std::unordered_set<std::string>> ents2;
  if (rel_type == RelationType::USES_S ||
      rel_type == RelationType::USES_P ||
      rel_type == RelationType::MODIFIES_S ||
      rel_type == RelationType::MODIFIES_P) { // TODO: CLEAN THIS UP
    ents2 = entData->get(EntityType::VARIABLE);
  } else {
    ents2 = entData->get(EntityType::STMT);
  }

  // TODO: Optimise
  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.insert(ent1);
      }
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(_, 3), FolowsStar(_, 3)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValuesGivenSecondType(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      entData->get(EntityType::STMT);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(entity_type);

  // TODO: Optimise
  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.insert(ent2);
      }
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
};

// example Follows(s, 3), FolowsStar(s, 3)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValues(
    EntityType entity_type, std::string value, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(entity_type);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string ent : *ents) {
    if (table->isRelated(ent, value)) {
      output.insert(ent);
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(3, s)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValues(
    std::string value, EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(entity_type);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string ent : *ents) {
    if (table->isRelated(value, ent)) {
      output.insert(ent);
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

//// 2 Declarations
// example Follows(s1, s2), FollowsStar(s1, s2)
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                       RelationType rel_type) {
  std::vector<std::pair<std::string, std::string>> output;
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      PKB::entData->get(entity_type_1);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(entity_type_2);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.push_back(make_pair(ent1, ent2));
      }
    }
  }
  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      output);
};


// Pattern clause
void PKB::insertPattern(std::string statement_number, std::string lhs,
                        std::unordered_set<std::string> rhs){};

std::unique_ptr<std::vector<std::string>> PKB::getPatternMatchesWithWildLhs(std::string rhs_expr, MatchType expr_match_type) {
    return std::make_unique<std::vector<std::string>>();
};


std::unique_ptr<std::vector<std::string>> PKB::getPatternMatchesWithLhsValue(std::string lhs_value, std::string rhs_expr,
                                                                        MatchType expr_match_type) {
    return std::make_unique<std::vector<std::string>>();
};

// 2 paired values - for the implicit assign declaration, and the values for the given lhs_entity_type
std::unique_ptr<std::vector<std::pair<std::string, std::string>>> PKB::getPatternMatchesWithDeclarationLhs(std::string rhs_expr, MatchType expr_match_type) {
    return std::make_unique<std::vector<std::pair<std::string, std::string>>>();
};
