#include "../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/WhileNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/IfNode.h"
#include "../../../../spa/src/source_processor/node/util_node/CFGNode.h"
#include "../../../../spa/src/source_processor/util/GraphRelationTraverser.h"
#include "catch.hpp"

TEST_CASE("Test GraphRelationTraverser has path (i.e. Next*)", "[HasPath]") {
  SECTION("Test path without loop should extract correctly") {
    /*
     * 1 -> 2 -> 3 -> 4
     */
    auto stmt1 = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "x");
    auto stmt2 = std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "y");
    auto stmt3 = std::make_shared<AssignNode>(
        3, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x"},
        std::unordered_set<int>{}, "x", nullptr);
    auto stmt4 = std::make_shared<CallNode>(4, StmtType::CALL_STMT, "proc1");

    std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(
        stmt1, StmtType::READ_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(
        stmt2, StmtType::PRINT_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(
        stmt3, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(
        stmt4, StmtType::CALL_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});

    cfg1->AddOutgoingNode(cfg2);
    cfg2->AddOutgoingNode(cfg3);
    cfg3->AddOutgoingNode(cfg4);

    cfg2->AddIncomingNode(cfg1);
    cfg3->AddIncomingNode(cfg2);
    cfg4->AddIncomingNode(cfg3);

    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg4));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg2, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg3, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg4, cfg3));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg3, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg4, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg4, cfg1));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg1, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg2, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg3, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg4, cfg4));
  }

  SECTION("Test path with loop") {
    /*
     * 1 -> 2
     * 2 -> 3
     * 3 -> 4
     * 4 -> 5
     * 5 -> 6
     * 6 -> 3
     * 3 -> 7
     * 7 -> 8
     * 7 -> 9
     * 8 -> 10
     * 9 -> 10
     * 10 -> 11
     * 11 -> 12
     */
    auto stmt1 = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "x");
    auto stmt2 = std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "y");
    auto stmt3 =
        std::make_shared<WhileNode>(3, StmtType::WHILE_STMT, nullptr, nullptr);
    auto stmt4 = std::make_shared<CallNode>(4, StmtType::CALL_STMT, "proc1");
    auto stmt5 = std::make_shared<ReadNode>(5, StmtType::READ_STMT, "x");
    auto stmt6 = std::make_shared<PrintNode>(6, StmtType::PRINT_STMT, "y");
    auto stmt7 = std::make_shared<IfNode>(7, StmtType::IF_STMT, nullptr,
                                          nullptr, nullptr);
    auto stmt8 = std::make_shared<CallNode>(8, StmtType::CALL_STMT, "proc1");
    auto stmt9 = std::make_shared<ReadNode>(9, StmtType::READ_STMT, "x");
    auto stmt10 = std::make_shared<PrintNode>(10, StmtType::PRINT_STMT, "y");
    auto stmt11 = std::make_shared<CallNode>(11, StmtType::CALL_STMT, "proc1");
    auto stmt12 = std::make_shared<ReadNode>(12, StmtType::READ_STMT, "x");

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
    std::shared_ptr<CFGNode> cfg11 = std::make_shared<CFGNode>(
        stmt11, StmtType::CALL_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg12 = std::make_shared<CFGNode>(
        stmt12, StmtType::READ_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});

    cfg1->AddOutgoingNode(cfg2);
    cfg2->AddOutgoingNode(cfg3);
    cfg3->AddOutgoingNode(cfg4);
    cfg4->AddOutgoingNode(cfg5);
    cfg5->AddOutgoingNode(cfg6);
    cfg6->AddOutgoingNode(cfg3);
    cfg3->AddOutgoingNode(cfg7);
    cfg7->AddOutgoingNode(cfg8);
    cfg7->AddOutgoingNode(cfg9);
    cfg8->AddOutgoingNode(cfg10);
    cfg9->AddOutgoingNode(cfg10);
    cfg10->AddOutgoingNode(cfg11);
    cfg11->AddOutgoingNode(cfg12);

    cfg2->AddIncomingNode(cfg1);
    cfg3->AddIncomingNode(cfg2);
    cfg4->AddIncomingNode(cfg3);
    cfg5->AddIncomingNode(cfg4);
    cfg6->AddIncomingNode(cfg5);
    cfg3->AddIncomingNode(cfg6);
    cfg7->AddIncomingNode(cfg3);
    cfg8->AddIncomingNode(cfg7);
    cfg9->AddIncomingNode(cfg7);
    cfg10->AddIncomingNode(cfg8);
    cfg10->AddIncomingNode(cfg9);
    cfg11->AddIncomingNode(cfg10);
    cfg12->AddIncomingNode(cfg11);

    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg5));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg6));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg7));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg5));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg6));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg7));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg5));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg6));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg7));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg5));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg6));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg7));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg6));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg7));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg7));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg7, cfg8));
    REQUIRE(GraphRelationTraverser::HasPath(cfg7, cfg9));
    REQUIRE(GraphRelationTraverser::HasPath(cfg7, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg7, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg7, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg8, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg8, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg8, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg9, cfg10));
    REQUIRE(GraphRelationTraverser::HasPath(cfg9, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg9, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg10, cfg11));
    REQUIRE(GraphRelationTraverser::HasPath(cfg10, cfg12));

    REQUIRE(GraphRelationTraverser::HasPath(cfg11, cfg12));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg2, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg3, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg5));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg6));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg7));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg7));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg8));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg9));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg10));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg11));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg3, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg4, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg5, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg6, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg1));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg4, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg5, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg6, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg2));

    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg3));

    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg4));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg5));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg5));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg5));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg5));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg5));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg5));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg6));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg6));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg6));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg6));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg6));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg7));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg7));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg7));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg7));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg8));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg8));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg8));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg9));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg9));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg9));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg10));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg10));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg11));

    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg2, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg5, cfg5));
    REQUIRE(GraphRelationTraverser::HasPath(cfg6, cfg6));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg7, cfg7));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg8, cfg8));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg9, cfg9));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg10, cfg10));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg11, cfg11));
    REQUIRE_FALSE(GraphRelationTraverser::HasPath(cfg12, cfg12));
  }

  SECTION("Test path with loop from end to start") {
    /*
     * 1 -> 2
     * 2 -> 3
     * 3 -> 4
     * 4 -> 1
     */
    auto stmt1 =
        std::make_shared<WhileNode>(1, StmtType::WHILE_STMT, nullptr, nullptr);
    auto stmt2 = std::make_shared<ReadNode>(2, StmtType::READ_STMT, "x");
    auto stmt3 = std::make_shared<PrintNode>(3, StmtType::PRINT_STMT, "y");
    auto stmt4 = std::make_shared<CallNode>(4, StmtType::CALL_STMT, "proc1");

    std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(
        stmt1, StmtType::WHILE_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(
        stmt2, StmtType::READ_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(
        stmt3, StmtType::PRINT_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(
        stmt4, StmtType::CALL_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{});

    cfg1->AddOutgoingNode(cfg2);
    cfg2->AddOutgoingNode(cfg3);
    cfg3->AddOutgoingNode(cfg4);
    cfg4->AddOutgoingNode(cfg1);

    cfg2->AddIncomingNode(cfg1);
    cfg3->AddIncomingNode(cfg2);
    cfg4->AddIncomingNode(cfg3);
    cfg1->AddIncomingNode(cfg4);

    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg4));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg4));

    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg1));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg3));

    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg1));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg1));

    REQUIRE(GraphRelationTraverser::HasPath(cfg1, cfg1));
    REQUIRE(GraphRelationTraverser::HasPath(cfg2, cfg2));
    REQUIRE(GraphRelationTraverser::HasPath(cfg3, cfg3));
    REQUIRE(GraphRelationTraverser::HasPath(cfg4, cfg4));
  }
}
