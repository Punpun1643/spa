#pragma once

#include <vector>

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"

class SpParser : public AParser {
 public:
  SpParser(std::vector<std::shared_ptr<Token>> tokens);

  std::shared_ptr<ProgramNode> parseProgram();

  std::shared_ptr<ProcedureNode> parseProcedure();

  std::shared_ptr<StmtLstNode> parseStmtLst();

  std::shared_ptr<PrintNode> parsePrint();

  std::shared_ptr<ReadNode> parseRead();

  std::shared_ptr<CallNode> parseCall();

  std::shared_ptr<ProgramNode> getSourceProgramNode();

  void parse() override;

  ~SpParser() = default;

 private:
  int currStmtIndex = 1;

  std::shared_ptr<ProgramNode> sourceProgramNode;
};