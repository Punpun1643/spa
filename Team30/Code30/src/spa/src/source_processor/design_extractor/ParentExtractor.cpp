#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../../program_knowledge_base/PKB.h"
#include "DesignExtractor.cpp"

class ParentExtractor : public DesignExtractor {
 public:
  void extractFromProgram(std::shared_ptr<ProgramNode> node) override {
    // TODO
  }

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override {
    // TODO
  }

  void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) override {
    // TODO
  }

  void extractFromCall(std::shared_ptr<CallNode> node) override {
    // TODO
  }

  void extractFromPrint(std::shared_ptr<PrintNode> node) override {
    // TODO
  }

  void extractFromRead(std::shared_ptr<ReadNode> node) override {
    // TODO
  }
}
