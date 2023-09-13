#include "../../spa/src/program_knowledge_base/tables/DictionaryTable.h"
#include "../../spa/src/program_knowledge_base/tables/UFDSTable.h"
#include "catch.hpp"

TEST_CASE("Parent & Follows") {
  DictionaryTable p = DictionaryTable();
  p.insert(1, 4);
  p.insert(4, 7);

  REQUIRE(p.isRelated(1, 7) == false);
  REQUIRE(p.isRelated(1, 4));
  REQUIRE(p.isRelated(2, 4) == false);
}

TEST_CASE("Parents Star & Follows Star") {
  UFDSTable ps = UFDSTable();

  ps.insert(1, 4);
  ps.insert(4, 7);

  REQUIRE(ps.isRelated(1, 4));
  REQUIRE(ps.isRelated(1, 7));
  REQUIRE(ps.isRelated(2, 7) == false);
}
