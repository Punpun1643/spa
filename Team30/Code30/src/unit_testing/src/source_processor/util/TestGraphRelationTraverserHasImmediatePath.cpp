#include "../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../../../spa/src/source_processor/node/util_node/CFGNode.h"
#include "../../../../spa/src/source_processor/util/GraphRelationTraverser.h"
#include "catch.hpp"

TEST_CASE("Test GraphTraverser has immediate path (i.e. Next)",
          "[HasImmediatePath]") {
  SECTION("Test immediate path without loop should extract correctly") {
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

    REQUIRE(GraphRelationTraverser::HasImmediatePath(cfg1, cfg2));
    REQUIRE(GraphRelationTraverser::HasImmediatePath(cfg2, cfg3));
    REQUIRE(GraphRelationTraverser::HasImmediatePath(cfg3, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg1, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg1, cfg4));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg2, cfg4));

    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg2, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg3, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg4, cfg3));

    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg3, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg4, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg4, cfg1));

    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg1, cfg1));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg2, cfg2));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg3, cfg3));
    REQUIRE_FALSE(GraphRelationTraverser::HasImmediatePath(cfg4, cfg4));
  }
}
