#include <program_knowledge_base/tables/LinkedListTable.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../../../../spa/src/program_knowledge_base/utils/LinkedList.h"
#include "catch.hpp"

TEST_CASE("Linked list functionality check") {
  LinkedList ll = LinkedList();
  ll.AddEdge("1", "2");
  ll.AddEdge("2", "3");

  REQUIRE(ll.GetNode("1")->next.find(ll.GetNode("2")) !=
          ll.GetNode("1")->next.end());
  REQUIRE(ll.GetNode("2")->next.find(ll.GetNode("3")) !=
          ll.GetNode("2")->next.end());

  REQUIRE(ll.GetNode("3")->prev.find(ll.GetNode("2")) !=
          ll.GetNode("3")->prev.end());
  REQUIRE(ll.GetNode("2")->prev.find(ll.GetNode("1")) !=
          ll.GetNode("2")->prev.end());

  REQUIRE(ll.GetNode("3")->next.empty());
  REQUIRE(ll.GetNode("1")->prev.empty());
}

TEST_CASE("Linked list table check") {
  LinkedListTable llt = LinkedListTable();

  REQUIRE(llt.IsEmpty());

  llt.Insert("1", "2");
  llt.Insert("2", "3");

  llt.Insert("4", "5");
  llt.Insert("5", "6");
  llt.Insert("5", "7");
  llt.Insert("5", "8");

  llt.Insert("9", "10");

  REQUIRE(llt.IsRelated("1", "2"));
  REQUIRE(!llt.IsEmpty());
  REQUIRE(llt.IsRelated("2", "3"));
  REQUIRE(llt.IsRelated("1", "3"));

  REQUIRE(llt.IsRelated("4", "5"));
  REQUIRE(llt.IsRelated("4", "6"));
  REQUIRE(llt.IsRelated("4", "7"));
  REQUIRE(llt.IsRelated("4", "8"));
  REQUIRE(llt.IsRelated("5", "8"));

  REQUIRE(!llt.IsRelated("3", "4"));
  REQUIRE(!llt.IsRelated("1", "6"));
  REQUIRE(!llt.IsRelated("6", "7"));
  REQUIRE(!llt.IsRelated("6", "8"));

  REQUIRE(!llt.IsRelated("2", "1"));
  REQUIRE(!llt.IsRelated("3", "1"));
  REQUIRE(!llt.IsRelated("8", "7"));

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
  REQUIRE(llt.GetAllWithRelations(input) == expected);
  expected = {"2", "3", "5", "6", "7", "8", "10"};
  REQUIRE(llt.GetAllWithInverseRelations(input) == expected);
}