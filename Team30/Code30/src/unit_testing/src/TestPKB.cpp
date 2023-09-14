#include "catch.hpp"
#include "program_knowledge_base/PKB.h"
#include "program_knowledge_base/RelationType.h"

TEST_CASE("Follows, Parent, Follows* and Parent*") {
  PKB pkb = PKB();
  pkb.insertEntity(EntityType::PROCEDURE, "main");
  pkb.insertEntity(EntityType::PROCEDURE, "sub");
  pkb.insertEntity(EntityType::CONSTANT, "3");
  pkb.insertEntity(EntityType::VARIABLE, "x");

  pkb.insertEntity(EntityType::STMT, "1");
  pkb.insertEntity(EntityType::CALL, "2");
  pkb.insertEntity(EntityType::PRINT, "3");
  pkb.insertEntity(EntityType::IF, "4");
  pkb.insertEntity(EntityType::READ, "5");
  pkb.insertEntity(EntityType::STMT, "6");
  pkb.insertEntity(EntityType::ASSIGN, "7");

  pkb.insertRelation(RelationType::FOLLOWS, "1", "2");
  pkb.insertRelation(RelationType::FOLLOWS, "2", "3");
  pkb.insertRelation(RelationType::FOLLOWS, "3", "4");
  pkb.insertRelation(RelationType::FOLLOWS, "5", "6");

  pkb.insertRelation(RelationType::PARENT, "4", "5");
  pkb.insertRelation(RelationType::PARENT, "4", "6");
  pkb.insertRelation(RelationType::PARENT, "6", "7");

  // added follows relation
  REQUIRE(pkb.isRelationTrue("2", "3", RelationType::FOLLOWS));
  // unadded follows relation
  REQUIRE(pkb.isRelationTrue("5", "8", RelationType::FOLLOWS) == false);
  // follows not transitive
  REQUIRE(pkb.isRelationTrue("1", "4", RelationType::FOLLOWS) == false);

  // added follows and hence follows_star relation
  REQUIRE(pkb.isRelationTrue("2", "3", RelationType::FOLLOWS_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.isRelationTrue("1", "6", RelationType::FOLLOWS_STAR) == false);
  // follows_star should be transitive
  REQUIRE(pkb.isRelationTrue("1", "3", RelationType::FOLLOWS_STAR));

  // added parent relation
  REQUIRE(pkb.isRelationTrue("6", "7", RelationType::PARENT));
  // unadded parent relation
  REQUIRE(pkb.isRelationTrue("3", "5", RelationType::PARENT) == false);
  // parent not transitive
  REQUIRE(pkb.isRelationTrue("4", "7", RelationType::PARENT) == false);

  // added parent and hence parent_star relation
  REQUIRE(pkb.isRelationTrue("4", "5", RelationType::PARENT_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.isRelationTrue("1", "6", RelationType::PARENT_STAR) == false);
  // parent_star should be transitive
  REQUIRE(pkb.isRelationTrue("4", "7", RelationType::PARENT_STAR));

  // Returns if Relation(int, _)
  // no statements follow 7
  REQUIRE(pkb.isRelationTrueGivenFirstValue("7", RelationType::FOLLOWS) ==
          false);
  // statement 6 follows statement 5
  REQUIRE(pkb.isRelationTrueGivenFirstValue("5", RelationType::FOLLOWS));
  // statement 6 is the Parent* of 7
  REQUIRE(pkb.isRelationTrueGivenFirstValue("6", RelationType::PARENT_STAR));
  // statement 1 is not the parent of any statement
  REQUIRE(pkb.isRelationTrueGivenFirstValue("1", RelationType::PARENT) == false);
}
