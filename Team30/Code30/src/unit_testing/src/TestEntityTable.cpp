#include "../../spa/src/program_knowledge_base/EntityDatabase.h"
#include "catch.hpp"

TEST_CASE("Successful insert and retrieval") {
  EntityDatabase db = EntityDatabase();
  db.insert(EntityType::PROCEDURE, "main");
  db.insert(EntityType::IF, "1");
  db.insert(EntityType::CALL, "2");
  db.insert(EntityType::WHILE, "3");
  db.insert(EntityType::CALL, "5");

  REQUIRE(db.get(EntityType::PROCEDURE).size() == 1);
  REQUIRE(db.get(EntityType::CALL).size() == 2);
  REQUIRE(db.get(EntityType::CONSTANT).size() == 0);

  unordered_set<std::string> if_set = db.get(EntityType::IF);
  REQUIRE(if_set.find("1") != if_set.end());
  REQUIRE(if_set.find("3") == if_set.end());

  unordered_set<std::string> while_set = db.get(EntityType::WHILE);
  REQUIRE(while_set.find("3") != while_set.end());
}
