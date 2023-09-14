#include "../../spa/src/program_knowledge_base/tables/DictionaryTable.h"
#include "../../spa/src/program_knowledge_base/tables/UFDSTable.h"
#include "catch.hpp"

TEST_CASE("Parent & Follows (DictionaryTable)") {
  DictionaryTable p = DictionaryTable();
  p.insert("1", "4");
  p.insert("4", "7");

  // added relation
  REQUIRE(p.isRelated("1", "4"));
  // non reflexive
  REQUIRE(p.isRelated("4", "1") == false);
  // non transitive
  REQUIRE(p.isRelated("1", "7") == false);
  // non added relation
  REQUIRE(p.isRelated("2", "4") == false);
}

TEST_CASE("Parents Star & Follows Star (UFDSTable)") {
  UFDSTable ps = UFDSTable();

  ps.insert("1", "4");
  ps.insert("4", "7");

  // added relation
  REQUIRE(ps.isRelated("1", "4"));
  // non reflexive
  REQUIRE(ps.isRelated("4", "1") == false);
  // transitive
  REQUIRE(ps.isRelated("1", "7"));
  // non reflexive + transitive
  REQUIRE(ps.isRelated("7", "1") == false);
  // non added relation
  REQUIRE(ps.isRelated("2", "7") == false);
}
