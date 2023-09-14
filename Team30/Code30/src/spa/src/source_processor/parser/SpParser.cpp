#include "SpParser.h"

#include <stdexcept>
#include <string>

namespace SpParserConstant {
std::string const START_PROCEDURE = "{";
std::string const END_PROCEDURE = "}";
std::string const START_WHILE_STMTLST = "{";
std::string const END_WHILE_STMTLST = "}";
std::string const START_COND_EXPR = "(";
std::string const END_COND_EXPR = ")";
std::string const STMT_TERMINATOR = ";";
std::string const PROCEDURE_KEYWORD = "procedure";
std::string const PRINT_KEYWORD = "print";
std::string const READ_KEYWORD = "read";
std::string const CALL_KEYWORD = "call";
std::string const WHILE_KEYWORD = "while";
}  // namespace SpParserConstant

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

  nextToken();
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
  // dummy for testing purposes
  nextToken();
  nextToken();
  nextToken();
  return std::make_shared<CondExprNode>(std::unordered_set<std::string>{"1", "2", "3"}, std::unordered_set<int>{1,2,3});
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
