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

class ParentExtractor : public DesignExtractor {
 public:
  explicit ParentExtractor(PKBSPInterface& pkb);

  void extractFromWhile(std::shared_ptr<WhileNode> node) override;

  void extractFromIf(std::shared_ptr<IfNode> node) override;

  ~ParentExtractor() = default;

 private:
  PKBSPInterface& pkb;
};