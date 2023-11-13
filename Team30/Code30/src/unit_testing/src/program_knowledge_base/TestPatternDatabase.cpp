#include "../../../spa/src/program_knowledge_base/patterns/PatternDatabase.h"
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
  db.InsertAssignment("2", "x", buildTree20());
  db.InsertAssignment("3", "x", buildTree21());
  db.InsertAssignment("4", "y", buildTree22());
  db.InsertAssignment("5", "x", buildTree23());
  std::unordered_set<std::string> assign_stmts = {"2", "3", "4", "5"};

  /*
      Line 6: while (x + 1 + z < y + z + a + c) {
      Line 7:   if (a > b) {
      Line 8:       while (c < b) {
      Line 9:           if (1 < 3) {
      Line 10:              print a;
  */
  db.InsertCondVar(EntityType::WHILE, "6", "x");
  db.InsertCondVar(EntityType::WHILE, "6", "z");
  db.InsertCondVar(EntityType::WHILE, "6", "y");
  db.InsertCondVar(EntityType::WHILE, "6", "z");
  db.InsertCondVar(EntityType::WHILE, "6", "a");
  db.InsertCondVar(EntityType::WHILE, "6", "c");
  db.InsertCondVar(EntityType::IF, "7", "a");
  db.InsertCondVar(EntityType::IF, "7", "b");
  db.InsertCondVar(EntityType::WHILE, "8", "c");
  db.InsertCondVar(EntityType::WHILE, "8", "b");
  std::unordered_set<std::string> assignments = {"2", "3", "4", "5"};
  std::unordered_set<std::string> vars = {"a", "b", "c", "x", "y", "z"};

  std::shared_ptr<TreeNode> dummy_node =
      std::make_shared<TreeNode>("", nullptr, nullptr);

  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));

  std::vector<std::string> expected;
  std::vector<std::string> actual;
  std::vector<std::pair<std::string, std::string>> expected_pairs;
  std::vector<std::pair<std::string, std::string>> actual_pairs;

  SECTION("Test Assignment Patterns") {
    // x = _
    expected = {"2", "3", "5"};
    actual = db.GetMatchingAssignStmts("x", dummy_node, MatchType::WILD_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // y = _
    expected = {"4"};
    actual = db.GetMatchingAssignStmts("y", dummy_node, MatchType::WILD_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // z = _
    expected = {};
    actual = db.GetMatchingAssignStmts("z", dummy_node, MatchType::WILD_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // _ = b * c
    expected = {"2"};
    actual = db.GetMatchingAssignStmts(assign_stmts,
                                       std::make_shared<TreeNode>(multiply),
                                       MatchType::EXACT_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // _ = _b * c_
    expected = {"2", "3", "5"};
    actual = db.GetMatchingAssignStmts(assign_stmts,
                                       std::make_shared<TreeNode>(multiply),
                                       MatchType::PARTIAL_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // _ = _c - a_
    expected = {"4"};
    actual = db.GetMatchingAssignStmts(assign_stmts, buildTree22(),
                                       MatchType::PARTIAL_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // x = _c - a_
    expected = {};
    actual =
        db.GetMatchingAssignStmts("x", buildTree22(), MatchType::PARTIAL_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // _ = _
    expected = {"2", "3", "4", "5"};
    actual = db.GetMatchingAssignStmts(assign_stmts, dummy_node,
                                       MatchType::WILD_MATCH);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // _ = _b * c_
    expected_pairs = {{"2", "x"}, {"3", "x"}, {"5", "x"}};
    actual_pairs = db.GetMatchingAssignStmtLhsVarPairs(
        buildTree20(), MatchType::PARTIAL_MATCH, assignments, vars);
    std::sort(actual_pairs.begin(), actual_pairs.end());
    REQUIRE(expected_pairs == actual_pairs);

    // _ = b * c
    expected_pairs = {{"2", "x"}};
    actual_pairs = db.GetMatchingAssignStmtLhsVarPairs(
        buildTree20(), MatchType::EXACT_MATCH, assignments, vars);
    std::sort(actual_pairs.begin(), actual_pairs.end());
    REQUIRE(expected_pairs == actual_pairs);
  }

  SECTION("Test While and If Patterns") {
    /*
      Line 6: while (x + 1 + z < y + z + a + c) {
      Line 7:   if (a > b) {
      Line 8:       while (c < b) {
      Line 9:           if (1 < 3) {
      Line 10:              print a;
    */

    std::unordered_set<std::string> while_stmts = {"6", "8"};
    std::unordered_set<std::string> if_stmts = {"7", "9"};
    std::unordered_set<std::string> vars = {"x", "y", "z", "a", "b", "c"};

    // While statements with variables
    expected = {"6", "8"};
    actual = db.GetContainerStmtsWithControlVar(EntityType::WHILE);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // If statements with variables
    expected = {"7"};
    actual = db.GetContainerStmtsWithControlVar(EntityType::IF);
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // If statements with var a
    expected = {"7"};
    actual = db.GetContainerStmtsWithGivenControlVar(EntityType::IF, "a");
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // If statements with var z
    expected = {};
    actual = db.GetContainerStmtsWithGivenControlVar(EntityType::IF, "z");
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // While statements with var c
    expected = {"6", "8"};
    actual = db.GetContainerStmtsWithGivenControlVar(EntityType::WHILE, "c");
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // While statements with var a
    expected = {"6"};
    actual = db.GetContainerStmtsWithGivenControlVar(EntityType::WHILE, "a");
    std::sort(actual.begin(), actual.end());
    REQUIRE(expected == actual);

    // all while <stmt#, var> pairs
    expected_pairs = {
        {"6", "a"}, {"6", "c"}, {"6", "x"}, {"6", "y"},
        {"6", "z"}, {"8", "b"}, {"8", "c"},
    };
    actual_pairs = db.GetContainerStmtControlVarPairs(EntityType::WHILE,
                                                      while_stmts, vars);
    std::sort(actual_pairs.begin(), actual_pairs.end());
    REQUIRE(expected_pairs == actual_pairs);

    std::unordered_set<std::string> while_stmts_reduced = {"6"};
    expected_pairs = {
        {"6", "a"}, {"6", "c"}, {"6", "x"}, {"6", "y"}, {"6", "z"}};
    actual_pairs = db.GetContainerStmtControlVarPairs(
        EntityType::WHILE, while_stmts_reduced, vars);
    std::sort(actual_pairs.begin(), actual_pairs.end());
    REQUIRE(expected_pairs == actual_pairs);

    std::unordered_set<std::string> vars_reduced = {"a", "c"};
    expected_pairs = {{"6", "a"}, {"6", "c"}, {"8", "c"}};
    actual_pairs = db.GetContainerStmtControlVarPairs(
        EntityType::WHILE, while_stmts, vars_reduced);
    std::sort(actual_pairs.begin(), actual_pairs.end());
    REQUIRE(expected_pairs == actual_pairs);

    // all if <stmt#, var> pairs
    expected_pairs = {{"7", "a"}, {"7", "b"}};
    actual_pairs =
        db.GetContainerStmtControlVarPairs(EntityType::IF, if_stmts, vars);
    std::sort(actual_pairs.begin(), actual_pairs.end());
    REQUIRE(expected_pairs == actual_pairs);

    expected_pairs = {};
    std::unordered_set<std::string> if_stmts_reduced = {"9"};
    actual_pairs = db.GetContainerStmtControlVarPairs(EntityType::IF,
                                                      if_stmts_reduced, vars);
    REQUIRE(expected_pairs == actual_pairs);
  }
}
