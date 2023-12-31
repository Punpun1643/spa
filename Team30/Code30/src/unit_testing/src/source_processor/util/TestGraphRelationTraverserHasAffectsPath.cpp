#include <memory>
#include <string>
#include <unordered_set>

#include "../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/IfNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/WhileNode.h"
#include "../../../../spa/src/source_processor/node/util_node/CFGNode.h"
#include "../../../../spa/src/source_processor/util/GraphRelationTraverser.h"
#include "catch.hpp"

TEST_CASE("Test has affects path for Affects relationship",
          "[HasAffectsPath]") {
  SECTION(
      "Test valid affect path without anything affected by procedure call,  "
      "extraction should return "
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

    /*
     * procedure Second {
01        x = 0;
02        i = 5;
03        while (i!=0) {
04            x = x + 2*y;
05            call Third;
06            i = i - 1; }
07        if (x==1) then {
08            x = x+1; }
          else {
09            z = 1; }
10        z = z + x + i;
11        y = z + 2;
12        x = x * y + z; }

    procedure Third {
      z = 5;
      v = z;
      print v; }
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

    std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(
        stmt1, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{"x"});
    std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(
        stmt2, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{"i"});
    std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(
        stmt3, StmtType::WHILE_STMT,
        std::unordered_set<std::string>{"i", "x", "y", "z", "v"},
        std::unordered_set<std::string>{"x", "z", "v", "i"});
    std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(
        stmt4, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x", "y"},
        std::unordered_set<std::string>{"x"});
    std::shared_ptr<CFGNode> cfg5 = std::make_shared<CFGNode>(
        stmt5, StmtType::CALL_STMT, std::unordered_set<std::string>{"z", "v"},
        std::unordered_set<std::string>{"z", "v"});
    std::shared_ptr<CFGNode> cfg6 = std::make_shared<CFGNode>(
        stmt6, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"i"},
        std::unordered_set<std::string>{"i"});
    std::shared_ptr<CFGNode> cfg7 = std::make_shared<CFGNode>(
        stmt7, StmtType::IF_STMT, std::unordered_set<std::string>{"x"},
        std::unordered_set<std::string>{"x", "z"});
    std::shared_ptr<CFGNode> cfg8 = std::make_shared<CFGNode>(
        stmt8, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x"},
        std::unordered_set<std::string>{"x"});
    std::shared_ptr<CFGNode> cfg9 = std::make_shared<CFGNode>(
        stmt9, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{"z"});
    std::shared_ptr<CFGNode> cfg10 = std::make_shared<CFGNode>(
        stmt10, StmtType::ASSIGN_STMT,
        std::unordered_set<std::string>{"z", "x", "i"},
        std::unordered_set<std::string>{"z"});
    std::shared_ptr<CFGNode> cfg11 = std::make_shared<CFGNode>(
        stmt11, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"z"},
        std::unordered_set<std::string>{"y"});
    std::shared_ptr<CFGNode> cfg12 = std::make_shared<CFGNode>(
        stmt12, StmtType::ASSIGN_STMT,
        std::unordered_set<std::string>{"x", "y", "z"},
        std::unordered_set<std::string>{"x"});

    std::shared_ptr<AffectsCache> cache = std::make_shared<AffectsCache>();

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

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg3, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg7, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg9, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg11, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg5, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg9, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg9, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg3, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg7, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg9, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg11, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg9, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg5, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg6, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg9, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg6, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg9, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg7, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg9, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg11, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg8, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg9, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg9, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg9, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg10, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg11, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg10, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg9, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg11, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg11, cfg12, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg5, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg6, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg7, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg8, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg9, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg10, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg11, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg12, cfg12, cache));
  }

  SECTION(
      "Test valid affect path with some path affected by procedure call should "
      "extract correctly") {
    /*
     * procedure p
     * assign 1 -> assign 2
     * assign 2 -> assign 3
     * assign 3 -> call 4
     * call 4 -> assign 5
     *
     * procedure q (called at call 4)
     */

    /*
     * procedure p {
1.    x = 1;
2.    y = 2;
3.    z = y;
4.    call q;
5.    z = x + y + z; }

     procedure q {
6.    x = 5;
7.    t = 4;
8.    if ( z > 0 ) then {
9.        t = x + 1; }
  else {
10.       y = z + x; }
11.  x = t + 1; }
     */

    auto stmt1 = std::make_shared<AssignNode>(
        1, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<int>{1}, "x", nullptr);
    auto stmt2 = std::make_shared<AssignNode>(
        2, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<int>{2}, "y", nullptr);
    auto stmt3 = std::make_shared<AssignNode>(
        3, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"y"},
        std::unordered_set<int>{}, "z", nullptr);
    auto stmt4 = std::make_shared<CallNode>(4, StmtType::CALL_STMT, "q");
    auto stmt5 = std::make_shared<AssignNode>(
        5, StmtType::ASSIGN_STMT,
        std::unordered_set<std::string>{"x", "y", "z"},
        std::unordered_set<int>{}, "z", nullptr);
    auto stmt10 = std::make_shared<AssignNode>(
        10, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"x", "z"},
        std::unordered_set<int>{}, "y", nullptr);

    std::shared_ptr<CFGNode> cfg1 = std::make_shared<CFGNode>(
        stmt1, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{"x"});
    std::shared_ptr<CFGNode> cfg2 = std::make_shared<CFGNode>(
        stmt2, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{},
        std::unordered_set<std::string>{"y"});
    std::shared_ptr<CFGNode> cfg3 = std::make_shared<CFGNode>(
        stmt3, StmtType::ASSIGN_STMT, std::unordered_set<std::string>{"y"},
        std::unordered_set<std::string>{"z"});
    std::shared_ptr<CFGNode> cfg4 = std::make_shared<CFGNode>(
        stmt4, StmtType::CALL_STMT,
        std::unordered_set<std::string>{"x", "z", "t"},
        std::unordered_set<std::string>{"x", "y", "t"});
    std::shared_ptr<CFGNode> cfg5 = std::make_shared<CFGNode>(
        stmt5, StmtType::ASSIGN_STMT,
        std::unordered_set<std::string>{"x", "y", "z"},
        std::unordered_set<std::string>{"z"});

    std::shared_ptr<CFGNode> cfg10 =
        std::make_shared<CFGNode>(stmt10, StmtType::ASSIGN_STMT,
                                  std::unordered_set<std::string>{"x", "z"},
                                  std::unordered_set<std::string>{"y"});

    cfg1->AddOutgoingNode(cfg2);
    cfg2->AddOutgoingNode(cfg3);
    cfg3->AddOutgoingNode(cfg4);
    cfg4->AddOutgoingNode(cfg5);

    cfg2->AddIncomingNode(cfg1);
    cfg3->AddIncomingNode(cfg2);
    cfg4->AddIncomingNode(cfg3);
    cfg5->AddIncomingNode(cfg4);

    std::shared_ptr<AffectsCache> cache = std::make_shared<AffectsCache>();

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg1, cfg5, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg2, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg2, cfg5, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg4, cache));
    REQUIRE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg5, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg4, cfg5, cache));

    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg1, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg2, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg3, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg4, cache));
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg5, cfg5, cache));

    // test cfg node from a different procedure
    REQUIRE_FALSE(GraphRelationTraverser::HasAffectsPath(cfg3, cfg10, cache));
  }
}
