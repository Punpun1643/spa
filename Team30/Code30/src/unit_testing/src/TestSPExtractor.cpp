#include "../../spa/src/program_knowledge_base/PkbApi.h"

#include "catch.hpp"
#include <source_processor/node/stmt_node/PrintNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/design_extractor/ExtractionController.cpp>

class PkbStub : public PkbApi {
  static bool insertFollows(std::shared_ptr<StmtNode> stmt1,
      std::shared_ptr<StmtNode> stmt2) {
    fCounter_insertFollows++;
  }

  static int getFCounter_insertFollows() { 
      return fCounter_insertFollows;
  }

 public:
  static int fCounter_insertFollows;

};

TEST_CASE("Extract Follows") {
  std::shared_ptr<PrintNode> p1 = std::make_shared<PrintNode>(
      new PrintNode(1, StmtType::PRINT_STMT, "var1"));
  std::shared_ptr<PrintNode> p2 = std::make_shared<PrintNode>(
      new PrintNode(2, StmtType::PRINT_STMT, "var1"));
  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(p1);
  stmts.push_back(p2);
  std::shared_ptr<StmtLstNode> stl = std::make_shared<StmtLstNode>(
      new StmtLstNode(stmts));
  std::shared_ptr<ProcedureNode> proc =
      std::make_shared<ProcedureNode>(new ProcedureNode("proc", stl));
  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(proc);
  std::shared_ptr<ProgramNode> prog =
      std::make_shared<ProgramNode>(new ProgramNode(procs));

  ExtractionController ec = ExtractionController();
  ec.executeExtract(prog);
  //int called = PkbStub::getFCounter_insertFollows();
  int calls = PkbStub::fCounter_insertFollows;
  REQUIRE(calls == 1);
}