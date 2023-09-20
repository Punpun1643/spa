#include "SpParser.h"

#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>
#include <unordered_set>

namespace SpParserConstant {

// Common symbols
constexpr char LEFT_PARENTHESIS[] = "(";
constexpr char RIGHT_PARENTHESIS[] = ")";
constexpr char STMT_TERMINATOR[] = ";";

// Procedure boundaries
constexpr char START_PROCEDURE[] = "{";
constexpr char END_PROCEDURE[] = "}";

// While statement boundaries
constexpr char START_WHILE_STMTLST[] = "{";
constexpr char END_WHILE_STMTLST[] = "}";

// Conditional statement boundaries
constexpr char START_COND_EXPR[] = "(";
constexpr char END_COND_EXPR[] = ")";

// If statement boundaries
constexpr char START_THEN_STMTLST[] = "{";
constexpr char END_THEN_STMTLST[] = "}";
constexpr char START_ELSE_STMTLST[] = "{";
constexpr char END_ELSE_STMTLST[] = "}";

// Assignment symbol
constexpr char ASSIGN_SYMBOL[] = "=";

// Keywords
constexpr char PROCEDURE_KEYWORD[] = "procedure";
constexpr char PRINT_KEYWORD[] = "print";
constexpr char READ_KEYWORD[] = "read";
constexpr char CALL_KEYWORD[] = "call";
constexpr char WHILE_KEYWORD[] = "while";
constexpr char IF_KEYWORD[] = "if";
constexpr char THEN_KEYWORD[] = "then";
constexpr char ELSE_KEYWORD[] = "else";

}  // namespace SpParserConstant

namespace SpParserMathOperator {
// Mathematical operators for expressions
constexpr char PLUS[] = "+";
constexpr char MINUS[] = "-";
constexpr char MULTIPLY[] = "*";
constexpr char DIVIDE[] = "/";
constexpr char MODULO[] = "%";
}  // namespace SpParserMathOperator

namespace SpParserComparisonOperator {
// Comparison operators for conditional expressions
constexpr char EQUAL[] = "==";
constexpr char NOT_EQUAL[] = "!=";
constexpr char LESS_THAN[] = "<";
constexpr char LESS_THAN_EQUAL[] = "<=";
constexpr char GREATER_THAN[] = ">";
constexpr char GREATER_THAN_EQUAL[] = ">=";
}  // namespace SpParserComparisonOperator

namespace SpRelationLogicalOperator {
// Logical operators for combining conditions
constexpr char AND[] = "&&";
constexpr char OR[] = "||";
constexpr char NOT[] = "!";
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

std::shared_ptr<IfNode> SpParser::parseIf() {
  std::shared_ptr<Token> currToken = getCurrToken();
  std::shared_ptr<CondExprNode> condExpr;
  std::shared_ptr<StmtLstNode> thenStmtLst;
  std::shared_ptr<StmtLstNode> elseStmtLst;

  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::START_COND_EXPR) {
    throw std::invalid_argument("Invalid if 1");
  }

  condExpr = parseCondExpr();

  currToken = getCurrToken();
  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::END_COND_EXPR) {
    throw std::invalid_argument("Invalid if 2");
  }

  nextToken();
  currToken = getCurrToken();

  if (currToken->getTokenType() != TokenType::WORD_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::THEN_KEYWORD) {
    throw std::invalid_argument("Invalid if 3");
  }

  nextToken();
  currToken = getCurrToken();

  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::START_THEN_STMTLST) {
    throw std::invalid_argument("Invalid if 4");
  }

  nextToken();
  thenStmtLst = parseStmtLst();

  currToken = getCurrToken();
  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::END_THEN_STMTLST) {
    throw std::invalid_argument("Invalid if 5");
  }

  nextToken();
  currToken = getCurrToken();

  if (currToken->getTokenType() != TokenType::WORD_TOKEN &&
      currToken->getTokenVal() != SpParserConstant::ELSE_KEYWORD) {
    throw std::invalid_argument("Invalid if 6");
  }

  nextToken();
  currToken = getCurrToken();

  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::START_ELSE_STMTLST) {
    throw std::invalid_argument("Invalid if 7");
  }

  nextToken();
  elseStmtLst = parseStmtLst();

  currToken = getCurrToken();
  if (currToken->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN ||
      currToken->getTokenVal() != SpParserConstant::END_ELSE_STMTLST) {
    throw std::invalid_argument("Invalid if 8");
  }

  nextToken();
  return std::make_shared<IfNode>(currStmtIndex++, StmtType::IF_STMT, condExpr,
                                  thenStmtLst, elseStmtLst);
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

std::shared_ptr<TreeNode> SpParser::buildExprTreeAndValidate(
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  // print out element in postfix queue
  std::queue<std::shared_ptr<std::string>> temp = postFixQueue;
  while (!temp.empty()) {
    std::cout << *temp.front() << " " << std::endl;
    temp.pop();
  }
  std::stack<std::shared_ptr<TreeNode>> treeStack;

  while (!postFixQueue.empty()) {
    auto element = postFixQueue.front();
    postFixQueue.pop();

    if (isOperator(element->c_str())) {
      if (treeStack.size() < 2) {
        throw std::invalid_argument(
            "Invalid expression: insufficient operands for operator");
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
    throw std::invalid_argument(
        "Invalid expression: mismatched operators and operands");
  }

  return treeStack.top();
}

std::shared_ptr<AssignNode> SpParser::parseAssign(std::string const& varName) {
  std::unordered_set<std::string> variables = std::unordered_set<std::string>();
  std::unordered_set<int> constants = std::unordered_set<int>();

  std::queue<std::shared_ptr<std::string>> postFixQueue;
  std::stack<std::shared_ptr<std::string>> operatorStack;

  int parenCount = 0;

  while (getCurrToken()->getTokenVal() != SpParserConstant::STMT_TERMINATOR) {
    std::shared_ptr<Token> currToken = getCurrToken();

    // MS1: only extract variables and constants on RHS
    if (currToken->getTokenType() == TokenType::WORD_TOKEN ||
        currToken->getTokenType() == TokenType::INTEGER_TOKEN) {
      postFixQueue.push(
          std::make_shared<std::string>(currToken->getTokenVal()));

      if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
        variables.insert(currToken->getTokenVal());
      } else if (currToken->getTokenType() == TokenType::INTEGER_TOKEN) {
        constants.insert(std::stoi(currToken->getTokenVal()));
      }

    } else if (isMathematicalOperator(currToken->getTokenVal())) {
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
        postFixQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      --parenCount;
      operatorStack.pop();
    } else {
      throw std::invalid_argument("Invalid assign");
    }

    nextToken();
  }

  if (parenCount != 0) {
    throw std::invalid_argument("Unmatched parentheses");
  }

  while (!operatorStack.empty()) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }

  std::shared_ptr<TreeNode> exprTreeRoot =
      std::make_shared<TreeNode>("0", nullptr, nullptr);

  nextToken();

  try {
    exprTreeRoot = buildExprTreeAndValidate(postFixQueue);
  } catch (std::invalid_argument& e) {
    throw std::invalid_argument("Invalid expression");
  }

  return std::make_shared<AssignNode>(currStmtIndex++, StmtType::ASSIGN_STMT,
                                      variables, constants, varName,
                                      exprTreeRoot);
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

bool SpParser::isMathematicalOperator(std::string const& tokenVal) {
  return tokenVal == SpParserMathOperator::PLUS ||
         tokenVal == SpParserMathOperator::MINUS ||
         tokenVal == SpParserMathOperator::MULTIPLY ||
         tokenVal == SpParserMathOperator::DIVIDE ||
         tokenVal == SpParserMathOperator::MODULO;
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
        peekToken()->getTokenVal() == SpParserConstant::ASSIGN_SYMBOL) {
      // parse assign
      nextToken();  // move to assign symbol
      nextToken();
      stmts.push_back(parseAssign(currToken->getTokenVal()));
    } else if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
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
    } else if (currToken->getTokenType() == TokenType::WORD_TOKEN &&
               currToken->getTokenVal() == SpParserConstant::IF_KEYWORD) {
      nextToken();
      // parse if
      stmts.push_back(parseIf());
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
