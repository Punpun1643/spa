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

std::shared_ptr<ProgramNode> SpParser::parseProgram() {
  std::vector<std::shared_ptr<ProcedureNode>> procedures;

  while (!IsCurrTokenType(TokenType::EOF_TOKEN)) {
    AParser::CheckAndAdvanceCurrToken<InvalidProcedureException>(
        SpParserConstant::PROCEDURE_KEYWORD);
    procedures.push_back(parseProcedure());
  }

  return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr<ProcedureNode> SpParser::parseProcedure() {
  if (!IsCurrTokenType(TokenType::WORD_TOKEN)) {
    throw InvalidProcedureException();
  }

  std::string procedureName = GetCurrTokenValue();
  NextToken();

  AParser::CheckAndAdvanceCurrToken<InvalidProcedureException>(
      SpParserConstant::START_PROCEDURE);

  std::shared_ptr<StmtLstNode> stmtLst = parseStmtLst();

  AParser::CheckAndAdvanceCurrToken<InvalidProcedureException>(
      SpParserConstant::END_PROCEDURE);
  return std::make_shared<ProcedureNode>(procedureName, stmtLst);
}

std::shared_ptr<PrintNode> SpParser::parsePrint() {
  std::string varName =
      AParser::ParseAndValidateVarName<InvalidPrintException>();
  return std::make_shared<PrintNode>(currStmtIndex++, StmtType::PRINT_STMT,
                                     varName);
}

std::shared_ptr<ReadNode> SpParser::parseRead() {
  std::string varName =
      AParser::ParseAndValidateVarName<InvalidReadException>();
  return std::make_shared<ReadNode>(currStmtIndex++, StmtType::READ_STMT,
                                    varName);
}

std::shared_ptr<CallNode> SpParser::parseCall() {
  std::string varName =
      AParser::ParseAndValidateVarName<InvalidCallException>();
  return std::make_shared<CallNode>(currStmtIndex++, StmtType::CALL_STMT,
                                    varName);
}

std::shared_ptr<IfNode> SpParser::parseIf() {
  int ifStmtIndex = currStmtIndex++;

  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_COND_EXPR,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  std::shared_ptr<CondExprNode> condExpr = parseCondExpr();

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
  std::shared_ptr<StmtLstNode> thenStmtLst = parseStmtLst();

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
  std::shared_ptr<StmtLstNode> elseStmtLst = parseStmtLst();

  AssertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_ELSE_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  NextToken();
  return std::make_shared<IfNode>(ifStmtIndex, StmtType::IF_STMT, condExpr,
                                  thenStmtLst, elseStmtLst);
}

std::shared_ptr<WhileNode> SpParser::parseWhile() {
  int whileStmtIndex = currStmtIndex++;

  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::START_COND_EXPR,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  std::shared_ptr<CondExprNode> condExpr = parseCondExpr();

  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::END_COND_EXPR,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  NextToken();
  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::START_WHILE_STMTLST,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  NextToken();
  std::shared_ptr<StmtLstNode> stmtLst = parseStmtLst();

  AssertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::END_WHILE_STMTLST,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  NextToken();
  return std::make_shared<WhileNode>(whileStmtIndex, StmtType::WHILE_STMT,
                                     condExpr, stmtLst);
}

void SpParser::handleWordOrIntegerToken(
    std::unordered_set<std::string>& variables,
    std::unordered_set<int>& constants) {
  if (IsCurrTokenType(TokenType::WORD_TOKEN)) {
    variables.insert(GetCurrTokenValue());
  } else if (IsCurrTokenType(TokenType::INTEGER_TOKEN)) {
    constants.insert(std::stoi(GetCurrTokenValue()));
  }
}

void SpParser::handleOperatorToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack,
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  while (!operatorStack.empty() &&
         AParser::IsGreaterOrEqualPrecedence(operatorStack.top()->c_str(),
                                             GetCurrTokenValue())) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
  operatorStack.push(std::make_shared<std::string>(GetCurrTokenValue()));
}

void SpParser::handleLeftParenthesisToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount) {
  if (AParser::IsPeekTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
    throw EmptyParenthesesException();
  }
  ++parenCount;
  operatorStack.push(std::make_shared<std::string>(GetCurrTokenValue()));
}

void SpParser::handleCondExprLeftParenthesisToken(
    std::shared_ptr<Token> const& currToken,
    std::stack<std::shared_ptr<Token>>& operatorStack, int& parenCount) {
  if (IsPeekTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
    throw EmptyParenthesesException();
  }
  ++parenCount;
  operatorStack.push(currToken);
}

void SpParser::condExprHandleRightParenthesisToken(
    std::stack<std::shared_ptr<Token>>& operatorStack,
    std::queue<std::shared_ptr<Token>>& postFixQueue) {
  bool isParseRelExpr = false;

  while (!(isLeftParenthesisToken(operatorStack.top()))) {
    if (isComparisonOperator(operatorStack.top()->getTokenVal())) {
      isParseRelExpr = true;  // closing relExpr
    }
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }

  if (!operatorStack.empty()) {
    operatorStack.pop();
  } else {
    throw InvalidRelExprException();
  }

  // check valid relExpr
  if (isParseRelExpr && !operatorStack.empty() &&
      !(isAndOrOrToken(PeekToken())) &&
      isLeftParenthesisToken(operatorStack.top())) {
    throw InvalidRelExprException();
  }
}

void SpParser::assignHandleRightParenthesisToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack,
    std::queue<std::shared_ptr<std::string>>& postFixQueue, int& parenCount) {
  if (parenCount <= 0) {
    throw UnmatchedParenthesesException();
  }
  while (operatorStack.top()->compare(AParserConstant::LEFT_PARENTHESIS) != 0) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
  --parenCount;
  operatorStack.pop();
}

void SpParser::trackOperatorAndOperand(
    std::vector<int>& constAppearances,
    std::vector<std::string>& varAppearances) {
  if (AParser::IsWordToken(GetCurrToken())) {
    varAppearances.push_back(GetCurrTokenValue());
  } else {
    constAppearances.push_back(std::stoi(GetCurrTokenValue()));
  }
}

void SpParser::ValidateCondExprAndOrOrTokenPosition() {
  if (!isRightParenthesisToken(PeekBackToken()) ||
      !isLeftParenthesisToken(PeekToken())) {
    throw InvalidCondExprException();
  }
}

void SpParser::TransferOperatorsToPostfixQueue(
    std::stack<std::shared_ptr<Token>>& operatorStack,
    std::queue<std::shared_ptr<Token>>& postFixQueue) {
  while (!operatorStack.empty() &&
         AParser::IsGreaterOrEqualPrecedence(operatorStack.top()->getTokenVal(),
                                             GetCurrTokenValue())) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
  operatorStack.push(GetCurrToken());
}

void SpParser::buildCondExprPostFix(
    std::queue<std::shared_ptr<Token>>& postFixQueue) {
  std::stack<std::shared_ptr<Token>> operatorStack;
  int parenCount = 0;

  while (!IsCurrTokenType(TokenType::EOF_TOKEN)) {
    std::shared_ptr<Token> currToken = GetCurrToken();
    if (AParser::IsWordOrIntegerToken(currToken)) {
      postFixQueue.push(GetCurrToken());
    } else if (isLeftParenthesisToken(currToken)) {
      handleCondExprLeftParenthesisToken(currToken, operatorStack, parenCount);
    } else if (isRightParenthesisToken(GetCurrToken())) {
      --parenCount;
      condExprHandleRightParenthesisToken(operatorStack, postFixQueue);
      if (parenCount == 0) break;
    } else if (isOperator(GetCurrTokenValue())) {
      if (isAndOrOrToken(GetCurrToken())) {
        ValidateCondExprAndOrOrTokenPosition();
      }
      TransferOperatorsToPostfixQueue(operatorStack, postFixQueue);
    } else {
      throw InvalidCondExprException();
    }

    NextToken();
  }

  if (parenCount != 0) {
    throw UnmatchedParenthesesException();
  }

  while (!operatorStack.empty()) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
}

void SpParser::handleCondExprWordToken(
    std::shared_ptr<Token> const& currToken,
    std::unordered_set<std::string>& variables,
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  variables.insert(currToken->getTokenVal());
  tokenStack.push(currToken);
}

void SpParser::handleCondExprIntegerToken(
    std::shared_ptr<Token> const& currToken, std::unordered_set<int>& constants,
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  constants.insert(std::stoi(currToken->getTokenVal()));
  tokenStack.push(currToken);
}

void SpParser::validateTokenStackSize(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (tokenStack.size() < AParserConstant::MINIMUM_OPERATOR_SIZE) {
    throw InvalidCondExprException();
  }
}

void SpParser::validateWordOrIntegerToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (AParser::IsWordOrIntegerToken(tokenStack.top())) {
    tokenStack.pop();
  } else {
    throw InvalidCondExprException();
  }
}

void SpParser::isTopStackNotWordOrIntegerToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (!AParser::IsWordOrIntegerToken(tokenStack.top())) {
    throw InvalidCondExprException();
  }
}

void SpParser::validateComparisonOperatorToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (isComparisonOperator(tokenStack.top()->getTokenVal())) {
    return;
  } else {
    throw InvalidCondExprException();
  }
}

void SpParser::isTopStackNotComparisonOperatorToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (!isComparisonOperator(tokenStack.top()->getTokenVal())) {
    throw InvalidCondExprException();
  }
}

void SpParser::ValidateCondExprMathematicalOperatorToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  validateTokenStackSize(tokenStack);
  validateWordOrIntegerToken(tokenStack);
  isTopStackNotWordOrIntegerToken(tokenStack);
}

void SpParser::ValidateCondExprNotToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  isTopStackNotComparisonOperatorToken(tokenStack);
}

void SpParser::ValidateCondExprAndOrOrToken(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  validateTokenStackSize(tokenStack);
  validateComparisonOperatorToken(tokenStack);
  validateComparisonOperatorToken(tokenStack);
}

void SpParser::ValidateCondExprComparisonOperatorToken(
    std::stack<std::shared_ptr<Token>>& tokenStack,
    std::shared_ptr<Token> const& token) {
  validateTokenStackSize(tokenStack);
  validateWordOrIntegerToken(tokenStack);
  validateWordOrIntegerToken(tokenStack);
  tokenStack.push(token);
}

void SpParser::ValidateCondExprFinalTokenStackState(
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (tokenStack.empty() || AParser::IsWordOrIntegerToken(tokenStack.top())) {
    throw InvalidCondExprException();
  }
}

void SpParser::HandleCondExprOperatorToken(
    std::shared_ptr<Token> const& token,
    std::stack<std::shared_ptr<Token>>& tokenStack) {
  if (AParser::IsMathematicalOperator(token->getTokenVal())) {
    ValidateCondExprMathematicalOperatorToken(tokenStack);
  } else if (isNotToken(token)) {
    ValidateCondExprNotToken(tokenStack);
  } else if (isAndOrOrToken(token)) {
    ValidateCondExprAndOrOrToken(tokenStack);
  } else if (isComparisonOperator(token->getTokenVal())) {
    ValidateCondExprComparisonOperatorToken(tokenStack, token);
  } else {
    throw InvalidCondExprException();
  }
}

void SpParser::validateTokenStack(
    std::queue<std::shared_ptr<Token>>& postFixQueue,
    std::stack<std::shared_ptr<Token>>& tokenStack,
    std::unordered_set<std::string>& variables,
    std::unordered_set<int>& constants) {
  // check postFix
  while (!postFixQueue.empty()) {
    std::shared_ptr<Token> currToken = postFixQueue.front();
    postFixQueue.pop();

    if (AParser::IsWordToken(currToken)) {
      handleCondExprWordToken(currToken, variables, tokenStack);
    } else if (AParser::IsIntegerToken(currToken)) {
      handleCondExprIntegerToken(currToken, constants, tokenStack);
    } else if (isOperator(currToken->getTokenVal())) {
      HandleCondExprOperatorToken(currToken, tokenStack);
    } else {
      throw InvalidCondExprException();
    }
  }
  ValidateCondExprFinalTokenStackState(tokenStack);
}

std::shared_ptr<CondExprNode> SpParser::parseCondExpr() {
  std::queue<std::shared_ptr<Token>> postFixQueue;
  std::unordered_set<std::string> variables;
  std::unordered_set<int> constants;
  std::stack<std::shared_ptr<Token>> tokenStack;

  buildCondExprPostFix(postFixQueue);
  validateTokenStack(postFixQueue, tokenStack, variables, constants);

  return std::make_shared<CondExprNode>(variables, constants);
}

std::shared_ptr<AssignNode> SpParser::parseAssign(std::string const& varName) {
  std::unordered_set<std::string> variables = std::unordered_set<std::string>();
  std::unordered_set<int> constants = std::unordered_set<int>();

  std::vector<std::shared_ptr<Token>> infixTokens;

  while (!IsCurrTokenValue(AParserConstant::STMT_TERMINATOR)) {
    if (AParser::IsWordOrIntegerToken(GetCurrToken())) {
      handleWordOrIntegerToken(variables, constants);
    }
    infixTokens.push_back(GetCurrToken());
    NextToken();
  }

  std::queue<std::shared_ptr<std::string>> postFixQueue =
      AParser::ConvertInfixToPostfix(infixTokens);
  NextToken();

  try {
    std::shared_ptr<TreeNode> exprTreeRoot =
        AParser::BuildExprTreeAndValidate(postFixQueue);
    return std::make_shared<AssignNode>(currStmtIndex++, StmtType::ASSIGN_STMT,
                                        variables, constants, varName,
                                        exprTreeRoot);
  } catch (std::invalid_argument& e) {
    throw InvalidAssignException();
  }
}

bool SpParser::isOperator(std::string const& tokenVal) {
  return tokenVal == AParserConstant::PLUS ||
         tokenVal == AParserConstant::MINUS ||
         tokenVal == AParserConstant::MULTIPLY ||
         tokenVal == AParserConstant::DIVIDE ||
         tokenVal == AParserConstant::MODULO ||
         tokenVal == SpParserConstant::EQUAL ||
         tokenVal == SpParserConstant::NOT_EQUAL ||
         tokenVal == SpParserConstant::LESS_THAN ||
         tokenVal == SpParserConstant::LESS_THAN_EQUAL ||
         tokenVal == SpParserConstant::GREATER_THAN ||
         tokenVal == SpParserConstant::GREATER_THAN_EQUAL ||
         tokenVal == SpParserConstant::AND ||
         tokenVal == SpParserConstant::OR || tokenVal == SpParserConstant::NOT;
}

bool SpParser::isComparisonOperator(std::string const& tokenVal) {
  return tokenVal == SpParserConstant::EQUAL ||
         tokenVal == SpParserConstant::NOT_EQUAL ||
         tokenVal == SpParserConstant::LESS_THAN ||
         tokenVal == SpParserConstant::LESS_THAN_EQUAL ||
         tokenVal == SpParserConstant::GREATER_THAN ||
         tokenVal == SpParserConstant::GREATER_THAN_EQUAL;
}

bool SpParser::isLogicalOperator(std::string const& tokenVal) {
  return tokenVal == SpParserConstant::AND ||
         tokenVal == SpParserConstant::OR || tokenVal == SpParserConstant::NOT;
}

bool SpParser::isMathematicalOperator(std::string const& tokenVal) {
  return tokenVal == AParserConstant::PLUS ||
         tokenVal == AParserConstant::MINUS ||
         tokenVal == AParserConstant::MULTIPLY ||
         tokenVal == AParserConstant::DIVIDE ||
         tokenVal == AParserConstant::MODULO;
}

bool SpParser::isPossibleRelFactor(std::shared_ptr<Token> token) {
  return AParser::IsWordOrIntegerToken(token) ||
         AParser::IsTokenValue(token, AParserConstant::LEFT_PARENTHESIS) ||
         AParser::IsTokenValue(token, SpParserConstant::NOT);
}

bool SpParser::isAndOrOrToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, SpParserConstant::AND) ||
         AParser::IsTokenValue(token, SpParserConstant::OR);
}

bool SpParser::isLeftParenthesisToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, AParserConstant::LEFT_PARENTHESIS);
}

bool SpParser::isRightParenthesisToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenValue(token, AParserConstant::RIGHT_PARENTHESIS);
}

bool SpParser::isNotToken(std::shared_ptr<Token> token) {
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
  } else if (isComparisonOperator(op)) {
    return 2;
  } else if (isLogicalOperator(op)) {
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
  std::shared_ptr<Token> currToken = GetCurrToken();
  NextToken();  // move to assign symbol
  NextToken();
  return parseAssign(currToken->getTokenVal());
}

std::shared_ptr<StmtNode> SpParser::ParseNonAssignStmt() {
  std::shared_ptr<StmtNode> stmtNode;
  if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                              SpParserConstant::PRINT_KEYWORD)) {
    NextToken();
    stmtNode = parsePrint();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::READ_KEYWORD)) {
    NextToken();
    stmtNode = parseRead();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::CALL_KEYWORD)) {
    NextToken();
    stmtNode = parseCall();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::WHILE_KEYWORD)) {
    NextToken();
    stmtNode = parseWhile();
  } else if (IsCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                     SpParserConstant::IF_KEYWORD)) {
    NextToken();
    stmtNode = parseIf();
  } else {
    throw InvalidStmtLstException();
  }

  return stmtNode;
}

std::shared_ptr<StmtLstNode> SpParser::parseStmtLst() {
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

void SpParser::parse() { sourceProgramNode = parseProgram(); }

std::shared_ptr<ProgramNode> SpParser::getSourceProgramNode() {
  return sourceProgramNode;
}
