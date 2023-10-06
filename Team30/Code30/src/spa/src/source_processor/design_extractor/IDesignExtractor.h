#pragma once

#include <memory>

#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/AssignNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"
#include "../node/stmt_node/WhileNode.h"
#include "program_knowledge_base/PKBSPInterface.h"

class IDesignExtractor {
 public:
  virtual void extractFromProgram(std::shared_ptr<ProgramNode> node) = 0;

  virtual void extractFromProcedure(std::shared_ptr<ProcedureNode> node) = 0;

  virtual void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) = 0;

  virtual void extractFromCall(std::shared_ptr<CallNode> node) = 0;

  virtual void extractFromPrint(std::shared_ptr<PrintNode> node) = 0;

  virtual void extractFromRead(std::shared_ptr<ReadNode> node) = 0;

  virtual void extractFromWhile(std::shared_ptr<WhileNode> node) = 0;

  virtual void extractFromIf(std::shared_ptr<IfNode> node) = 0;

  virtual void extractFromAssign(std::shared_ptr<AssignNode> node) = 0;

  virtual ~IDesignExtractor() = default;
};