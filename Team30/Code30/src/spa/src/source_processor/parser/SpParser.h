#pragma once

#include <memory>
#include <string>
#include <vector>
#include <queue>

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/AssignNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"
#include "../node/stmt_node/WhileNode.h"
#include "../node/util_node/CondExprNode.h"
#include "../node/util_node/TreeNode.h"

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

  std::shared_ptr<AssignNode> parseAssign(std::string const& varName);

  std::shared_ptr<ProgramNode> getSourceProgramNode();

  static std::shared_ptr<TreeNode> buildExprTreeAndValidate(std::queue<std::shared_ptr<std::string>>& postFixQueue);

  void parse() override;

  ~SpParser() = default;

 private:
  int currStmtIndex = 1;

  static int precedence(std::string const& op);

  static bool isOperator(std::string const& tokenVal);

  static bool isComparisonOperator(std::string const& tokenVal);

  static bool isLogicalOperator(std::string const& tokenVal);

  static bool isMathematicalOperator(std::string const& tokenVal);

  std::shared_ptr<ProgramNode> sourceProgramNode;
};
