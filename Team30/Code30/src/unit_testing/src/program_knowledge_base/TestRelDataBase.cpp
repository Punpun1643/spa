#include "../../../spa/src/program_knowledge_base/RelDatabase.h"
#include "catch.hpp"
#include "source_processor/node/stmt_node/AssignNode.h"
#include "source_processor/node/stmt_node/CallNode.h"
#include "source_processor/node/stmt_node/IfNode.h"
#include "source_processor/node/stmt_node/PrintNode.h"
#include "source_processor/node/stmt_node/ReadNode.h"
#include "source_processor/node/stmt_node/WhileNode.h"
#include "source_processor/node/util_node/CondExprNode.h"

AssignNode stmt1 = AssignNode(
    1, StmtType::ASSIGN_STMT, std::unordered_set<std::string>({"b", "c"}),
    std::unordered_set<int>(), "a", std::shared_ptr<TreeNode>());
std::shared_ptr<CFGNode> node1 = std::make_shared<CFGNode>(
    CFGNode(std::make_shared<AssignNode>(stmt1), StmtType::ASSIGN_STMT,
            std::unordered_set<std::string>({"b", "c"}),
            std::unordered_set<std::string>({"a"})));

CondExprNode cond_expr = CondExprNode(std::unordered_set<std::string>({"c"}),
                                      std::unordered_set<int>({0}));
WhileNode stmt2 = WhileNode(2, StmtType::WHILE_STMT,
                            std::make_shared<CondExprNode>(cond_expr),
                            std::shared_ptr<StmtLstNode>());
std::shared_ptr<CFGNode> node2 = std::make_shared<CFGNode>(
    CFGNode(std::make_shared<WhileNode>(stmt2), StmtType::WHILE_STMT,
            std::unordered_set<std::string>({"a", "c", "d"}),
            std::unordered_set<std::string>({"b"})));

PrintNode stmt3 = PrintNode(3, StmtType::PRINT_STMT, "c");
std::shared_ptr<CFGNode> node3 = std::make_shared<CFGNode>(CFGNode(
    std::make_shared<PrintNode>(stmt3), StmtType::PRINT_STMT,
    std::unordered_set<std::string>({"c"}), std::unordered_set<std::string>()));

CondExprNode cond_expr2 = CondExprNode(std::unordered_set<std::string>({"a"}),
                                       std::unordered_set<int>({2}));
IfNode stmt4 =
    IfNode(4, StmtType::IF_STMT, std::make_shared<CondExprNode>(cond_expr2),
           std::shared_ptr<StmtLstNode>(), std::shared_ptr<StmtLstNode>());
std::shared_ptr<CFGNode> node4 = std::make_shared<CFGNode>(
    CFGNode(std::make_shared<IfNode>(stmt4), StmtType::IF_STMT,
            std::unordered_set<std::string>({"a", "c", "d"}),
            std::unordered_set<std::string>({"b"})));

ReadNode stmt5 = ReadNode(5, StmtType::READ_STMT, "b");
std::shared_ptr<CFGNode> node5 = std::make_shared<CFGNode>(CFGNode(
    std::make_shared<ReadNode>(stmt5), StmtType::READ_STMT,
    std::unordered_set<std::string>(), std::unordered_set<std::string>({"b"})));

CallNode stmt6 = CallNode(6, StmtType::CALL_STMT, "sub");
std::shared_ptr<CFGNode> node6 = std::make_shared<CFGNode>(
    CFGNode(std::make_shared<CallNode>(stmt6), StmtType::CALL_STMT,
            std::unordered_set<std::string>({"c", "d"}),
            std::unordered_set<std::string>({"b"})));

AssignNode stmt7 =
    AssignNode(7, StmtType::ASSIGN_STMT, std::unordered_set<std::string>({"a"}),
               std::unordered_set<int>(), "c", std::shared_ptr<TreeNode>());
std::shared_ptr<CFGNode> node7 = std::make_shared<CFGNode>(
    CFGNode(std::make_shared<AssignNode>(stmt7), StmtType::ASSIGN_STMT,
            std::unordered_set<std::string>({"a"}),
            std::unordered_set<std::string>({"c"})));

AssignNode stmt8 = AssignNode(
    8, StmtType::ASSIGN_STMT, std::unordered_set<std::string>({"c", "d"}),
    std::unordered_set<int>(), "b", std::shared_ptr<TreeNode>());
std::shared_ptr<CFGNode> node8 = std::make_shared<CFGNode>(
    CFGNode(std::make_shared<AssignNode>(stmt8), StmtType::ASSIGN_STMT,
            std::unordered_set<std::string>({"c", "d"}),
            std::unordered_set<std::string>({"b"})));

void AddEdge(std::shared_ptr<CFGNode> n1, std::shared_ptr<CFGNode> n2) {
  n1->AddOutgoingNode(n2);
  n2->AddIncomingNode(n1);
}

TEST_CASE("Next/Next* and Affects") {
  /*
  procedure main() {
1.	a = b + c;
2.	while (c > 0) {
3.		print c;
4.      if (a < 2) {
5.          read b;
        } else {
6.          call sub;
        }
    }
7.	c = a;
  }

  procedure sub() {
8.	b = c + d;
  }
  */

  AddEdge(node1, node2);
  AddEdge(node2, node3);
  AddEdge(node3, node4);
  AddEdge(node4, node5);
  AddEdge(node4, node6);
  AddEdge(node5, node2);
  AddEdge(node6, node8);
  AddEdge(node8, node2);
  AddEdge(node2, node7);

  RelDatabase db = RelDatabase();
  db.InsertCFGNode("1", node1);
  db.InsertCFGNode("2", node2);
  db.InsertCFGNode("3", node3);
  db.InsertCFGNode("4", node4);
  db.InsertCFGNode("5", node5);
  db.InsertCFGNode("6", node6);
  db.InsertCFGNode("7", node7);
  db.InsertCFGNode("8", node8);

  SECTION("NEXT/*") {
    // valid NEXT relations
    REQUIRE(db.IsRelated(RelationType::NEXT, "1", "2"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "2", "3"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "3", "4"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "4", "5"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "4", "6"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "5", "2"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "6", "8"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "8", "2"));
    REQUIRE(db.IsRelated(RelationType::NEXT, "2", "7"));

    // invalid NEXT relations
    REQUIRE(!db.IsRelated(RelationType::NEXT, "6", "7"));
    REQUIRE(!db.IsRelated(RelationType::NEXT, "3", "2"));
    REQUIRE(!db.IsRelated(RelationType::NEXT, "1", "3"));
    REQUIRE(!db.IsRelated(RelationType::NEXT, "2", "2"));

    // valid NEXT_STAR relations
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "1", "2"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "2", "3"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "3", "4"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "4", "5"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "4", "6"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "5", "2"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "6", "8"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "8", "2"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "2", "7"));

    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "5", "5"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "5", "6"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "6", "3"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "1", "7"));
    REQUIRE(db.IsRelated(RelationType::NEXT_STAR, "1", "8"));

    // invalid NEXT_STAR relations
    REQUIRE(!db.IsRelated(RelationType::NEXT_STAR, "2", "1"));
    REQUIRE(!db.IsRelated(RelationType::NEXT_STAR, "7", "1"));
    REQUIRE(!db.IsRelated(RelationType::NEXT_STAR, "7", "2"));

    // all values next to if statement
    std::unordered_set<std::string> expected = {"5", "6"};
    std::unordered_set<std::string> actual =
        db.GetAllRelatedToValue(RelationType::NEXT, "4");
    REQUIRE(actual == expected);

    // get all values next to a while statement
    expected = {"3", "7"};
    actual = db.GetAllRelatedToValue(RelationType::NEXT, "2");
    REQUIRE(actual == expected);

    // get all values next to the last statement
    expected = {};
    actual = db.GetAllRelatedToValue(RelationType::NEXT, "7");
    REQUIRE(actual == expected);

    // get all values next to the first assign statement
    expected = {"2"};
    actual = db.GetAllRelatedToValue(RelationType::NEXT, "1");
    REQUIRE(actual == expected);

    // get all values next* to the first statement
    expected = {"2", "3", "4", "5", "6", "7", "8"};
    actual = db.GetAllRelatedToValue(RelationType::NEXT_STAR, "1");
    REQUIRE(actual == expected);

    // get all values next* to the while statement
    expected = {"2", "3", "4", "5", "6", "7", "8"};
    actual = db.GetAllRelatedToValue(RelationType::NEXT_STAR, "2");
    REQUIRE(actual == expected);

    // get all values next* to the read statement
    expected = {"2", "3", "4", "5", "6", "7", "8"};
    actual = db.GetAllRelatedToValue(RelationType::NEXT_STAR, "5");
    REQUIRE(actual == expected);

    // get all values next* to the last statement
    expected = {};
    actual = db.GetAllRelatedToValue(RelationType::NEXT_STAR, "7");
    REQUIRE(actual == expected);

    // filter all statement numbers with NEXT relations
    expected = {"1", "2", "3", "4", "5", "6", "8"};
    std::unordered_set<std::string> tmp = {"1", "2", "3", "4",
                                           "5", "6", "7", "8"};
    actual = db.GetAllWithRelations(
        RelationType::NEXT,
        std::make_shared<std::unordered_set<std::string>>(tmp));
    REQUIRE(actual == expected);

    // filter all statement numbers with NEXT_STAR relations
    expected = {"1", "2", "3", "4", "5", "6", "8"};
    tmp = {"1", "2", "3", "4", "5", "6", "7", "8"};
    actual = db.GetAllWithRelations(
        RelationType::NEXT_STAR,
        std::make_shared<std::unordered_set<std::string>>(tmp));
    REQUIRE(actual == expected);

    // filter all statement numbers that are NEXT of another statement
    expected = {"2", "3", "4", "5", "6", "7", "8"};
    tmp = {"1", "2", "3", "4", "5", "6", "7", "8"};
    actual = db.GetAllWithInverseRelations(
        RelationType::NEXT,
        std::make_shared<std::unordered_set<std::string>>(tmp));
    REQUIRE(actual == expected);

    // filter all statement numbers that are NEXT_STAR of another statement
    expected = {"2", "3", "4", "5", "6", "7", "8"};
    tmp = {"1", "2", "3", "4", "5", "6", "7", "8"};
    actual = db.GetAllWithInverseRelations(
        RelationType::NEXT_STAR,
        std::make_shared<std::unordered_set<std::string>>(tmp));
    REQUIRE(actual == expected);

    // get all statements directly before 8
    expected = {"6"};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT, "8");
    REQUIRE(actual == expected);

    // get all statements directly before 6
    expected = {"4"};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT, "6");
    REQUIRE(actual == expected);

    // get all statements before 6
    expected = {"1", "2", "3", "4", "5", "6", "8"};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT_STAR, "6");
    REQUIRE(actual == expected);

    // get all statements before 4
    expected = {"1", "2", "3", "4", "5", "6", "8"};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT_STAR, "4");
    REQUIRE(actual == expected);

    // get all statements before 2
    expected = {"1", "2", "3", "4", "5", "6", "8"};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT_STAR, "2");
    REQUIRE(actual == expected);

    // get all statements before 7
    expected = {"1", "2", "3", "4", "5", "6", "8"};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT_STAR, "7");
    REQUIRE(actual == expected);

    // get all statements before 1
    expected = {};
    actual = db.GetAllInverseRelatedToValue(RelationType::NEXT_STAR, "1");
    REQUIRE(actual == expected);
  }

  SECTION("AFFECTS") {
    // 1 non assignment statement
    REQUIRE(!db.IsRelated(RelationType::AFFECTS, "1", "2"));
    // 2 non assignment statements
    REQUIRE(!db.IsRelated(RelationType::AFFECTS, "2", "3"));
    // 1 assign 1 call statement
    REQUIRE(!db.IsRelated(RelationType::AFFECTS, "4", "6"));

    // valid affects
    REQUIRE(db.IsRelated(RelationType::AFFECTS, "1", "7"));

    // invalid affects
    REQUIRE(!db.IsRelated(RelationType::AFFECTS, "1", "8"));

    // get all values affected by statement 1
    std::unordered_set<std::string> expected = {"7"};
    std::unordered_set<std::string> actual =
        db.GetAllRelatedToValue(RelationType::AFFECTS, "1");
    REQUIRE(actual == expected);

    // get all values affected by statement 8
    expected = {};
    actual = db.GetAllRelatedToValue(RelationType::AFFECTS, "8");
    REQUIRE(actual == expected);

    // get all values affected by statement 7
    expected = {};
    actual = db.GetAllRelatedToValue(RelationType::AFFECTS, "7");
    REQUIRE(actual == expected);

    // get all statements that affect 7
    expected = {"1"};
    actual = db.GetAllInverseRelatedToValue(RelationType::AFFECTS, "7");
    REQUIRE(actual == expected);

    // get all statements that affect 5
    expected = {};
    actual = db.GetAllInverseRelatedToValue(RelationType::AFFECTS, "5");
    REQUIRE(actual == expected);

    std::unordered_set<std::string> tmp = {"1", "2", "3", "4",
                                           "5", "6", "7", "8"};
    expected = {"1"};
    actual = db.GetAllWithRelations(
        RelationType::AFFECTS,
        std::make_shared<std::unordered_set<std::string>>(tmp));
    REQUIRE(actual == expected);

    expected = {"7"};
    actual = db.GetAllWithInverseRelations(
        RelationType::AFFECTS,
        std::make_shared<std::unordered_set<std::string>>(tmp));
    REQUIRE(actual == expected);
  }
}
