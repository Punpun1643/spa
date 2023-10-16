#include "../../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/IfNode.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/StmtNode.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../../../../spa/src/source_processor/node/stmt_node/WhileNode.h"
#include "../../../../../spa/src/source_processor/node/util_node/CFGNode.h"
#include "catch.hpp"

TEST_CASE("Test CFG has path (i.e. Next*)", "[hasPath]") {
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

    std::shared_ptr<CFGNode> cfg1 =
        std::make_shared<CFGNode>(stmt1, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg2 =
        std::make_shared<CFGNode>(stmt2, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg3 =
        std::make_shared<CFGNode>(stmt3, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg4 =
        std::make_shared<CFGNode>(stmt4, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});

    cfg1->addOutgoingNode(cfg2);
    cfg2->addOutgoingNode(cfg3);
    cfg3->addOutgoingNode(cfg4);

    cfg2->addIncomingNode(cfg1);
    cfg3->addIncomingNode(cfg2);
    cfg4->addIncomingNode(cfg3);

    REQUIRE(CFGNode::HasPath(cfg1, cfg2));
    REQUIRE(CFGNode::HasPath(cfg1, cfg3));
    REQUIRE(CFGNode::HasPath(cfg1, cfg4));
    REQUIRE(CFGNode::HasPath(cfg2, cfg3));
    REQUIRE(CFGNode::HasPath(cfg2, cfg4));
    REQUIRE(CFGNode::HasPath(cfg3, cfg4));

    REQUIRE_FALSE(CFGNode::HasPath(cfg2, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg3, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg4, cfg3));

    REQUIRE_FALSE(CFGNode::HasPath(cfg3, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg4, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg4, cfg1));

    REQUIRE_FALSE(CFGNode::HasPath(cfg1, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg2, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg3, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg4, cfg4));
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

    std::shared_ptr<CFGNode> cfg1 =
        std::make_shared<CFGNode>(stmt1, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg2 =
        std::make_shared<CFGNode>(stmt2, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg3 =
        std::make_shared<CFGNode>(stmt3, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg4 =
        std::make_shared<CFGNode>(stmt4, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg5 =
        std::make_shared<CFGNode>(stmt5, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg6 =
        std::make_shared<CFGNode>(stmt6, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg7 =
        std::make_shared<CFGNode>(stmt7, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg8 =
        std::make_shared<CFGNode>(stmt8, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg9 =
        std::make_shared<CFGNode>(stmt9, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg10 =
        std::make_shared<CFGNode>(stmt10, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg11 =
        std::make_shared<CFGNode>(stmt11, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg12 =
        std::make_shared<CFGNode>(stmt12, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});

    cfg1->addOutgoingNode(cfg2);
    cfg2->addOutgoingNode(cfg3);
    cfg3->addOutgoingNode(cfg4);
    cfg4->addOutgoingNode(cfg5);
    cfg5->addOutgoingNode(cfg6);
    cfg6->addOutgoingNode(cfg3);
    cfg3->addOutgoingNode(cfg7);
    cfg7->addOutgoingNode(cfg8);
    cfg7->addOutgoingNode(cfg9);
    cfg8->addOutgoingNode(cfg10);
    cfg9->addOutgoingNode(cfg10);
    cfg10->addOutgoingNode(cfg11);
    cfg11->addOutgoingNode(cfg12);

    cfg2->addIncomingNode(cfg1);
    cfg3->addIncomingNode(cfg2);
    cfg4->addIncomingNode(cfg3);
    cfg5->addIncomingNode(cfg4);
    cfg6->addIncomingNode(cfg5);
    cfg3->addIncomingNode(cfg6);
    cfg7->addIncomingNode(cfg3);
    cfg8->addIncomingNode(cfg7);
    cfg9->addIncomingNode(cfg7);
    cfg10->addIncomingNode(cfg8);
    cfg10->addIncomingNode(cfg9);
    cfg11->addIncomingNode(cfg10);
    cfg12->addIncomingNode(cfg11);

    REQUIRE(CFGNode::HasPath(cfg1, cfg2));
    REQUIRE(CFGNode::HasPath(cfg1, cfg3));
    REQUIRE(CFGNode::HasPath(cfg1, cfg4));
    REQUIRE(CFGNode::HasPath(cfg1, cfg5));
    REQUIRE(CFGNode::HasPath(cfg1, cfg6));
    REQUIRE(CFGNode::HasPath(cfg1, cfg7));
    REQUIRE(CFGNode::HasPath(cfg1, cfg8));
    REQUIRE(CFGNode::HasPath(cfg1, cfg9));
    REQUIRE(CFGNode::HasPath(cfg1, cfg10));
    REQUIRE(CFGNode::HasPath(cfg1, cfg11));
    REQUIRE(CFGNode::HasPath(cfg1, cfg12));

    REQUIRE(CFGNode::HasPath(cfg2, cfg3));
    REQUIRE(CFGNode::HasPath(cfg2, cfg4));
    REQUIRE(CFGNode::HasPath(cfg2, cfg5));
    REQUIRE(CFGNode::HasPath(cfg2, cfg6));
    REQUIRE(CFGNode::HasPath(cfg2, cfg7));
    REQUIRE(CFGNode::HasPath(cfg2, cfg8));
    REQUIRE(CFGNode::HasPath(cfg2, cfg9));
    REQUIRE(CFGNode::HasPath(cfg2, cfg10));
    REQUIRE(CFGNode::HasPath(cfg2, cfg11));
    REQUIRE(CFGNode::HasPath(cfg2, cfg12));

    REQUIRE(CFGNode::HasPath(cfg3, cfg4));
    REQUIRE(CFGNode::HasPath(cfg3, cfg5));
    REQUIRE(CFGNode::HasPath(cfg3, cfg6));
    REQUIRE(CFGNode::HasPath(cfg3, cfg7));
    REQUIRE(CFGNode::HasPath(cfg3, cfg8));
    REQUIRE(CFGNode::HasPath(cfg3, cfg9));
    REQUIRE(CFGNode::HasPath(cfg3, cfg10));
    REQUIRE(CFGNode::HasPath(cfg3, cfg11));
    REQUIRE(CFGNode::HasPath(cfg3, cfg12));

    REQUIRE(CFGNode::HasPath(cfg4, cfg5));
    REQUIRE(CFGNode::HasPath(cfg4, cfg6));
    REQUIRE(CFGNode::HasPath(cfg4, cfg7));
    REQUIRE(CFGNode::HasPath(cfg4, cfg8));
    REQUIRE(CFGNode::HasPath(cfg4, cfg9));
    REQUIRE(CFGNode::HasPath(cfg4, cfg10));
    REQUIRE(CFGNode::HasPath(cfg4, cfg11));
    REQUIRE(CFGNode::HasPath(cfg4, cfg12));

    REQUIRE(CFGNode::HasPath(cfg5, cfg6));
    REQUIRE(CFGNode::HasPath(cfg5, cfg7));
    REQUIRE(CFGNode::HasPath(cfg5, cfg8));
    REQUIRE(CFGNode::HasPath(cfg5, cfg9));
    REQUIRE(CFGNode::HasPath(cfg5, cfg10));
    REQUIRE(CFGNode::HasPath(cfg5, cfg11));
    REQUIRE(CFGNode::HasPath(cfg5, cfg12));

    REQUIRE(CFGNode::HasPath(cfg6, cfg7));
    REQUIRE(CFGNode::HasPath(cfg6, cfg8));
    REQUIRE(CFGNode::HasPath(cfg6, cfg9));
    REQUIRE(CFGNode::HasPath(cfg6, cfg10));
    REQUIRE(CFGNode::HasPath(cfg6, cfg11));
    REQUIRE(CFGNode::HasPath(cfg6, cfg12));

    REQUIRE(CFGNode::HasPath(cfg7, cfg8));
    REQUIRE(CFGNode::HasPath(cfg7, cfg9));
    REQUIRE(CFGNode::HasPath(cfg7, cfg10));
    REQUIRE(CFGNode::HasPath(cfg7, cfg11));
    REQUIRE(CFGNode::HasPath(cfg7, cfg12));

    REQUIRE(CFGNode::HasPath(cfg8, cfg10));
    REQUIRE(CFGNode::HasPath(cfg8, cfg11));
    REQUIRE(CFGNode::HasPath(cfg8, cfg12));

    REQUIRE(CFGNode::HasPath(cfg9, cfg10));
    REQUIRE(CFGNode::HasPath(cfg9, cfg11));
    REQUIRE(CFGNode::HasPath(cfg9, cfg12));

    REQUIRE(CFGNode::HasPath(cfg10, cfg11));
    REQUIRE(CFGNode::HasPath(cfg10, cfg12));

    REQUIRE(CFGNode::HasPath(cfg11, cfg12));

    REQUIRE_FALSE(CFGNode::HasPath(cfg2, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg3, cfg2));
    REQUIRE(CFGNode::HasPath(cfg4, cfg3));
    REQUIRE(CFGNode::HasPath(cfg5, cfg4));
    REQUIRE(CFGNode::HasPath(cfg6, cfg5));
    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg6));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg7));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg7));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg8));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg9));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg10));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg11));

    REQUIRE_FALSE(CFGNode::HasPath(cfg3, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg4, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg5, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg6, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg1));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg1));

    REQUIRE_FALSE(CFGNode::HasPath(cfg4, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg5, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg6, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg2));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg2));

    REQUIRE(CFGNode::HasPath(cfg5, cfg3));
    REQUIRE(CFGNode::HasPath(cfg6, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg3));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg3));

    REQUIRE(CFGNode::HasPath(cfg6, cfg4));
    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg4));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg4));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg4));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg4));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg4));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg4));

    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg5));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg5));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg5));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg5));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg5));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg5));

    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg6));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg6));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg6));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg6));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg6));

    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg7));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg7));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg7));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg7));

    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg8));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg8));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg8));

    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg9));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg9));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg9));

    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg10));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg10));

    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg11));

    REQUIRE_FALSE(CFGNode::HasPath(cfg2, cfg2));
    REQUIRE(CFGNode::HasPath(cfg3, cfg3));
    REQUIRE(CFGNode::HasPath(cfg4, cfg4));
    REQUIRE(CFGNode::HasPath(cfg5, cfg5));
    REQUIRE(CFGNode::HasPath(cfg6, cfg6));
    REQUIRE_FALSE(CFGNode::HasPath(cfg7, cfg7));
    REQUIRE_FALSE(CFGNode::HasPath(cfg8, cfg8));
    REQUIRE_FALSE(CFGNode::HasPath(cfg9, cfg9));
    REQUIRE_FALSE(CFGNode::HasPath(cfg10, cfg10));
    REQUIRE_FALSE(CFGNode::HasPath(cfg11, cfg11));
    REQUIRE_FALSE(CFGNode::HasPath(cfg12, cfg12));
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

    std::shared_ptr<CFGNode> cfg1 =
        std::make_shared<CFGNode>(stmt1, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg2 =
        std::make_shared<CFGNode>(stmt2, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg3 =
        std::make_shared<CFGNode>(stmt3, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});
    std::shared_ptr<CFGNode> cfg4 =
        std::make_shared<CFGNode>(stmt4, std::unordered_set<std::string>{},
                                  std::unordered_set<std::string>{});

    cfg1->addOutgoingNode(cfg2);
    cfg2->addOutgoingNode(cfg3);
    cfg3->addOutgoingNode(cfg4);
    cfg4->addOutgoingNode(cfg1);

    cfg2->addIncomingNode(cfg1);
    cfg3->addIncomingNode(cfg2);
    cfg4->addIncomingNode(cfg3);
    cfg1->addIncomingNode(cfg4);

    REQUIRE(CFGNode::HasPath(cfg1, cfg2));
    REQUIRE(CFGNode::HasPath(cfg1, cfg3));
    REQUIRE(CFGNode::HasPath(cfg1, cfg4));
    REQUIRE(CFGNode::HasPath(cfg2, cfg3));
    REQUIRE(CFGNode::HasPath(cfg2, cfg4));
    REQUIRE(CFGNode::HasPath(cfg3, cfg4));

    REQUIRE(CFGNode::HasPath(cfg2, cfg1));
    REQUIRE(CFGNode::HasPath(cfg3, cfg2));
    REQUIRE(CFGNode::HasPath(cfg4, cfg3));

    REQUIRE(CFGNode::HasPath(cfg3, cfg1));
    REQUIRE(CFGNode::HasPath(cfg4, cfg2));
    REQUIRE(CFGNode::HasPath(cfg4, cfg1));

    REQUIRE(CFGNode::HasPath(cfg1, cfg1));
    REQUIRE(CFGNode::HasPath(cfg2, cfg2));
    REQUIRE(CFGNode::HasPath(cfg3, cfg3));
    REQUIRE(CFGNode::HasPath(cfg4, cfg4));
  }
}
