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

  REQUIRE(ll.getNode("1")->next.find(ll.getNode("2")) !=
          ll.getNode("1")->next.end());
  REQUIRE(ll.getNode("2")->next.find(ll.getNode("3")) !=
          ll.getNode("2")->next.end());

  REQUIRE(ll.getNode("3")->prev->value == "2");
  REQUIRE(ll.getNode("3")->prev->prev->value == "1");

  REQUIRE(ll.getNode("3")->next.empty());
  REQUIRE(ll.getNode("1")->prev == nullptr);
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
  llt.insert("9", "10");

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

  std::unordered_set<std::string> empty_set = {};
  auto input = std::make_shared<std::unordered_set<std::string>>();
  input->insert("1");
  input->insert("2");
  input->insert("3");
  input->insert("4");
  input->insert("5");
  input->insert("6");
  input->insert("7");
  input->insert("8");
  input->insert("9");
  input->insert("10");

  std::unordered_set<std::string> expected = {"1", "2", "4", "5", "9"};
  REQUIRE(llt.getAllRelated(input) == expected);
  expected = {"2", "3", "5", "6", "7", "8", "10"};
  REQUIRE(llt.getAllInverseRelated(input) == expected);
}