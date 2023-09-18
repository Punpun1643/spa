#include "SpParser.h"

#include <queue>
#include <stack>
#include <stdexcept>
#include <unordered_set>

namespace SpParserConstant {
std::string const START_PROCEDURE = "{";
std::string const END_PROCEDURE = "}";
std::string const START_WHILE_STMTLST = "{";
std::string const END_WHILE_STMTLST = "}";
std::string const START_COND_EXPR = "(";
std::string const END_COND_EXPR = ")";
std::string const LEFT_PARENTHESIS = "(";
std::string const RIGHT_PARENTHESIS = ")";
std::string const STMT_TERMINATOR = ";";
std::string const PROCEDURE_KEYWORD = "procedure";
std::string const PRINT_KEYWORD = "print";
std::string const READ_KEYWORD = "read";
std::string const CALL_KEYWORD = "call";
std::string const WHILE_KEYWORD = "while";
}  // namespace SpParserConstant

namespace SpParserMathOperator {
std::string const PLUS = "+";
std::string const MINUS = "-";
std::string const MULTIPLY = "*";
std::string const DIVIDE = "/";
std::string const MODULO = "%";
}  // namespace SpParserMathOperator

namespace SpParserComparisonOperator {
std::string const EQUAL = "==";
std::string const NOT_EQUAL = "!=";
std::string const LESS_THAN = "<";
std::string const LESS_THAN_EQUAL = "<=";
std::string const GREATER_THAN = ">";
std::string const GREATER_THAN_EQUAL = ">=";
}  // namespace SpParserComparisonOperator

namespace SpRelationLogicalOperator {
std::string const AND = "&&";
std::string const OR = "||";
std::string const NOT = "!";
}  // namespace SpRelationLogicalOperator

SpParser::SpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens) {}

std::shared_ptr<ProgramNode> SpParser::parseProgram() {
  std::vector<std::shared_ptr<ProcedureNode>> procedures;

  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    // current token
    std::shared_ptr<Token> currToken = getCurrToken();
    if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
        currToken->getTokenVal() == SpParserConstant::PROCEDURE_KEYWORD) {
      // increment index token to get next token
      nextToken();
      // parse procedure
      procedures.push_back(parseProcedure());
    } else {
      throw std::invalid_argument("Invalid procedure");
    }
  }

  return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr<ProcedureNode> SpParser::parseProcedure() {
  // check if valid procedureName
  std::shared_ptr<Token> currToken = getCurrToken();

  if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
    std::string procedureName = currToken->getTokenVal();

    nextToken();
    // check if valid procedure
    if (getCurrToken()->getTokenVal() == SpParserConstant::START_PROCEDURE) {
      // increment index token to get next token
      nextToken();
      // parse stmtLst
      std::shared_ptr<StmtLstNode> stmtLst = parseStmtLst();

      if (getCurrToken()->getTokenVal() == SpParserConstant::END_PROCEDURE) {
        nextToken();
        return std::make_shared<ProcedureNode>(procedureName, stmtLst);
      } else {
        throw std::invalid_argument("Invalid procedure 1");
      }
    } else {
      throw std::invalid_argument("Invalid procedure 2");
    }
  } else {
    throw std::invalid_argument("Invalid procedure 4");
  }
}

std::shared_ptr<PrintNode> SpParser::parsePrint() {
  std::shared_ptr<Token> currToken = getCurrToken();

  if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
    std::string varName = currToken->getTokenVal();

    nextToken();

    // check if valid print
    if (getCurrToken()->getTokenVal() == SpParserConstant::STMT_TERMINATOR) {
      nextToken();

      return std::make_shared<PrintNode>(currStmtIndex++, StmtType::PRINT_STMT,
                                         varName);
    } else {
      throw std::invalid_argument("Invalid print 1");
    }
  } else {
    throw std::invalid_argument("Invalid print 2");
  }
}

std::shared_ptr<ReadNode> SpParser::parseRead() {
  std::shared_ptr<Token> currToken = getCurrToken();

  if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
    std::string varName = currToken->getTokenVal();
    nextToken();
    if (getCurrToken()->getTokenVal() == SpParserConstant::STMT_TERMINATOR) {
      nextToken();
      return std::make_shared<ReadNode>(currStmtIndex++, StmtType::READ_STMT,
                                        varName);
    } else {
      throw std::invalid_argument("Invalid read 1");
    }
  } else {
    throw std::invalid_argument("Invalid read 2");
  }
}

std::shared_ptr<CallNode> SpParser::parseCall() {
  std::shared_ptr<Token> currToken = getCurrToken();

  if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
    std::string procedureName = currToken->getTokenVal();
    nextToken();
    if (getCurrToken()->getTokenVal() == SpParserConstant::STMT_TERMINATOR) {
      nextToken();
      return std::make_shared<CallNode>(currStmtIndex++, StmtType::CALL_STMT,
                                        procedureName);
    } else {
      throw std::invalid_argument("Invalid call 1");
    }
  } else {
    throw std::invalid_argument("Invalid call 2");
  }
}

std::shared_ptr<WhileNode> SpParser::parseWhile() {
  std::shared_ptr<Token> currToken = getCurrToken();
  std::shared_ptr<CondExprNode> condExpr;
  std::shared_ptr<StmtLstNode> stmtLst;

  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::START_COND_EXPR) {
    throw std::invalid_argument("Invalid while 1");
  }

  condExpr = parseCondExpr();

  currToken = getCurrToken();
  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::END_COND_EXPR) {
    throw std::invalid_argument("Invalid while 2");
  }

  nextToken();
  currToken = getCurrToken();
  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::START_WHILE_STMTLST) {
    throw std::invalid_argument("Invalid while 3");
  }

  nextToken();
  stmtLst = parseStmtLst();

  currToken = getCurrToken();
  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::END_WHILE_STMTLST) {
    throw std::invalid_argument("Invalid while 4");
  }

  nextToken();
  return std::make_shared<WhileNode>(currStmtIndex++, StmtType::WHILE_STMT,
                                     condExpr, stmtLst);
}

std::shared_ptr<CondExprNode> SpParser::parseCondExpr() {
  std::queue<std::shared_ptr<std::string>> postFixQueue;
  std::stack<std::shared_ptr<std::string>> operatorStack;
  std::unordered_set<std::string> variables;
  std::unordered_set<int> constants;
  bool isParseRelExpr = false;

  int parenCount = 0;

  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    std::shared_ptr<Token> currToken = getCurrToken();

    if (currToken->getTokenType() == TokenType::WORD_TOKEN ||
        currToken->getTokenType() == TokenType::INTEGER_TOKEN) {
      postFixQueue.push(
          std::make_shared<std::string>(currToken->getTokenVal()));
      if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
        variables.insert(currToken->getTokenVal());
      } else if (currToken->getTokenType() == TokenType::INTEGER_TOKEN) {
        constants.insert(std::stoi(currToken->getTokenVal()));
      }

    } else if (isOperator(currToken->getTokenVal())) {
      // if it's an operator, pop operators from the stack to the postFix
      // until the stack top has an operator for lower precedence or the stack
      // is empty
      if (isLogicalOperator(currToken->getTokenVal()) &&
          operatorStack.top()->compare(SpParserConstant::LEFT_PARENTHESIS) !=
              0) {
        throw std::invalid_argument("Invalid condExpr");
      }
      while (!operatorStack.empty() &&
             precedence(operatorStack.top()->c_str()) >=
                 precedence(currToken->getTokenVal())) {
        postFixQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      operatorStack.push(
          std::make_shared<std::string>(currToken->getTokenVal()));
    } else if (currToken->getTokenVal() == SpParserConstant::LEFT_PARENTHESIS) {
      ++parenCount;
      operatorStack.push(
          std::make_shared<std::string>(currToken->getTokenVal()));
    } else if (currToken->getTokenVal() ==
               SpParserConstant::RIGHT_PARENTHESIS) {
      if (parenCount <= 0) {
        throw std::invalid_argument("No matching parenthesis");
      }
      while (operatorStack.top()->compare(SpParserConstant::LEFT_PARENTHESIS) !=
             0) {
        if (isComparisonOperator(operatorStack.top()->c_str())) {
          isParseRelExpr = true;  // closing relExpr
        }
        postFixQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      operatorStack.pop();
      --parenCount;

      // check valid relExpr
      if (isParseRelExpr && !operatorStack.empty() &&
          !isLogicalOperator(peekToken()->getTokenVal()) &&
          operatorStack.top()->compare(SpParserConstant::LEFT_PARENTHESIS) ==
              0) {
        throw std::invalid_argument("Invalid relExpr");
      }
      if (parenCount == 0) break;
    } else {
      throw std::invalid_argument("Invalid condExpr");
    }

    nextToken();
  }

  if (parenCount != 0) {
    throw std::invalid_argument("Unmatched parentheses");
  }

  return std::make_shared<CondExprNode>(variables, constants);
}

bool SpParser::isOperator(std::string const& tokenVal) {
  return tokenVal == SpParserMathOperator::PLUS ||
         tokenVal == SpParserMathOperator::MINUS ||
         tokenVal == SpParserMathOperator::MULTIPLY ||
         tokenVal == SpParserMathOperator::DIVIDE ||
         tokenVal == SpParserMathOperator::MODULO ||
         tokenVal == SpParserComparisonOperator::EQUAL ||
         tokenVal == SpParserComparisonOperator::NOT_EQUAL ||
         tokenVal == SpParserComparisonOperator::LESS_THAN ||
         tokenVal == SpParserComparisonOperator::LESS_THAN_EQUAL ||
         tokenVal == SpParserComparisonOperator::GREATER_THAN ||
         tokenVal == SpParserComparisonOperator::GREATER_THAN_EQUAL ||
         tokenVal == SpRelationLogicalOperator::AND ||
         tokenVal == SpRelationLogicalOperator::OR ||
         tokenVal == SpRelationLogicalOperator::NOT;
}

bool SpParser::isComparisonOperator(std::string const& tokenVal) {
  return tokenVal == SpParserComparisonOperator::EQUAL ||
         tokenVal == SpParserComparisonOperator::NOT_EQUAL ||
         tokenVal == SpParserComparisonOperator::LESS_THAN ||
         tokenVal == SpParserComparisonOperator::LESS_THAN_EQUAL ||
         tokenVal == SpParserComparisonOperator::GREATER_THAN ||
         tokenVal == SpParserComparisonOperator::GREATER_THAN_EQUAL;
}

bool SpParser::isLogicalOperator(std::string const& tokenVal) {
  return tokenVal == SpRelationLogicalOperator::AND ||
         tokenVal == SpRelationLogicalOperator::OR ||
         tokenVal == SpRelationLogicalOperator::NOT;
}

// helper function to calculate precedence of an operator
int SpParser::precedence(std::string const& op) {
  if (op.compare(SpParserMathOperator::MULTIPLY) == 0 ||
      op.compare(SpParserMathOperator::DIVIDE) == 0 ||
      op.compare(SpParserMathOperator::MODULO) == 0) {
    return 4;
  } else if (op.compare(SpParserMathOperator::PLUS) == 0 ||
             op.compare(SpParserMathOperator::MINUS) == 0) {
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

  while (getCurrToken()->getTokenVal() != SpParserConstant::END_PROCEDURE) {
    std::shared_ptr<Token> currToken = getCurrToken();
    if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
        currToken->getTokenVal() == SpParserConstant::PRINT_KEYWORD) {
      nextToken();
      // parse print
      stmts.push_back(parsePrint());
    } else if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
               currToken->getTokenVal() == SpParserConstant::READ_KEYWORD) {
      nextToken();
      // parse read
      stmts.push_back(parseRead());
    } else if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
               currToken->getTokenVal() == SpParserConstant::CALL_KEYWORD) {
      nextToken();
      // parse call
      stmts.push_back(parseCall());
    } else if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
               currToken->getTokenVal() == SpParserConstant::WHILE_KEYWORD) {
      nextToken();
      // parse while
      stmts.push_back(parseWhile());
    } else {
      throw std::invalid_argument(
          "The stmtLst is invalid as there are stmts that are not print, read, "
          "call or while");
    }
  }

  return std::make_shared<StmtLstNode>(stmts);
}

void SpParser::parse() { sourceProgramNode = parseProgram(); }

std::shared_ptr<ProgramNode> SpParser::getSourceProgramNode() {
  return sourceProgramNode;
}
