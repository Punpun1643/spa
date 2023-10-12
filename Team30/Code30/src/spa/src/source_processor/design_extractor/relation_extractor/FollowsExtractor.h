#pragma once

#include <memory>

#include "../../node/ProcedureNode.h"
#include "../../node/ProgramNode.h"
#include "../../node/StmtLstNode.h"
#include "../../node/stmt_node/AssignNode.h"
#include "../../node/stmt_node/CallNode.h"
#include "../../node/stmt_node/IfNode.h"
#include "../../node/stmt_node/PrintNode.h"
#include "../../node/stmt_node/ReadNode.h"
#include "../../node/stmt_node/WhileNode.h"
#include "../DesignExtractor.h"

class FollowsExtractor : public DesignExtractor {
 public:
  explicit FollowsExtractor(PKBSPInterface& pkb);

  void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) override;

  ~FollowsExtractor() = default;

 private:
  PKBSPInterface& pkb;
};