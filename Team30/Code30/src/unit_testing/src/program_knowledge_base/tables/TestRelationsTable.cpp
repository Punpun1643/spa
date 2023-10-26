#include "program_knowledge_base/relations/tables/LinkedListTable.h"

#include "../../../../spa/src/program_knowledge_base/relations/tables/DictionaryTable.h"
#include "catch.hpp"

TEST_CASE("Parent & Follows (DictionaryTable)") {
  DictionaryTable p = DictionaryTable();
  p.Insert("1", "4");
  p.Insert("4", "7");

  // added relation
  REQUIRE(p.IsRelated("1", "4"));
  // non added relation
  REQUIRE(p.IsRelated("2", "4") == false);

  // non reflexive
  REQUIRE(p.IsRelated("4", "4") == false);
  // non symmetric
  REQUIRE(p.IsRelated("4", "1") == false);
  // non transitive
  REQUIRE(p.IsRelated("1", "7") == false);
}

TEST_CASE("Parents Star & Follows Star (LinkedListTable)") {
  LinkedListTable ps = LinkedListTable();

  ps.Insert("1", "4");
  ps.Insert("4", "7");

  // added relation
  REQUIRE(ps.IsRelated("1", "4"));
  // non added relation
  REQUIRE(ps.IsRelated("2", "7") == false);

  // non reflexive
  REQUIRE(ps.IsRelated("4", "4") == false);
  // non symmetric
  REQUIRE(ps.IsRelated("4", "1") == false);
  // transitive
  REQUIRE(ps.IsRelated("1", "7"));
  // non symmetric + transitive
  REQUIRE(ps.IsRelated("7", "1") == false);
}

TEST_CASE("Uses & Modifies (Dictionary Table)") {
  DictionaryTable um = DictionaryTable();

  um.Insert("2", "foo");
  um.Insert("2", "bar");
  um.Insert("5", "foo");
  um.Insert("main", "bar");

  // added relation
  REQUIRE(um.IsRelated("2", "foo"));
  // non added relation
  REQUIRE(um.IsRelated("3", "flan") == false);

  // non reflexive
  REQUIRE(um.IsRelated("2", "2") == false);
  // non symmetric
  REQUIRE(um.IsRelated("foo", "5") == false);

  REQUIRE(um.IsRelated("main", "2") == false);

  // TODO: Case where procedure contains a statement that has a known Relation
  // But no way to know if procedure contains statement?
  // Eg. "main" proc contains stmt 2 and Uses/Modifies(2, foo) is true
  // REQUIRE(ps.isRelated("main", "foo"));
}
