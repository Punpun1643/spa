#include "SpParser.h"

#include "../../shared/parser/exceptions/EmptyParenthesesException.h"
#include "../../shared/parser/exceptions/UnmatchedParenthesesException.h"
#include "../constant/SpParserConstant.h"
#include "../exceptions/EmptyStmtLstException.h"
#include "../exceptions/InvalidAssignException.h"
#include "../exceptions/InvalidCallException.h"
#include "../exceptions/InvalidCondExprException.h"
#include "../exceptions/InvalidPrintException.h"
#include "../exceptions/InvalidProcedureException.h"
#include "../exceptions/InvalidReadException.h"
#include "../exceptions/InvalidRelExprException.h"
#include "../exceptions/InvalidStmtLstException.h"
#include "../exceptions/exception_message/ExceptionMessage.h"

SpParser::SpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens) {}

std::shared_ptr<ProgramNode> SpParser::ParseProgram() {
  std::vector<std::shared_ptr<ProcedureNode>> procedures;

  while (!IsCurrTokenType(TokenType::EOF_TOKEN)) {
    AParser::CheckAndAdvanceCurrToken<InvalidProcedureException>(
        SpParserConstant::PROCEDURE_KEYWORD);
    procedures.push_back(ParseProcedure());
  }

  return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr<ProcedureNode> SpParser::ParseProcedure() {
  if (!IsCurrTokenType(TokenType::WORD_TOKEN)) {
    throw InvalidProcedureException();
  }

  std::string procedure_name = GetCurrTokenValue();
  NextToken();

  AParser::CheckAndAdvanceCurrToken<InvalidProcedureException>(
      SpParserConstant::START_PROCEDURE);

  std::shared_ptr<StmtLstNode> stmt_lst = ParseStmtLst();

  AParser::CheckAndAdvanceCurrToken<InvalidProcedureException>(
      SpParserConstant::END_PROCEDURE);
  return std::make_shared<ProcedureNode>(procedure_name, stmt_lst);
}

std::shared_ptr<PrintNode> SpParser::ParsePrint() {
  std::string var_name =
      AParser::ParseAndValidateVarName<InvalidPrintException>();
  return std::make_shared<PrintNode>(curr_stmt_index++, StmtType::PRINT_STMT,
                                     var_name);
}

std::shared_ptr<ReadNode> SpParser::ParseRead() {
  std::string var_name =
      AParser::ParseAndValidateVarName<InvalidReadException>();
  return std::make_shared<ReadNode>(curr_stmt_index++, StmtType::READ_STMT,
                                    var_name);
}

std::shared_ptr<CallNode> SpParser::ParseCall() {
  std::string var_name =
      AParser::ParseAndValidateVarName<InvalidCallException>();
  return std::make_shared<CallNode>(curr_stmt_index++, StmtType::CALL_STMT,
                                    var_name);
}

std::shared_ptr<IfNode> SpParser::ParseIf() {
  int if_stmt_index = curr_stmt_index++;

  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_COND_EXPR,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  std::shared_ptr<CondExprNode> cond_expr = ParseCondExpr();

  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_COND_EXPR,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  AssertCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                              SpParserConstant::THEN_KEYWORD,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_THEN_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  std::shared_ptr<StmtLstNode> then_stmt_lst = ParseStmtLst();

  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_THEN_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  AssertCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                              SpParserConstant::ELSE_KEYWORD,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_ELSE_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  std::shared_ptr<StmtLstNode> else_stmt_lst = ParseStmtLst();

  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_ELSE_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  return std::make_shared<IfNode>(if_stmt_index, StmtType::IF_STMT, cond_expr,
                                  then_stmt_lst, else_stmt_lst);
}

std::shared_ptr<WhileNode> SpParser::ParseWhile() {
  int while_stmt_index = curr_stmt_index++;

  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::START_COND_EXPR,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  std::shared_ptr<CondExprNode> condExpr = ParseCondExpr();

  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::END_COND_EXPR,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  NextToken();
  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::START_WHILE_STMTLST,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  NextToken();
  std::shared_ptr<StmtLstNode> stmt_lst = ParseStmtLst();

  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::END_WHILE_STMTLST,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  NextToken();
  return std::make_shared<WhileNode>(while_stmt_index, StmtType::WHILE_STMT,
                                     condExpr, stmt_lst);
}

void SpParser::HandleWordOrIntegerToken(
    std::unordered_set<std::string>& variables,
    std::unordered_set<int>& constants) {
  if (IsCurrTokenType(TokenType::WORD_TOKEN)) {
    variables.insert(GetCurrTokenValue());
  } else if (IsCurrTokenType(TokenType::INTEGER_TOKEN)) {
    constants.insert(std::stoi(GetCurrTokenValue()));
  }
}

void SpParser::HandleOperatorToken(
    std::stack<std::shared_ptr<std::string>>& operator_stack,
    std::queue<std::shared_ptr<std::string>>& postfix_queue) {
  while (!operator_stack.empty() &&
         AParser::IsGreaterOrEqualPrecedence(operator_stack.top()->c_str(),
                                             GetCurrTokenValue())) {
    postfix_queue.push(operator_stack.top());
    operator_stack.pop();
  }
  operator_stack.push(std::make_shared<std::string>(GetCurrTokenValue()));
}

void SpParser::HandleLeftParenthesisToken(
    std::stack<std::shared_ptr<std::string>>& operator_stack,
    int& paren_count) {
  if (AParser::IsPeekTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
    throw EmptyParenthesesException();
  }
  ++paren_count;
  operator_stack.push(std::make_shared<std::string>(GetCurrTokenValue()));
}

void SpParser::HandleCondExprLeftParenthesisToken(
    std::shared_ptr<Token> const& curr_token,
    std::stack<std::shared_ptr<Token>>& operator_stack, int& paren_count) {
  if (IsPeekTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
    throw EmptyParenthesesException();
  }
  ++paren_count;
  operator_stack.push(curr_token);
}

void SpParser::CondExprHandleRightParenthesisToken(
    std::stack<std::shared_ptr<Token>>& operator_stack,
    std::queue<std::shared_ptr<Token>>& postfix_queue) {
  bool is_parse_rel_expr = false;

  while (!(IsLeftParenthesisToken(operator_stack.top()))) {
    if (IsComparisonOperator(operator_stack.top()->GetTokenVal()) ||
        operator_stack.top()->GetTokenVal() == SpParserConstant::NOT) {
      is_parse_rel_expr = true;  // closing rel_expr
    }
    postfix_queue.push(operator_stack.top());
    operator_stack.pop();
  }

  if (!operator_stack.empty()) {  // pop the corresponding left parenthesis
    operator_stack.pop();
  } else {
    throw InvalidRelExprException();
  }

  // check valid relExpr
  if (is_parse_rel_expr && !operator_stack.empty() &&
      !(IsAndOrOrToken(PeekToken())) &&
      IsLeftParenthesisToken(operator_stack.top())) {
    throw InvalidRelExprException();
  }
}

void SpParser::AssignHandleRightParenthesisToken(
    std::stack<std::shared_ptr<std::string>>& operator_stack,
    std::queue<std::shared_ptr<std::string>>& postfix_queue, int& paren_count) {
  if (paren_count <= 0) {
    throw UnmatchedParenthesesException();
  }
  while (operator_stack.top()->compare(AParserConstant::LEFT_PARENTHESIS) !=
         0) {
    postfix_queue.push(operator_stack.top());
    operator_stack.pop();
  }
  --paren_count;
  operator_stack.pop();
}

void SpParser::TrackOperatorAndOperand(
    std::vector<int>& const_appearances,
    std::vector<std::string>& var_appearances) {
  if (AParser::IsWordToken(GetCurrToken())) {
    var_appearances.push_back(GetCurrTokenValue());
  } else {
    const_appearances.push_back(std::stoi(GetCurrTokenValue()));
  }
}

void SpParser::ValidateCondExprAndOrOrTokenPosition() {
  if (!IsRightParenthesisToken(PeekBackToken()) ||
      !IsLeftParenthesisToken(PeekToken())) {
    throw InvalidCondExprException();
  }
}

void SpParser::HandleOperatorsStackAndPostfixQueue(
    std::stack<std::shared_ptr<Token>>& operator_stack,
    std::queue<std::shared_ptr<Token>>& postfix_queue) {
  while (!operator_stack.empty() &&
         AParser::IsGreaterOrEqualPrecedence(
             operator_stack.top()->GetTokenVal(), GetCurrTokenValue())) {
    postfix_queue.push(operator_stack.top());
    operator_stack.pop();
  }
  operator_stack.push(GetCurrToken());
}

void SpParser::TransferOperatorsToPostfixQueue(
    std::stack<std::shared_ptr<Token>>& operator_stack,
    std::queue<std::shared_ptr<Token>>& postfix_queue) {
  while (!operator_stack.empty()) {
    postfix_queue.push(operator_stack.top());
    operator_stack.pop();
  }
}

void SpParser::ValidateBalanceParentheses(int& paren_count) {
  if (paren_count != 0) {
    throw UnmatchedParenthesesException();
  }
}

void SpParser::BuildCondExprPostFix(
    std::queue<std::shared_ptr<Token>>& postfix_queue) {
  std::stack<std::shared_ptr<Token>> operator_stack;
  int paren_count = 0;

  while (!IsCurrTokenType(TokenType::EOF_TOKEN)) {
    std::shared_ptr<Token> curr_token = GetCurrToken();
    if (AParser::IsWordOrIntegerToken(curr_token)) {
      postfix_queue.push(GetCurrToken());
    } else if (IsLeftParenthesisToken(curr_token)) {
      HandleCondExprLeftParenthesisToken(curr_token, operator_stack,
                                         paren_count);
    } else if (IsRightParenthesisToken(GetCurrToken())) {
      --paren_count;
      CondExprHandleRightParenthesisToken(operator_stack, postfix_queue);
      if (paren_count == 0)
        break;
    } else if (IsOperator(GetCurrTokenValue())) {
      if (IsAndOrOrToken(GetCurrToken())) {
        ValidateCondExprAndOrOrTokenPosition();
      }
      HandleOperatorsStackAndPostfixQueue(operator_stack, postfix_queue);
    } else {
      throw InvalidCondExprException();
    }

    NextToken();
  }

  ValidateBalanceParentheses(paren_count);
  TransferOperatorsToPostfixQueue(operator_stack, postfix_queue);
}

void SpParser::HandleCondExprWordToken(
    std::shared_ptr<Token> const& curr_token,
    std::unordered_set<std::string>& variables,
    std::stack<std::shared_ptr<Token>>& token_stack) {
  variables.insert(curr_token->GetTokenVal());
  token_stack.push(curr_token);
}

void SpParser::HandleCondExprIntegerToken(
    std::shared_ptr<Token> const& curr_token,
    std::unordered_set<int>& constants,
    std::stack<std::shared_ptr<Token>>& token_stack) {
  constants.insert(std::stoi(curr_token->GetTokenVal()));
  token_stack.push(curr_token);
}

void SpParser::ValidateTokenStackSize(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  if (token_stack.size() < AParserConstant::MINIMUM_OPERATOR_SIZE) {
    throw InvalidCondExprException();
  }
}

void SpParser::ValidateWordOrIntegerToken(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  if (AParser::IsWordOrIntegerToken(token_stack.top())) {
    token_stack.pop();
  } else {
    throw InvalidCondExprException();
  }
}

void SpParser::IsTopStackNotWordOrIntegerToken(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  if (!AParser::IsWordOrIntegerToken(token_stack.top())) {
    throw InvalidCondExprException();
  }
}

void SpParser::ValidateComparisonOperatorToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (IsComparisonOperator(tokenStack.top()->GetTokenVal())) {
    return;
  } else {
    throw InvalidCondExprException();
  }
}

void SpParser::IsTopStackNotComparisonOperatorToken(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  if (!IsComparisonOperator(token_stack.top()->GetTokenVal())) {
    throw InvalidCondExprException();
  }
}

void SpParser::ValidateCondExprMathematicalOperatorToken(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  ValidateTokenStackSize(token_stack);
  ValidateWordOrIntegerToken(token_stack);
  IsTopStackNotWordOrIntegerToken(token_stack);
}

void SpParser::ValidateCondExprNotToken(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  IsTopStackNotComparisonOperatorToken(token_stack);
}

void SpParser::ValidateCondExprAndOrOrToken(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  ValidateTokenStackSize(token_stack);
  ValidateComparisonOperatorToken(token_stack);
  ValidateComparisonOperatorToken(token_stack);
}

void SpParser::ValidateCondExprComparisonOperatorToken(
    std::stack<std::shared_ptr<Token>>& token_stack,
    std::shared_ptr<Token> const& token) {
  ValidateTokenStackSize(token_stack);
  ValidateWordOrIntegerToken(token_stack);
  ValidateWordOrIntegerToken(token_stack);
  token_stack.push(token);
}

void SpParser::ValidateCondExprFinalTokenStackState(
    std::stack<std::shared_ptr<Token>>& token_stack) {
  if (token_stack.empty() || AParser::IsWordOrIntegerToken(token_stack.top())) {
    throw InvalidCondExprException();
  }
}

void SpParser::HandleCondExprOperatorToken(
    std::shared_ptr<Token> const& token,
    std::stack<std::shared_ptr<Token>>& token_stack) {
  if (AParser::IsMathematicalOperator(token->GetTokenVal())) {
    ValidateCondExprMathematicalOperatorToken(token_stack);
  } else if (IsNotToken(token)) {
    ValidateCondExprNotToken(token_stack);
  } else if (IsAndOrOrToken(token)) {
    ValidateCondExprAndOrOrToken(token_stack);
  } else if (IsComparisonOperator(token->GetTokenVal())) {
    ValidateCondExprComparisonOperatorToken(token_stack, token);
  } else {
    throw InvalidCondExprException();
  }
}

void SpParser::ValidateTokenStack(
    std::queue<std::shared_ptr<Token>>& postfix_queue,
    std::stack<std::shared_ptr<Token>>& token_stack,
    std::unordered_set<std::string>& variables,
    std::unordered_set<int>& constants) {
  // check postFix
  while (!postfix_queue.empty()) {
    std::shared_ptr<Token> curr_token = postfix_queue.front();
    postfix_queue.pop();

    if (AParser::IsWordToken(curr_token)) {
      HandleCondExprWordToken(curr_token, variables, token_stack);
    } else if (AParser::IsIntegerToken(curr_token)) {
      HandleCondExprIntegerToken(curr_token, constants, token_stack);
    } else if (IsOperator(curr_token->GetTokenVal())) {
      HandleCondExprOperatorToken(curr_token, token_stack);
    } else {
      throw InvalidCondExprException();
    }
  }
  ValidateCondExprFinalTokenStackState(token_stack);
}

std::shared_ptr<CondExprNode> SpParser::ParseCondExpr() {
  std::queue<std::shared_ptr<Token>> postfix_queue;
  std::unordered_set<std::string> variables;
  std::unordered_set<int> constants;
  std::stack<std::shared_ptr<Token>> token_stack;

  BuildCondExprPostFix(postfix_queue);
  ValidateTokenStack(postfix_queue, token_stack, variables, constants);

  return std::make_shared<CondExprNode>(variables, constants);
}

std::shared_ptr<AssignNode> SpParser::ParseAssign(std::string const& var_name) {
  std::unordered_set<std::string> variables = std::unordered_set<std::string>();
  std::unordered_set<int> constants = std::unordered_set<int>();

  std::vector<std::shared_ptr<Token>> infix_tokens;

  while (!IsCurrTokenValue(AParserConstant::STMT_TERMINATOR)) {
    if (AParser::IsWordOrIntegerToken(GetCurrToken())) {
      HandleWordOrIntegerToken(variables, constants);
    }
    infix_tokens.push_back(GetCurrToken());
    NextToken();
  }

  std::queue<std::shared_ptr<std::string>> postfix_queue =
      AParser::ConvertInfixToPostfix(infix_tokens);
  NextToken();

  try {
    std::shared_ptr<TreeNode> expr_tree_root =
        AParser::BuildExprTreeAndValidate(postfix_queue);
    return std::make_shared<AssignNode>(curr_stmt_index++,
                                        StmtType::ASSIGN_STMT, variables,
                                        constants, var_name, expr_tree_root);
  } catch (std::invalid_argument& e) {
    throw InvalidAssignException();
  }
}

bool SpParser::IsOperator(std::string const& token_val) {
  return token_val == AParserConstant::PLUS ||
         token_val == AParserConstant::MINUS ||
         token_val == AParserConstant::MULTIPLY ||
         token_val == AParserConstant::DIVIDE ||
         token_val == AParserConstant::MODULO ||
         token_val == SpParserConstant::EQUAL ||
         token_val == SpParserConstant::NOT_EQUAL ||
         token_val == SpParserConstant::LESS_THAN ||
         token_val == SpParserConstant::LESS_THAN_EQUAL ||
         token_val == SpParserConstant::GREATER_THAN ||
         token_val == SpParserConstant::GREATER_THAN_EQUAL ||
         token_val == SpParserConstant::AND ||
         token_val == SpParserConstant::OR ||
         token_val == SpParserConstant::NOT;
}

bool SpParser::IsComparisonOperator(std::string const& token_val) {
  return token_val == SpParserConstant::EQUAL ||
         token_val == SpParserConstant::NOT_EQUAL ||
         token_val == SpParserConstant::LESS_THAN ||
         token_val == SpParserConstant::LESS_THAN_EQUAL ||
         token_val == SpParserConstant::GREATER_THAN ||
         token_val == SpParserConstant::GREATER_THAN_EQUAL;
}

bool SpParser::IsLogicalOperator(std::string const& token_val) {
  return token_val == SpParserConstant::AND ||
         token_val == SpParserConstant::OR ||
         token_val == SpParserConstant::NOT;
}

bool SpParser::IsMathematicalOperator(std::string const& token_val) {
  return token_val == AParserConstant::PLUS ||
         token_val == AParserConstant::MINUS ||
         token_val == AParserConstant::MULTIPLY ||
         token_val == AParserConstant::DIVIDE ||
         token_val == AParserConstant::MODULO;
}

bool SpParser::IsPossibleRelFactor(std::shared_ptr<Token> token) {
  return AParser::IsWordOrIntegerToken(token) ||
         AParser::IsTokenValue(token, AParserConstant::LEFT_PARENTHESIS) ||
         AParser::IsTokenValue(token, SpParserConstant::NOT);
}

bool SpParser::IsAndOrOrToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, SpParserConstant::AND) ||
         AParser::IsTokenValue(token, SpParserConstant::OR);
}

bool SpParser::IsLeftParenthesisToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, AParserConstant::LEFT_PARENTHESIS);
}

bool SpParser::IsRightParenthesisToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, AParserConstant::RIGHT_PARENTHESIS);
}

bool SpParser::IsNotToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, SpParserConstant::NOT);
}

// helper function to calculate precedence of an operator
int SpParser::Precedence(std::string const& op) {
  if (op.compare(AParserConstant::MULTIPLY) == 0 ||
      op.compare(AParserConstant::DIVIDE) == 0 ||
      op.compare(AParserConstant::MODULO) == 0) {
    return 4;
  } else if (op.compare(AParserConstant::PLUS) == 0 ||
             op.compare(AParserConstant::MINUS) == 0) {
    return 3;
  } else if (IsComparisonOperator(op)) {
    return 2;
  } else if (IsLogicalOperator(op)) {
    return 1;
  } else {
    return -1;
  }
}

void SpParser::ValidateStmtLst() {
  if (GetCurrTokenValue() == SpParserConstant::END_PROCEDURE) {
    throw EmptyStmtLstException();
  }
}

bool SpParser::IsAssignStmt() {
  return IsCurrTokenType(TokenType::WORD_TOKEN) &&
         IsPeekTokenValue(SpParserConstant::ASSIGN_SYMBOL);
}

std::shared_ptr<StmtNode> SpParser::ParseAssignStmt() {
  std::shared_ptr<Token> curr_token = GetCurrToken();
  NextToken();  // move to assign symbol
  NextToken();
  return ParseAssign(curr_token->GetTokenVal());
}

std::shared_ptr<StmtNode> SpParser::ParseNonAssignStmt() {
  std::shared_ptr<StmtNode> stmt_node;
  if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                              SpParserConstant::PRINT_KEYWORD)) {
    NextToken();
    stmt_node = ParsePrint();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::READ_KEYWORD)) {
    NextToken();
    stmt_node = ParseRead();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::CALL_KEYWORD)) {
    NextToken();
    stmt_node = ParseCall();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::WHILE_KEYWORD)) {
    NextToken();
    stmt_node = ParseWhile();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::IF_KEYWORD)) {
    NextToken();
    stmt_node = ParseIf();
  } else {
    throw InvalidStmtLstException();
  }

  return stmt_node;
}

std::shared_ptr<StmtLstNode> SpParser::ParseStmtLst() {
  std::vector<std::shared_ptr<StmtNode>> stmts;

  ValidateStmtLst();

  while (!IsCurrTokenValue(SpParserConstant::END_PROCEDURE)) {
    if (IsAssignStmt()) {
      stmts.push_back(ParseAssignStmt());
    } else {
      stmts.push_back(ParseNonAssignStmt());
    }
  }
  return std::make_shared<StmtLstNode>(stmts);
}

void SpParser::Parse() {
  source_program_node = ParseProgram();
}

std::shared_ptr<ProgramNode> SpParser::GetSourceProgramNode() {
  return source_program_node;
}
