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

TEST_CASE("Test CFGNode has immediate path", "[hasImmediatePath]") {
  SECTION("adsads") {
    /*
     * 1 -> 2 -> 3 -> 4
     */
    auto stmt1 = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "x");
    auto stmt2 = std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "y");
    auto stmt3 = std::make_shared<AssignNode>(
        3, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x"},
        std::unordered_set<int>{}, "x", nullptr);
    auto stmt4 = std::make_shared<CallNode>(4, StmtType::CALL_STMT, "proc1");

    std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(stmt1);
    std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(stmt2);
    std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(stmt3);
    std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(stmt4);

    cfg1->addOutgoingNode(cfg2);
    cfg2->addOutgoingNode(cfg3);
    cfg3->addOutgoingNode(cfg4);

    cfg2->addIncomingNode(cfg1);
    cfg3->addIncomingNode(cfg2);
    cfg4->addIncomingNode(cfg3);

    REQUIRE(CFGNode::HasImmediatePath(cfg1, cfg2));
    REQUIRE(CFGNode::HasImmediatePath(cfg2, cfg3));
    REQUIRE(CFGNode::HasImmediatePath(cfg3, cfg4));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg1, cfg3));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg1, cfg4));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg2, cfg4));

    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg2, cfg1));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg3, cfg2));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg4, cfg3));

    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg3, cfg1));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg4, cfg2));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg4, cfg1));

    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg1, cfg1));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg2, cfg2));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg3, cfg3));
    REQUIRE_FALSE(CFGNode::HasImmediatePath(cfg4, cfg4));
  }
}