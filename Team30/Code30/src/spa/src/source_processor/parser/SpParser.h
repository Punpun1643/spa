#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include "../../shared/parser/AParser.h"
#include "../../shared/parser/node/TreeNode.h"
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

  static std::shared_ptr<TreeNode> buildExprTreeAndValidate(
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  void parse() override;

  ~SpParser() = default;

 private:
  int currStmtIndex = 1;

  std::shared_ptr<ProgramNode> sourceProgramNode;

  int Precedence(std::string const& op) override;

  static bool isOperator(std::string const& tokenVal);

  static bool isComparisonOperator(std::string const& tokenVal);

  static bool isLogicalOperator(std::string const& tokenVal);

  static bool isMathematicalOperator(std::string const& tokenVal);

  static bool isPossibleRelFactor(std::shared_ptr<Token> token);

  static bool isAndOrOrToken(std::shared_ptr<Token> token);

  static bool isLeftParenthesisToken(std::shared_ptr<Token> token);

  static bool isRightParenthesisToken(std::shared_ptr<Token> token);

  static bool isNotToken(std::shared_ptr<Token> token);

  void handleWordOrIntegerToken(
      std::queue<std::shared_ptr<std::string>>& postFixQueue,
      std::unordered_set<std::string>& variables,
      std::unordered_set<int>& constants);

  void handleOperatorToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  void handleLeftParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount);

  void handleCondExprLeftParenthesisToken(
      std::shared_ptr<Token> const& currToken,
      std::stack<std::shared_ptr<Token>>& operatorStack, int& parenCount);

  void condExprHandleRightParenthesisToken(
      std::stack<std::shared_ptr<Token>>& operatorStack,
      std::queue<std::shared_ptr<Token>>& postFixQueue);

  void handleCondExprWordToken(std::shared_ptr<Token> const& currToken,
                               std::unordered_set<std::string>& variables,
                               std::stack<std::shared_ptr<Token>>& tokenStack);

  void handleCondExprIntegerToken(
      std::shared_ptr<Token> const& currToken,
      std::unordered_set<int>& constants,
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void assignHandleRightParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue, int& parenCount);

  void trackOperatorAndOperand(std::vector<int>& constAppearances,
                               std::vector<std::string>& varAppearances);

  void buildCondExprPostFix(std::queue<std::shared_ptr<Token>>& postFixQueue);

  void validateTokenStack(std::queue<std::shared_ptr<Token>>& postFixQueue,
                          std::stack<std::shared_ptr<Token>>& tokenStack,
                          std::unordered_set<std::string>& variables,
                          std::unordered_set<int>& constants);

  void validateTokenStackSize(std::stack<std::shared_ptr<Token>>& tokenStack,
                              int size);

  void validateWordOrIntegerToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void isTopStackNotWordOrIntegerToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void validateComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void isTopStackNotComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);
};
