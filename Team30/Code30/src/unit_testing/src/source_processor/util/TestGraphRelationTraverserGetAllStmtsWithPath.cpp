#include "../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/IfNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/WhileNode.h"
#include "../../../../spa/src/source_processor/node/util_node/CFGNode.h"
#include "../../../../spa/src/source_processor/util/GraphRelationTraverser.h"
#include "catch.hpp"

TEST_CASE("Test GraphRelationTraverser get all stmts with path (i.e. Next*)") {
  /*
   *      1
   *      2
   *   3     4,5
   *      6
   *      7
   *   8     9
   *   10
   *
   * loop from 6 to 1
   * loop from 10 to 8
   */
  auto stmt1 = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "x");
  auto stmt2 = std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "y");
  auto stmt3 =
      std::make_shared<WhileNode>(3, StmtType::WHILE_STMT, nullptr, nullptr);
  auto stmt4 = std::make_shared<CallNode>(4, StmtType::CALL_STMT, "proc1");
  auto stmt5 = std::make_shared<ReadNode>(5, StmtType::READ_STMT, "x");
  auto stmt6 = std::make_shared<PrintNode>(6, StmtType::PRINT_STMT, "y");
  auto stmt7 =
      std::make_shared<IfNode>(7, StmtType::IF_STMT, nullptr, nullptr, nullptr);
  auto stmt8 = std::make_shared<CallNode>(8, StmtType::CALL_STMT, "proc1");
  auto stmt9 = std::make_shared<ReadNode>(9, StmtType::READ_STMT, "x");
  auto stmt10 = std::make_shared<PrintNode>(10, StmtType::PRINT_STMT, "y");

  std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(
      stmt1, StmtType::READ_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(
      stmt2, StmtType::PRINT_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(
      stmt3, StmtType::WHILE_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(
      stmt4, StmtType::CALL_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg5 = std::make_shared<CFGNode>(
      stmt5, StmtType::READ_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg6 = std::make_shared<CFGNode>(
      stmt6, StmtType::PRINT_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg7 = std::make_shared<CFGNode>(
      stmt7, StmtType::IF_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg8 = std::make_shared<CFGNode>(
      stmt8, StmtType::CALL_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg9 = std::make_shared<CFGNode>(
      stmt9, StmtType::READ_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});
  std::shared_ptr<CFGNode> cfg10 = std::make_shared<CFGNode>(
      stmt10, StmtType::PRINT_STMT, std::unordered_set<std::string>{},
      std::unordered_set<std::string>{});

  // 1 -> 2
  cfg1->AddOutgoingNode(cfg2);
  cfg2->AddIncomingNode(cfg1);

  // 2 -> 3, 4
  cfg2->AddOutgoingNode(cfg3);
  cfg2->AddOutgoingNode(cfg4);
  cfg3->AddIncomingNode(cfg2);
  cfg4->AddIncomingNode(cfg2);

  // 4 -> 5
  cfg4->AddOutgoingNode(cfg5);
  cfg5->AddIncomingNode(cfg4);

  // 3,5 -> 6
  cfg3->AddOutgoingNode(cfg6);
  cfg5->AddOutgoingNode(cfg6);
  cfg6->AddIncomingNode(cfg3);
  cfg6->AddIncomingNode(cfg5);

  // 6 -> 1 (loop)
  cfg6->AddOutgoingNode(cfg1);
  cfg1->AddIncomingNode(cfg6);

  // 1 -> 7
  cfg1->AddOutgoingNode(cfg7);
  cfg7->AddIncomingNode(cfg1);

  // 7 -> 8, 9
  cfg7->AddOutgoingNode(cfg8);
  cfg7->AddOutgoingNode(cfg9);
  cfg8->AddIncomingNode(cfg7);
  cfg9->AddIncomingNode(cfg7);

  // 8 <> 10
  cfg8->AddOutgoingNode(cfg10);
  cfg10->AddOutgoingNode(cfg8);
  cfg8->AddIncomingNode(cfg10);
  cfg10->AddIncomingNode(cfg8);

  std::unordered_set<std::string> expected;
  std::unordered_set<std::string> actual;

  SECTION("Test GetAllStmtsWithPathFrom") {
    expected = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg1));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg2));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg3));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg4));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg5));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg6));

    expected = {"8", "9", "10"};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg7));

    expected = {"8", "10"};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg8));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg10));

    expected = {};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathFrom(cfg9));
  }

  SECTION("Test GetAllStmtsWithPathTo") {
    expected = {"1", "2", "3", "4", "5", "6"};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg1));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg2));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg3));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg4));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg5));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg6));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg7));

    expected = {"1", "2", "3", "4", "5", "6", "7", "8", "10"};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg8));
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg10));

    expected = {"1", "2", "3", "4", "5", "6", "7"};
    REQUIRE(expected == GraphRelationTraverser::GetAllStmtsWithPathTo(cfg9));
  }
}
