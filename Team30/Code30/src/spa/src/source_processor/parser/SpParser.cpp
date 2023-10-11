#include "SpParser.h"

#include "../exceptions/InvalidCallException.h"
#include "../exceptions/InvalidPrintException.h"
#include "../exceptions/InvalidProcedureException.h"
#include "../exceptions/InvalidReadException.h"
#include "../exceptions/InvalidStmtLstException.h"
#include "../exceptions/InvalidAssignException.h"
#include "../exceptions/EmptyParenthesesException.h"
#include "../exceptions/UnmatchedParenthesesException.h"
#include "../exceptions/EmptyStmtLstException.h"
#include "../exceptions/InvalidRelExprException.h"
#include "../exceptions/InvalidCondExprException.h"
#include "../exceptions/exception_message/ExceptionMessage.h"
#include "../constant/SpParserConstant.h"

SpParser::SpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens) {}

std::shared_ptr<ProgramNode> SpParser::parseProgram() {
  std::vector<std::shared_ptr<ProcedureNode>> procedures;

  while (!isCurrTokenType(TokenType::EOF_TOKEN)) {
    if (!isCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                 SpParserConstant::PROCEDURE_KEYWORD)) {
      throw InvalidProcedureException();
    }

    nextToken();
    procedures.push_back(parseProcedure());
  }

  return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr<ProcedureNode> SpParser::parseProcedure() {
  if (!isCurrTokenType(TokenType::WORD_TOKEN)) {
    throw InvalidProcedureException();
  }

  std::string procedureName = getCurrTokenValue();
  nextToken();

  if (!isCurrTokenValue(SpParserConstant::START_PROCEDURE)) {
    throw InvalidProcedureException();
  }

  nextToken();
  std::shared_ptr<StmtLstNode> stmtLst = parseStmtLst();

  if (!isCurrTokenValue(SpParserConstant::END_PROCEDURE)) {
    throw InvalidProcedureException();
  }

  nextToken();
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

  assertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_COND_EXPR,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  std::shared_ptr<CondExprNode> condExpr = parseCondExpr();

  assertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_COND_EXPR,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  assertCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                              SpParserConstant::THEN_KEYWORD,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  assertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_THEN_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  std::shared_ptr<StmtLstNode> thenStmtLst = parseStmtLst();

  assertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_THEN_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  assertCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                              SpParserConstant::ELSE_KEYWORD,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  assertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::START_ELSE_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  std::shared_ptr<StmtLstNode> elseStmtLst = parseStmtLst();

  assertCurrTokenTypeAndValue(TokenType::SPECIAL_CHAR_TOKEN,
                              SpParserConstant::END_ELSE_STMTLST,
                              ExceptionMessage::INVALID_IF_EXCEPTION_MESSAGE);

  nextToken();
  return std::make_shared<IfNode>(ifStmtIndex, StmtType::IF_STMT, condExpr,
                                  thenStmtLst, elseStmtLst);
}

std::shared_ptr<WhileNode> SpParser::parseWhile() {
  int whileStmtIndex = currStmtIndex++;

  assertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::START_COND_EXPR,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  std::shared_ptr<CondExprNode> condExpr = parseCondExpr();

  assertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::END_COND_EXPR,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  nextToken();
  assertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::START_WHILE_STMTLST,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  nextToken();
  std::shared_ptr<StmtLstNode> stmtLst = parseStmtLst();

  assertCurrTokenTypeAndValue(
      TokenType::SPECIAL_CHAR_TOKEN, SpParserConstant::END_WHILE_STMTLST,
      ExceptionMessage::INVALID_WHILE_EXCEPTION_MESSAGE);

  nextToken();
  return std::make_shared<WhileNode>(whileStmtIndex, StmtType::WHILE_STMT,
                                     condExpr, stmtLst);
}

void SpParser::handleWordOrIntegerToken(
    std::queue<std::shared_ptr<std::string>>& postFixQueue,
    std::unordered_set<std::string>& variables,
    std::unordered_set<int>& constants) {
  postFixQueue.push(std::make_shared<std::string>(getCurrTokenValue()));

  if (isCurrTokenType(TokenType::WORD_TOKEN)) {
    variables.insert(getCurrTokenValue());
  } else if (isCurrTokenType(TokenType::INTEGER_TOKEN)) {
    constants.insert(std::stoi(getCurrTokenValue()));
  }
}

void SpParser::handleOperatorToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack,
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  while (!operatorStack.empty() && Precedence(operatorStack.top()->c_str()) >=
                                       Precedence(getCurrTokenValue())) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
  operatorStack.push(std::make_shared<std::string>(getCurrTokenValue()));
}

void SpParser::handleLeftParenthesisToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount) {
  if (AParser::isPeekTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
    throw EmptyParenthesesException();
  }
  ++parenCount;
  operatorStack.push(std::make_shared<std::string>(getCurrTokenValue()));
}

void SpParser::handleCondExprLeftParenthesisToken(
    std::shared_ptr<Token> const& currToken,
    std::stack<std::shared_ptr<Token>>& operatorStack, int& parenCount) {
  if (isPeekTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
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
      !(isAndOrOrToken(peekToken())) &&
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
  while (operatorStack.top()->compare(AParserConstant::LEFT_PARENTHESIS) !=
         0) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
  --parenCount;
  operatorStack.pop();
}

void SpParser::trackOperatorAndOperand(
    std::vector<int>& constAppearances,
    std::vector<std::string>& varAppearances) {
  if (AParser::IsWordToken(getCurrToken())) {
    varAppearances.push_back(getCurrTokenValue());
  } else {
    constAppearances.push_back(std::stoi(getCurrTokenValue()));
  }
}

void SpParser::buildCondExprPostFix(
    std::queue<std::shared_ptr<Token>>& postFixQueue) {
  std::stack<std::shared_ptr<Token>> operatorStack;
  int parenCount = 0;

  while (!isCurrTokenType(TokenType::EOF_TOKEN)) {
    std::shared_ptr<Token> currToken = getCurrToken();
    if (AParser::IsWordOrIntegerToken(currToken)) {
      postFixQueue.push(getCurrToken());
    } else if (isLeftParenthesisToken(currToken)) {
      handleCondExprLeftParenthesisToken(currToken, operatorStack, parenCount);
    } else if (isRightParenthesisToken(getCurrToken())) {
      --parenCount;
      condExprHandleRightParenthesisToken(operatorStack, postFixQueue);
      if (parenCount == 0) break;
    } else if (isOperator(getCurrTokenValue())) {
      if (isAndOrOrToken(getCurrToken())) {
        if (!isRightParenthesisToken(peekBackToken()) ||
            !isLeftParenthesisToken(peekToken())) {
          throw InvalidCondExprException();
        }
      }
      while (!operatorStack.empty() &&
             Precedence(operatorStack.top()->getTokenVal()) >=
                 Precedence(getCurrTokenValue())) {
        postFixQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      operatorStack.push(getCurrToken());
    } else {
      throw InvalidCondExprException();
    }

    nextToken();
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
    } else if (isMathematicalOperator(currToken->getTokenVal())) {
      validateTokenStackSize(tokenStack);
      validateWordOrIntegerToken(tokenStack);
      isTopStackNotWordOrIntegerToken(tokenStack);
    } else if (isNotToken(currToken)) {
      isTopStackNotComparisonOperatorToken(tokenStack);
    } else if (isAndOrOrToken(currToken)) {
      validateTokenStackSize(tokenStack);
      validateComparisonOperatorToken(tokenStack);
      validateComparisonOperatorToken(tokenStack);

    } else if (isComparisonOperator(currToken->getTokenVal())) {
      validateTokenStackSize(tokenStack);
      validateWordOrIntegerToken(tokenStack);
      validateWordOrIntegerToken(tokenStack);
      tokenStack.push(currToken);

    } else {
      throw InvalidCondExprException();
    }
  }
  // no operator in stack, handle (x) case
  if (tokenStack.empty() || AParser::IsWordOrIntegerToken(tokenStack.top())) {
    throw InvalidCondExprException();
  }
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

std::shared_ptr<TreeNode> SpParser::buildExprTreeAndValidate(
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  std::stack<std::shared_ptr<TreeNode>> treeStack;

  while (!postFixQueue.empty()) {
    auto element = postFixQueue.front();
    postFixQueue.pop();

    if (isMathematicalOperator(element->c_str())) {
      if (treeStack.size() < 2) {
        throw InvalidCondExprException();
      }

      auto right = treeStack.top();
      treeStack.pop();
      auto left = treeStack.top();
      treeStack.pop();

      treeStack.push(std::make_shared<TreeNode>(*element, left, right));
    } else {
      treeStack.push(std::make_shared<TreeNode>(*element, nullptr, nullptr));
    }
  }

  if (treeStack.size() != 1) {
    throw InvalidCondExprException();
  }

  return treeStack.top();
}

std::shared_ptr<AssignNode> SpParser::parseAssign(std::string const& varName) {
  std::unordered_set<std::string> variables = std::unordered_set<std::string>();
  std::unordered_set<int> constants = std::unordered_set<int>();

  std::queue<std::shared_ptr<std::string>> postFixQueue;
  std::stack<std::shared_ptr<std::string>> operatorStack;

  int parenCount = 0;

  while (!isCurrTokenValue(AParserConstant::STMT_TERMINATOR)) {
    if (AParser::IsWordOrIntegerToken(getCurrToken())) {
      handleWordOrIntegerToken(postFixQueue, variables, constants);
    } else if (isMathematicalOperator(getCurrTokenValue())) {
      handleOperatorToken(operatorStack, postFixQueue);
    } else if (isCurrTokenValue(AParserConstant::LEFT_PARENTHESIS)) {
      handleLeftParenthesisToken(operatorStack, parenCount);
    } else if (isCurrTokenValue(AParserConstant::RIGHT_PARENTHESIS)) {
      assignHandleRightParenthesisToken(operatorStack, postFixQueue,
                                        parenCount);
    } else {
      throw InvalidAssignException();
    }

    nextToken();
  }

  if (parenCount != 0) {
    throw UnmatchedParenthesesException();
  }

  while (!operatorStack.empty()) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }

  nextToken();

  try {
    std::shared_ptr<TreeNode> exprTreeRoot =
        buildExprTreeAndValidate(postFixQueue);
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
         tokenVal == SpParserConstant::OR ||
         tokenVal == SpParserConstant::NOT;
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
         tokenVal == SpParserConstant::OR ||
         tokenVal == SpParserConstant::NOT;
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

std::shared_ptr<StmtLstNode> SpParser::parseStmtLst() {
  std::vector<std::shared_ptr<StmtNode>> stmts;

  if (getCurrTokenValue() == SpParserConstant::END_PROCEDURE) {
    throw EmptyStmtLstException();
  }

  while (!isCurrTokenValue(SpParserConstant::END_PROCEDURE)) {
    std::shared_ptr<Token> currToken = getCurrToken();
    if (isCurrTokenType(TokenType::WORD_TOKEN) &&
        isPeekTokenValue(SpParserConstant::ASSIGN_SYMBOL)) {
      nextToken();  // move to assign symbol
      nextToken();
      stmts.push_back(parseAssign(currToken->getTokenVal()));
    } else if (isCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                       SpParserConstant::PRINT_KEYWORD)) {
      nextToken();
      stmts.push_back(parsePrint());
    } else if (isCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                       SpParserConstant::READ_KEYWORD)) {
      nextToken();
      stmts.push_back(parseRead());
    } else if (isCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                       SpParserConstant::CALL_KEYWORD)) {
      nextToken();
      stmts.push_back(parseCall());
    } else if (isCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                       SpParserConstant::WHILE_KEYWORD)) {
      nextToken();
      stmts.push_back(parseWhile());
    } else if (isCurrTokenTypeAndValue(TokenType::WORD_TOKEN,
                                       SpParserConstant::IF_KEYWORD)) {
      nextToken();
      stmts.push_back(parseIf());
    } else {
      throw InvalidStmtLstException();
    }
  }

  return std::make_shared<StmtLstNode>(stmts);
}

void SpParser::parse() { sourceProgramNode = parseProgram(); }

std::shared_ptr<ProgramNode> SpParser::getSourceProgramNode() {
  return sourceProgramNode;
}
