#include "../../../spa/src/program_knowledge_base/EntityDatabase.h"
#include "catch.hpp"

TEST_CASE("Successful insert and retrieval") {
  EntityDatabase db = EntityDatabase();
  db.InsertEntity(EntityType::PROCEDURE, "main");
  db.InsertEntity(EntityType::IF, "1");
  db.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "2", "sub");
  db.InsertEntity(EntityType::WHILE, "3");
  db.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "5", "sub");

  REQUIRE(db.Get(EntityType::PROCEDURE)->size() == 1);
  REQUIRE(db.Get(EntityType::CALL)->size() == 2);
  REQUIRE(db.Get(EntityType::CONSTANT)->size() == 0);

  std::shared_ptr<std::unordered_set<std::string>> if_set =
      db.Get(EntityType::IF);
  REQUIRE(if_set->find("1") != if_set->end());
  REQUIRE(if_set->find("3") == if_set->end());

  std::shared_ptr<std::unordered_set<std::string>> while_set =
      db.Get(EntityType::WHILE);
  REQUIRE(while_set->find("3") != while_set->end());
}