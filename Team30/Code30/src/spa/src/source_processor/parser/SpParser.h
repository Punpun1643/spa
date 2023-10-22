#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
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
#include "../node/stmt_node/StmtNode.h"
#include "../node/stmt_node/WhileNode.h"
#include "../node/util_node/CondExprNode.h"

class SpParser : public AParser {
 public:
  explicit SpParser(std::vector<std::shared_ptr<Token>> tokens);

  /**
   * @brief Get the Source Program Node object.
   *
   * @return Program node.
   */
  std::shared_ptr<ProgramNode> GetSourceProgramNode();

  void parse() override;

  ~SpParser() = default;

 protected:
  std::shared_ptr<ProgramNode> ParseProgram();

  std::shared_ptr<ProcedureNode> ParseProcedure();

  std::shared_ptr<StmtLstNode> ParseStmtLst();

  std::shared_ptr<PrintNode> ParsePrint();

  std::shared_ptr<ReadNode> ParseRead();

  std::shared_ptr<CallNode> ParseCall();

  std::shared_ptr<IfNode> ParseIf();

  std::shared_ptr<WhileNode> ParseWhile();

  std::shared_ptr<CondExprNode> ParseCondExpr();

  std::shared_ptr<AssignNode> ParseAssign(std::string const& varName);

 private:
  int curr_stmt_index = 1;

  std::shared_ptr<ProgramNode> source_program_node;

  int Precedence(std::string const& op) override;

  static bool IsOperator(std::string const& tokenVal);

  static bool IsComparisonOperator(std::string const& tokenVal);

  static bool IsLogicalOperator(std::string const& tokenVal);

  static bool IsMathematicalOperator(std::string const& tokenVal);

  static bool IsPossibleRelFactor(std::shared_ptr<Token> token);

  static bool IsAndOrOrToken(std::shared_ptr<Token> token);

  static bool IsLeftParenthesisToken(std::shared_ptr<Token> token);

  static bool IsRightParenthesisToken(std::shared_ptr<Token> token);

  static bool IsNotToken(std::shared_ptr<Token> token);

  void HandleWordOrIntegerToken(std::unordered_set<std::string>& variables,
                                std::unordered_set<int>& constants);

  void HandleOperatorToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  void HandleCondExprOperatorToken(
      std::shared_ptr<Token> const& token,
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void HandleLeftParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount);

  void HandleCondExprLeftParenthesisToken(
      std::shared_ptr<Token> const& currToken,
      std::stack<std::shared_ptr<Token>>& operatorStack, int& parenCount);

  void CondExprHandleRightParenthesisToken(
      std::stack<std::shared_ptr<Token>>& operatorStack,
      std::queue<std::shared_ptr<Token>>& postFixQueue);

  void HandleCondExprWordToken(std::shared_ptr<Token> const& currToken,
                               std::unordered_set<std::string>& variables,
                               std::stack<std::shared_ptr<Token>>& tokenStack);

  void HandleCondExprIntegerToken(
      std::shared_ptr<Token> const& currToken,
      std::unordered_set<int>& constants,
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void AssignHandleRightParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue, int& parenCount);

  void TrackOperatorAndOperand(std::vector<int>& constAppearances,
                               std::vector<std::string>& varAppearances);

  void BuildCondExprPostFix(std::queue<std::shared_ptr<Token>>& postFixQueue);

  void ValidateTokenStack(std::queue<std::shared_ptr<Token>>& postFixQueue,
                          std::stack<std::shared_ptr<Token>>& tokenStack,
                          std::unordered_set<std::string>& variables,
                          std::unordered_set<int>& constants);

  void ValidateTokenStackSize(std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateWordOrIntegerToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void IsTopStackNotWordOrIntegerToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateCondExprMathematicalOperatorToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateCondExprNotToken(std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateCondExprAndOrOrToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateCondExprComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& tokenStack,
      std::shared_ptr<Token> const& token);

  void ValidateCondExprFinalTokenStackState(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void IsTopStackNotComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& tokenStack);

  void ValidateStmtLst();

  void ValidateCondExprAndOrOrTokenPosition();

  void ValidateBalanceParentheses(int& parenCount);

  void HandleOperatorsStackAndPostfixQueue(
      std::stack<std::shared_ptr<Token>>& operatorStack,
      std::queue<std::shared_ptr<Token>>& postFixQueue);

  void TransferOperatorsToPostfixQueue(
      std::stack<std::shared_ptr<Token>>& operatorStack,
      std::queue<std::shared_ptr<Token>>& postFixQueue);

  bool IsAssignStmt();

  std::shared_ptr<StmtNode> ParseAssignStmt();

  std::shared_ptr<StmtNode> ParseNonAssignStmt();
};
