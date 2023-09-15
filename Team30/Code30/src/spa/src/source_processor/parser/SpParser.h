#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/CallNode.h"
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

  std::shared_ptr<WhileNode> parseWhile();

  std::shared_ptr<CondExprNode> parseCondExpr();

  std::shared_ptr<ProgramNode> getSourceProgramNode();

  void parse() override;

  ~SpParser() = default;

 private:
  int currStmtIndex = 1;

  int precedence(const std::string& op);

  bool isOperator(const std::string& tokenVal);

  bool isComparisonOperator(const std::string& tokenVal);

  bool isLogicalOperator(const std::string& tokenVal);

  std::shared_ptr<ProgramNode> sourceProgramNode;
};