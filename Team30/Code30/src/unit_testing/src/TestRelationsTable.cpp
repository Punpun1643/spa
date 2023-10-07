#include <program_knowledge_base/tables/LinkedListTable.h>
#include "../../spa/src/program_knowledge_base/tables/DictionaryTable.h"
#include "catch.hpp"

TEST_CASE("Parent & Follows (DictionaryTable)") {
  DictionaryTable p = DictionaryTable();
  p.insert("1", "4");
  p.insert("4", "7");

  // added relation
  REQUIRE(p.isRelated("1", "4"));
  // non added relation
  REQUIRE(p.isRelated("2", "4") == false);

  // non reflexive
  REQUIRE(p.isRelated("4", "4") == false);
  // non symmetric
  REQUIRE(p.isRelated("4", "1") == false);
  // non transitive
  REQUIRE(p.isRelated("1", "7") == false);
}

TEST_CASE("Parents Star & Follows Star (LinkedListTable)") {
  LinkedListTable ps = LinkedListTable();

  ps.insert("1", "4");
  ps.insert("4", "7");

  // added relation
  REQUIRE(ps.isRelated("1", "4"));
  // non added relation
  REQUIRE(ps.isRelated("2", "7") == false);

  // non reflexive
  REQUIRE(ps.isRelated("4", "4") == false);
  // non symmetric
  REQUIRE(ps.isRelated("4", "1") == false);
  // transitive
  REQUIRE(ps.isRelated("1", "7"));
  // non symmetric + transitive
  REQUIRE(ps.isRelated("7", "1") == false);
}

TEST_CASE("Uses & Modifies (Dictionary Table)") {
  DictionaryTable um = DictionaryTable();

  um.insert("2", "foo");
  um.insert("2", "bar");
  um.insert("5", "foo");
  um.insert("main", "bar");

  // added relation
  REQUIRE(um.isRelated("2", "foo"));
  // non added relation
  REQUIRE(um.isRelated("3", "flan") == false);

  // non reflexive
  REQUIRE(um.isRelated("2", "2") == false);
  // non symmetric
  REQUIRE(um.isRelated("foo", "5") == false);

  REQUIRE(um.isRelated("main", "2") == false);

  // TODO: Case where procedure contains a statement that has a known Relation
  // But no way to know if procedure contains statement?
  // Eg. "main" proc contains stmt 2 and Uses/Modifies(2, foo) is true
  // REQUIRE(ps.isRelated("main", "foo"));
}
