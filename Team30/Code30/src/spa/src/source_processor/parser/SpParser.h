#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"
#include "../node/stmt_node/WhileNode.h"
#include "../node/util_node/CondExprNode.h"

class SpParser : public AParser {
 public:
  SpParser(std::vector<std::shared_ptr<Token>> tokens);

  std::shared_ptr<ProgramNode> parseProgram();

  std::shared_ptr<ProcedureNode> parseProcedure();

  std::shared_ptr<StmtLstNode> parseStmtLst();

  std::shared_ptr<PrintNode> parsePrint();

  std::shared_ptr<ReadNode> parseRead();

  std::shared_ptr<CallNode> parseCall();

  std::shared_ptr<IfNode> parseIf();

  std::shared_ptr<WhileNode> parseWhile();

  std::shared_ptr<CondExprNode> parseCondExpr();

  std::shared_ptr<ProgramNode> getSourceProgramNode();

  void parse() override;

  ~SpParser() = default;

 private:
  int currStmtIndex = 1;

  int precedence(std::string const& op);

  bool isOperator(std::string const& tokenVal);

  bool isComparisonOperator(std::string const& tokenVal);

  bool isLogicalOperator(std::string const& tokenVal);

  std::shared_ptr<ProgramNode> sourceProgramNode;
};