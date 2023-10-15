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

TEST_CASE("Test HasAffectPath for Affect") {
  SECTION(
      "Test valid affect path with no call stmt extraction should return "
      "correct result") {
    /*
     * assign 1 -> assign 2
     * assign 2 -> while 3
     * while 3 -> assign 4
     * assign 4 -> call 5
     * call 5 -> assign 6
     * assign 6 -> while 3
     * while 3 -> if 7
     * if 7 -> assign 8
     * if 7 -> assign 9
     * assign 8 -> assign 10
     * assign 9 -> assign 10
     * assign 10 -> assign 11
     * assign 11 -> assign 12
     */

    auto stmt1 = std::make_shared<AssignNode>(
        1, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<int>{0}, "x", nullptr);
    auto stmt2 = std::make_shared<AssignNode>(
        2, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<int>{5}, "i", nullptr);
    auto stmt3 =
        std::make_shared<WhileNode>(3, StmtType::WHILE_STMT, nullptr, nullptr);
    auto stmt4 = std::make_shared<AssignNode>(
        4, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x", "y"},
        std::unordered_set<int>{2}, "x", nullptr);
    auto stmt5 = std::make_shared<CallNode>(5, StmtType::CALL_STMT, "proc1");
    auto stmt6 = std::make_shared<AssignNode>(
        6, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"i"},
        std::unordered_set<int>{1}, "i", nullptr);
    auto stmt7 = std::make_shared<IfNode>(7, StmtType::IF_STMT, nullptr,
                                          nullptr, nullptr);
    auto stmt8 = std::make_shared<AssignNode>(
        8, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x"},
        std::unordered_set<int>{1}, "x", nullptr);
    auto stmt9 = std::make_shared<AssignNode>(
        9, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<int>{1}, "z", nullptr);
    auto stmt10 = std::make_shared<AssignNode>(
        10, StmtType::ASSIGN_STMT,
        std::unordered_set<std::string>{"z", "x", "i"},
        std::unordered_set<int>{}, "z", nullptr);
    auto stmt11 = std::make_shared<AssignNode>(
        11, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"z"},
        std::unordered_set<int>{2}, "y", nullptr);
    auto stmt12 = std::make_shared<AssignNode>(
        12, StmtType::ASSIGN_STMT,
        std::unordered_set<std::string>{"x", "y", "z"},
        std::unordered_set<int>{}, "x", nullptr);

    std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(stmt1);
    std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(stmt2);
    std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(stmt3);
    std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(stmt4);
    std::shared_ptr<CFGNode> cfg5 = std::make_shared<CFGNode>(stmt5);
    std::shared_ptr<CFGNode> cfg6 = std::make_shared<CFGNode>(stmt6);
    std::shared_ptr<CFGNode> cfg7 = std::make_shared<CFGNode>(stmt7);
    std::shared_ptr<CFGNode> cfg8 = std::make_shared<CFGNode>(stmt8);
    std::shared_ptr<CFGNode> cfg9 = std::make_shared<CFGNode>(stmt9);
    std::shared_ptr<CFGNode> cfg10 = std::make_shared<CFGNode>(stmt10);
    std::shared_ptr<CFGNode> cfg11 = std::make_shared<CFGNode>(stmt11);
    std::shared_ptr<CFGNode> cfg12 = std::make_shared<CFGNode>(stmt12);

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

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg3));
    REQUIRE(CFGNode::HasAffectsPath(cfg1, cfg4));
    //        REQUIRE(CFGNode::HasAffectsPath(cfg1, cfg5));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg7));
    REQUIRE(CFGNode::HasAffectsPath(cfg1, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg9));
    REQUIRE(CFGNode::HasAffectsPath(cfg1, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg1, cfg11));
    REQUIRE(CFGNode::HasAffectsPath(cfg1, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg3));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg4));
    //        REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg5));
    REQUIRE(CFGNode::HasAffectsPath(cfg2, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg7));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg9));
    REQUIRE(CFGNode::HasAffectsPath(cfg2, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg11));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg2, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg3));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg4));
    //        REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg5));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg7));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg9));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg11));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg3, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg3));
    REQUIRE(CFGNode::HasAffectsPath(cfg4, cfg4));
    //        REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg5));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg7));
    REQUIRE(CFGNode::HasAffectsPath(cfg4, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg9));
    REQUIRE(CFGNode::HasAffectsPath(cfg4, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg4, cfg11));
    REQUIRE(CFGNode::HasAffectsPath(cfg4, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg3));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg4));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg5));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg7));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg9));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg11));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg5, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg3));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg4));
    //        REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg5));
    REQUIRE(CFGNode::HasAffectsPath(cfg6, cfg6));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg7));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg9));
    REQUIRE(CFGNode::HasAffectsPath(cfg6, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg11));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg6, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg3));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg4));
    //        REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg5));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg7));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg9));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg11));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg7, cfg12));

    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg1));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg2));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg3));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg4));
    //        REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg5));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg6));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg7));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg8));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg9));
    REQUIRE(CFGNode::HasAffectsPath(cfg8, cfg10));
    REQUIRE_FALSE(CFGNode::HasAffectsPath(cfg8, cfg11));
    REQUIRE(CFGNode::HasAffectsPath(cfg8, cfg12));
  }
}