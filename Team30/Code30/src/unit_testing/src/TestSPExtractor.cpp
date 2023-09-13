#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>

#include <iostream>
#include <source_processor/design_extractor/ExtractionController.cpp>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "catch.hpp"

//class PkbStub : public PkbApi {
//  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
//                     std::shared_ptr<StmtNode> stmt2) {
//    fCounter_insertFollows++;
//    std::cout << "InsertFollows called";
//    return true;
//  }
//
//  int getFCounter_insertFollows() { return fCounter_insertFollows; }
//
//  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
//      EntityType type) {
//    return NULL;
//  }
//  std::optional<std::pair<int, int>> getFollows(int s1_line_num,
//                                                EntityType s2_type) {
//    return std::make_optional<std::pair<int, int>>();
//  }
//  std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
//                                                int s2_line_num) {
//    return std::make_optional<std::pair<int, int>>();
//  };
//  virtual std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
//      EntityType s1_type, EntityType s2_type) {
//    return NULL;
//  }
//
// public:
//  int fCounter_insertFollows;
//};

class PkbApiStub : public PkbApi {
 public:
  int insertFollowsCallCount;
  PkbApiStub() : insertFollowsCallCount(0) {}

  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2) {
    ++insertFollowsCallCount;
    return true;
  }

  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();
    return output;
  }

  std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                EntityType s2_type) override {
    return std::nullopt;
  }

  std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                int s2_line_num) override {
    return std::nullopt;
  }

  std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type) override {
    return std::make_unique<std::vector<std::pair<int, int>>>();
  }
};

TEST_CASE("Extract Follows") {
  std::string varName = "var";
  PrintNode p1 = PrintNode(1, StmtType::PRINT_STMT, "varName");
  PrintNode p2 = PrintNode(2, StmtType::PRINT_STMT, "varName");
  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(std::make_shared<PrintNode>(p1));
  stmts.push_back(std::make_shared<PrintNode>(p2));
  StmtLstNode stl = StmtLstNode(stmts);
  ProcedureNode proc =
      ProcedureNode("proc", std::make_shared<StmtLstNode>(stl));
  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(std::make_shared<ProcedureNode>(proc));
  ProgramNode prog = ProgramNode(procs);

  PkbApiStub pkb = PkbApiStub();
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(std::make_shared<ProgramNode>(prog));
  //ec.executeStmtLstExtraction(std::make_shared<StmtLstNode>(stl));
  REQUIRE(pkb.insertFollowsCallCount == 1);
}