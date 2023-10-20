#include "../../../spa/src/program_knowledge_base/PatternDatabase.h"
#include "catch.hpp"

std::shared_ptr<TreeNode> buildTree20() {
  // b * c;
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));
  return std::make_shared<TreeNode>(multiply);
}

std::shared_ptr<TreeNode> buildTree21() {
  // a + b * c
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));
  TreeNode add =
      TreeNode("+", std::make_shared<TreeNode>("a", nullptr, nullptr),
               std::make_shared<TreeNode>(multiply));
  return std::make_shared<TreeNode>(add);
}

std::shared_ptr<TreeNode> buildTree22() {
  // c - a
  TreeNode minus =
      TreeNode("-", std::make_shared<TreeNode>("c", nullptr, nullptr),
               std::make_shared<TreeNode>("a", nullptr, nullptr));
  return std::make_shared<TreeNode>(minus);
}

std::shared_ptr<TreeNode> buildTree23() {
  // c - a
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));
  TreeNode minus = TreeNode("-", std::make_shared<TreeNode>(multiply),
                            std::make_shared<TreeNode>("a", nullptr, nullptr));
  return std::make_shared<TreeNode>(minus);
}

TEST_CASE("Pattern Database insertion and retrieval") {
  PatternDatabase db = PatternDatabase();

  /*
   Line 2: x = b * c
   Line 3: x = a + b * c;
   Line 4: y = c - a;
   Line 5: x = b * c - a;
  */
  db.insert(PatternType::ASSIGN, "2", "x", buildTree20());
  db.insert(PatternType::ASSIGN, "3", "x", buildTree21());
  db.insert(PatternType::ASSIGN, "4", "y", buildTree22());
  db.insert(PatternType::ASSIGN, "5", "x", buildTree23());

  // x = _
  std::unordered_set<std::string> expected = {"2", "3", "5"};
  std::unordered_set<std::string> actual = db.getStatementNumbersGivenLHS("x");
  REQUIRE(expected == actual);

  // y = _
  expected = {"4"};
  actual = db.getStatementNumbersGivenLHS("y");
  REQUIRE(expected == actual);

  // z = _
  expected = {};
  actual = db.getStatementNumbersGivenLHS("z");
  REQUIRE(expected == actual);
}