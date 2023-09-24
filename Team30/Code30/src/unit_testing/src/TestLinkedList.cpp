#include <program_knowledge_base/tables/LinkedListTable.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../../spa/src/program_knowledge_base/utils/LinkedList.h"
#include "catch.hpp"

TEST_CASE("Linked list functionality check") {
  LinkedList ll = LinkedList();
  ll.addEdge("1", "2");
  ll.addEdge("2", "3");

  ll.addEdge("4", "5");

  ll.addEdge("5", "6");
  ll.addEdge("5", "7");
  ll.addEdge("5", "8");

  REQUIRE(ll.hasPath("1", "2"));
  REQUIRE(ll.hasPath("2", "3"));
  REQUIRE(ll.hasPath("1", "3"));

  REQUIRE(ll.hasPath("4", "5"));
  REQUIRE(ll.hasPath("4", "6"));
  REQUIRE(ll.hasPath("4", "7"));
  REQUIRE(ll.hasPath("4", "8"));
  REQUIRE(ll.hasPath("5", "8"));

  REQUIRE(!ll.hasPath("3", "4"));
  REQUIRE(!ll.hasPath("1", "6"));
  REQUIRE(!ll.hasPath("6", "7"));
  REQUIRE(!ll.hasPath("6", "8"));

  REQUIRE(!ll.hasPath("2", "1"));
  REQUIRE(!ll.hasPath("3", "1"));
  REQUIRE(!ll.hasPath("8", "7"));
}

TEST_CASE("Linked list table check") {
  LinkedListTable llt = LinkedListTable();

  REQUIRE(llt.isEmpty());

  llt.insert("1", "2");
  llt.insert("2", "3");
  llt.insert("4", "5");

  llt.insert("5", "6");
  llt.insert("5", "7");
  llt.insert("5", "8");

  REQUIRE(llt.isRelated("1", "2"));
  REQUIRE(!llt.isEmpty());
  REQUIRE(llt.isRelated("2", "3"));
  REQUIRE(llt.isRelated("1", "3"));

  REQUIRE(llt.isRelated("4", "5"));
  REQUIRE(llt.isRelated("4", "6"));
  REQUIRE(llt.isRelated("4", "7"));
  REQUIRE(llt.isRelated("4", "8"));
  REQUIRE(llt.isRelated("5", "8"));

  REQUIRE(!llt.isRelated("3", "4"));
  REQUIRE(!llt.isRelated("1", "6"));
  REQUIRE(!llt.isRelated("6", "7"));
  REQUIRE(!llt.isRelated("6", "8"));

  REQUIRE(!llt.isRelated("2", "1"));
  REQUIRE(!llt.isRelated("3", "1"));
  REQUIRE(!llt.isRelated("8", "7"));
}