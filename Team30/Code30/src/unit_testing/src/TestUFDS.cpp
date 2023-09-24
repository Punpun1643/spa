#include <program_knowledge_base/tables/UFDSTable.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../../spa/src/program_knowledge_base/utils/UFDS.h"
#include "catch.hpp"

TEST_CASE("Basic checks") {
  UFDS ufds = UFDS();

  REQUIRE(ufds.isSameSet(1, 2) == false);
  REQUIRE(ufds.isSameSet(2, 7) == false);

  REQUIRE(ufds.findSet(7) == 7);
  REQUIRE(ufds.findSet(4) == 4);
  REQUIRE(ufds.sizeOf(3) == 1);
}

TEST_CASE("Check transitive property") {
  UFDS ufds = UFDS();

  ufds.unionSets(1, 2);
  ufds.unionSets(3, 4);

  REQUIRE(ufds.isSameSet(1, 2));
  REQUIRE(ufds.sizeOf(3) == 2);

  ufds.unionSets(1, 4);

  REQUIRE(ufds.isSameSet(1, 3));
  REQUIRE(ufds.isSameSet(2, 4));
  REQUIRE(ufds.sizeOf(1) == 4);
}

TEST_CASE("Check correct dynamic UFDS expansion") {
  UFDS ufds = UFDS();

  ufds.unionSets(1, 2);
  ufds.unionSets(2, 7);

  REQUIRE(ufds.isSameSet(1, 7));
  REQUIRE(ufds.sizeOf(5) == 1);
  REQUIRE(ufds.sizeOf(7) == 3);
  REQUIRE(ufds.sizeOf(11) == 1);
}

TEST_CASE("Check UFDS Table") {
  UFDSTable ut = UFDSTable();
  ut.insert("4", "5");
  ut.insert("4", "6");
  ut.insert("4", "7");
  ut.insert("7", "8");
  ut.insert("4", "10");
  ut.insert("14", "15");
  ut.insert("14", "16");
  ut.insert("20", "21");
  ut.insert("20", "22");
  ut.insert("20", "23");
  ut.insert("20", "24");
  ut.insert("24", "25");
  ut.insert("24", "26");

  for (int i = 1; i < 30; i++) {
    if (ut.isRelated("22", std::to_string(i))) {
      std::cout << i;
    }
  }
}