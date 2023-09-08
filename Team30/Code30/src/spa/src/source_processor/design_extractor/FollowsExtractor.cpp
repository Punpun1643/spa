#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../../program_knowledge_base/PKB.h"
#include "IDesignExtractor.h"

class FollowsExtractor : public IDesignExtractor {
 public:
  void extractFromProgram(std::shared_ptr<ProgramNode> node) override {
    // TODO
  }

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override {
    // TODO
  }

  void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) override {
    std::vector<std::shared_ptr<StmtNode>> children = node->getChildren();
    for (int i = 0; i < children.size() - 1; i++) {
      // PKB::insertFollows(children[i], children[i + 1]);
    }
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
};
