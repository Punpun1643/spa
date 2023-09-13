#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../../spa/src/program_knowledge_base/utils/UFDS.h"
#include "catch.hpp"

TEST_CASE("Basic checks") {
  UFDS ufds = UFDS(8);

  REQUIRE(ufds.isSameSet(1, 2) == false);
  REQUIRE(ufds.isSameSet(2, 7) == false);

  REQUIRE(ufds.numDisjointSets() == 8);
  REQUIRE(ufds.findSet(7) == 7);
  REQUIRE(ufds.findSet(4) == 4);
  REQUIRE(ufds.sizeOf(3) == 1);
}

TEST_CASE("Check transitive property") {
  UFDS ufds = UFDS(12);
  REQUIRE(ufds.numDisjointSets() == 12);

  ufds.unionSets(1, 2);
  ufds.unionSets(3, 4);

  REQUIRE(ufds.numDisjointSets() == 10);
  REQUIRE(ufds.isSameSet(1, 2));
  REQUIRE(ufds.sizeOf(3) == 2);

  ufds.unionSets(1, 4);

  REQUIRE(ufds.numDisjointSets() == 9);
  REQUIRE(ufds.isSameSet(1, 3));
  REQUIRE(ufds.isSameSet(2, 4));
  REQUIRE(ufds.sizeOf(1) == 4);
}