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

  std::shared_ptr<AssignNode> ParseAssign(std::string const& var_name);

 private:
  int curr_stmt_index = 1;

  std::shared_ptr<ProgramNode> source_program_node;

  int Precedence(std::string const& op) override;

  static bool IsOperator(std::string const& token_val);

  static bool IsComparisonOperator(std::string const& token_val);

  static bool IsLogicalOperator(std::string const& token_val);

  static bool IsMathematicalOperator(std::string const& token_val);

  static bool IsPossibleRelFactor(std::shared_ptr<Token> token);

  static bool IsAndOrOrToken(std::shared_ptr<Token> token);

  static bool IsLeftParenthesisToken(std::shared_ptr<Token> token);

  static bool IsRightParenthesisToken(std::shared_ptr<Token> token);

  static bool IsNotToken(std::shared_ptr<Token> token);

  void HandleWordOrIntegerToken(std::unordered_set<std::string>& variables,
                                std::unordered_set<int>& constants);

  void HandleOperatorToken(
      std::stack<std::shared_ptr<std::string>>& operator_stack,
      std::queue<std::shared_ptr<std::string>>& postfix_queue);

  void HandleCondExprOperatorToken(
      std::shared_ptr<Token> const& token,
      std::stack<std::shared_ptr<Token>>& token_stack);

  void HandleLeftParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operator_stack,
      int& paren_count);

  void HandleCondExprLeftParenthesisToken(
      std::shared_ptr<Token> const& curr_token,
      std::stack<std::shared_ptr<Token>>& operator_stack, int& paren_count);

  void CondExprHandleRightParenthesisToken(
      std::stack<std::shared_ptr<Token>>& operator_stack,
      std::queue<std::shared_ptr<Token>>& postfix_queue);

  void HandleCondExprWordToken(std::shared_ptr<Token> const& curr_token,
                               std::unordered_set<std::string>& variables,
                               std::stack<std::shared_ptr<Token>>& token_stack);

  void HandleCondExprIntegerToken(
      std::shared_ptr<Token> const& curr_token,
      std::unordered_set<int>& constants,
      std::stack<std::shared_ptr<Token>>& token_stack);

  void AssignHandleRightParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operator_stack,
      std::queue<std::shared_ptr<std::string>>& postfix_queue,
      int& paren_count);

  void TrackOperatorAndOperand(std::vector<int>& const_appearances,
                               std::vector<std::string>& var_appearances);

  void BuildCondExprPostFix(std::queue<std::shared_ptr<Token>>& postfix_queue);

  void ValidateTokenStack(std::queue<std::shared_ptr<Token>>& postfix_queue,
                          std::stack<std::shared_ptr<Token>>& token_stack,
                          std::unordered_set<std::string>& variables,
                          std::unordered_set<int>& constants);

  void ValidateTokenStackSize(std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateWordOrIntegerToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void IsTopStackNotWordOrIntegerToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateCondExprMathematicalOperatorToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateCondExprNotToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateCondExprAndOrOrToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateCondExprComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& token_stack,
      std::shared_ptr<Token> const& token);

  void ValidateCondExprFinalTokenStackState(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void IsTopStackNotComparisonOperatorToken(
      std::stack<std::shared_ptr<Token>>& token_stack);

  void ValidateStmtLst();

  void ValidateCondExprAndOrOrTokenPosition();

  void ValidateBalanceParentheses(int& paren_count);

  void HandleOperatorsStackAndPostfixQueue(
      std::stack<std::shared_ptr<Token>>& operator_stack,
      std::queue<std::shared_ptr<Token>>& postfix_queue);

  void TransferOperatorsToPostfixQueue(
      std::stack<std::shared_ptr<Token>>& operator_stack,
      std::queue<std::shared_ptr<Token>>& postfix_queue);

  bool IsAssignStmt();

  std::shared_ptr<StmtNode> ParseAssignStmt();

  std::shared_ptr<StmtNode> ParseNonAssignStmt();
};
