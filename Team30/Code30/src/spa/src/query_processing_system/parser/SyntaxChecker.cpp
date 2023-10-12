#include "SyntaxChecker.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "../exceptions/InvalidSemanticsException.h"
#include "../exceptions/InvalidSyntaxException.h"

SyntaxChecker::SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens){};

void SyntaxChecker::parse() {
  std::shared_ptr<Token> current_token = getCurrToken();
  if (!IsWordToken(current_token)) {
    throw InvalidSyntaxException(
        "Incorrect query format: Expected to start with either 'Select' or a "
        "<Declaration>");
  }
  if (getCurrTokenValue() != QpParser::SELECT) {
    CheckDeclaration();
  }
  CheckSelect();
  CheckClauses();
}

void SyntaxChecker::CheckAnd(ClauseType clause_type) {
  assert(getCurrTokenValue() == QpParser::AND);
  nextToken();

  if (clause_type == ClauseType::such_that) {
    if (QpParser::IsRelRef(getCurrTokenValue())) {
      CheckSuchThat(true);
      return;
    } else {
      throw InvalidSyntaxException("Expected <relref> after 'and'");
    }
  } else if (clause_type == ClauseType::pattern) {
    if (getCurrTokenValue() == QpParser::PATTERN) {
      CheckPattern();
      return;
    } else {
      throw InvalidSyntaxException("Expected 'pattern' after 'and'");
    }
  }

  throw InvalidSyntaxException(
      "Expected either 'such' or 'pattern' after 'and'");
}

void SyntaxChecker::CheckCalls() {
  assert(getCurrTokenValue() == QpParser::CALLS ||
         getCurrTokenValue() == QpParser::CALLS_STAR);

  nextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Calls clause");

  nextToken();
  this->CheckUpcomingTokensAreEntRef(
      "First arg of Calls clause not valid ent ref",
      "First arg of Calls clause has not been declared");

  nextToken();
  CheckCurrentTokenSyntax(",", "Expected \',\' for Calls clause");

  nextToken();
  this->CheckUpcomingTokensAreEntRef(
      "Second arg of Calls clause not valid ent ref",
      "Second arg of Calls clause has not been declared");

  nextToken();
  CheckCurrentTokenSyntax(")", "Expected \')\' for Calls clause");

  nextToken();
}

void SyntaxChecker::CheckClauses() {
  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    if (getCurrTokenValue() == QpParser::SUCH) {
      this->CheckSuchThat(false);
    } else if (getCurrTokenValue() == QpParser::PATTERN) {
      this->CheckPattern();
    } else {
      throw InvalidSyntaxException("Did not encounter expected clause");
    }
  }
}

void SyntaxChecker::CheckDeclaration() {
  while (getCurrTokenValue() != QpParser::SELECT) {
    if (IsEOFToken(getCurrToken())) {
      throw InvalidSyntaxException("Query missing Select Clause");
    }
    EntityType entity_type = QpParser::StringToEntityType(
        getCurrTokenValue());  // throws an error if not valid entity_type
    nextToken();               // synonym
    if (!IsSynonym(getCurrTokenValue())) {
      throw InvalidSyntaxException("Invalid synonym given in declaration");
    } else {
      if (existing_declarations.find(getCurrTokenValue()) !=
          existing_declarations.end()) {
        throw InvalidSyntaxException("Synonym cannot be declared twice");
      }
      PqlDeclaration declaration =
          PqlDeclaration(getCurrTokenValue(), entity_type);
      existing_declarations.insert(
          std::make_pair(getCurrTokenValue(), declaration));
    }
    nextToken();  // , OR ;
    while (getCurrTokenValue() == ",") {
      nextToken();  // synonym
      if (!IsSynonym(getCurrTokenValue())) {
        throw InvalidSyntaxException("Invalid synonym given in declaration");
      } else {
        if (existing_declarations.find(getCurrTokenValue()) !=
            existing_declarations.end()) {
          throw InvalidSyntaxException("Synonym cannot be declared twice");
        }
        PqlDeclaration declaration =
            PqlDeclaration(getCurrTokenValue(), entity_type);
        existing_declarations.insert(
            std::make_pair(getCurrTokenValue(), declaration));
      }
      nextToken();  // , OR ;
    }
    if (getCurrTokenValue() != ";") {
      throw InvalidSyntaxException("Invalid declaration format");
    }
    nextToken();  // entity_type or Select
  }
}

void SyntaxChecker::CheckEOF() {
  if (!IsEOFToken(getCurrToken())) {
    throw InvalidSyntaxException("EOF Token Expected");
  }
}

void SyntaxChecker::CheckFollows() {
  assert(getCurrTokenValue() == QpParser::FOLLOWS ||
         getCurrTokenValue() == QpParser::FOLLOWS_STAR);

  nextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Follows/* clause");

  nextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for first argument of Follows/* clause",
      "Synonym in first arg of Follows/* clause has not been declared");

  nextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Follows/* clause");

  nextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for second argument of Follows/* clause",
      "Synonym in second arg of Follows/* clause has not been declared");

  nextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Follows/* clause");

  nextToken();
}

void SyntaxChecker::CheckModifies() {
  assert(getCurrTokenValue() == QpParser::MODIFIES);

  nextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Modifies clause");

  nextToken();
  if (QpParser::IsStmtRef(getCurrTokenValue())) {
    this->CheckCurrentTokenStmtRef(
        "First arg of Modifies clause not valid stmt ref",
        "First arg of Modifies clause has not been declared");
  } else {
    this->CheckUpcomingTokensAreEntRef(
        "First arg of Modifies clause not valid ent ref",
        "First arg of Modifies clause has not been declared");
  }

  nextToken();
  CheckCurrentTokenSyntax(",", "Expected \',\' for Modifies clause");

  nextToken();
  this->CheckUpcomingTokensAreEntRef(
      "Second arg of Modifies clause not valid ent ref",
      "Second arg of Modifies clause has not been declared");

  nextToken();
  CheckCurrentTokenSyntax(")", "Expected \')\' for Modifies clause");

  nextToken();
}

void SyntaxChecker::CheckParent() {
  assert(getCurrTokenValue() == PARENT || getCurrTokenValue() == PARENT_STAR);

  nextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Parent/* clause");

  nextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for first argument of Parent/* clause",
      "Synonym in first arg of Parent/* clause has not been declared");

  nextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Parent/* clause");

  nextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for second argument of Parent/* clause",
      "Synonym in second arg of Parent/* clause has not been declared");

  nextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Parent/* clause");

  nextToken();
}

void SyntaxChecker::CheckPattern() {
  if (!(getCurrTokenValue() == QpParser::PATTERN)) {
    return;
  }

  nextToken();
  EntityType entity_type = this->CheckCurrentTokenPatternEntity();

  nextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Pattern clause");

  nextToken();
  this->CheckCurrentTokenPatternFirstArg(entity_type);

  nextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Pattern clause");

  nextToken();
  this->CheckCurrentTokenPatternSecondArg(entity_type);

  nextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Pattern clause");

  nextToken();
  if (getCurrTokenValue() == QpParser::AND) {
    CheckAnd(ClauseType::pattern);
  }
}

void SyntaxChecker::CheckSelect() {
  if (getCurrTokenValue() != QpParser::SELECT) {
    throw InvalidSyntaxException("Expected 'Select'");
  }

  nextToken();  // BOOLEAN or synonym or <
  if (getCurrTokenValue() == QpParser::BOOLEAN) {
    if (existing_declarations.find(QpParser::BOOLEAN) ==
        existing_declarations.end()) {
      CheckSelectBoolean();
    } else {
      CheckSelectSingle();
    }
  } else if (getCurrTokenValue() == "<") {
    CheckSelectMultiple();
  } else {
    CheckSelectSingle();
  }
  nextToken();
}

void SyntaxChecker::CheckSelectBoolean() {
  assert(getCurrTokenValue() == QpParser::BOOLEAN);
}

void SyntaxChecker::CheckSelectMultiple() {
  assert(getCurrTokenValue() == "<");

  nextToken();  // synonym
  while (getCurrTokenValue() != ">") {
    if (getCurrToken()->getTokenType() == TokenType::EOF_TOKEN) {
      throw InvalidSyntaxException(
          "Reached EOF before reaching '>' for multiple select elements");
    }

    CheckSelectSingle();

    nextToken();  // , or >
    if (getCurrTokenValue() == ",") {
      nextToken();  // element
      if (getCurrTokenValue() == ">") {
        throw InvalidSyntaxException("Extra comma detected in multiple select");
      }
    }
  }
}

void SyntaxChecker::CheckSelectSingle() {
  if (getCurrTokenValue() == QpParser::BOOLEAN) {
    if (existing_declarations.find(QpParser::BOOLEAN) ==
        existing_declarations.end()) {
      throw InvalidSyntaxException(
          "Expected 'BOOLEAN' to be a declared synonym, but has not been "
          "declared");
    }
  }
  if (!IsSynonym(getCurrTokenValue())) {
    throw InvalidSyntaxException("Expected synonym for single select clause");
  }
}

void SyntaxChecker::CheckSuchThat(bool has_and) {
  if (!has_and) {
    nextToken();  // that
    nextToken();  // relRef
  }
  if (!QpParser::IsRelRef(getCurrTokenValue())) {
    throw InvalidSyntaxException("Invalid relref for such that clause");
  }
  std::string rel_ref = getCurrTokenValue();
  if (rel_ref == QpParser::FOLLOWS || rel_ref == QpParser::FOLLOWS_STAR) {
    this->CheckFollows();
  } else if (rel_ref == QpParser::PARENT || rel_ref == QpParser::PARENT_STAR) {
    this->CheckParent();
  } else if (rel_ref == QpParser::USES) {
    this->CheckUses();
  } else if (rel_ref == QpParser::MODIFIES) {
    this->CheckModifies();
  } else if (rel_ref == QpParser::CALLS || rel_ref == QpParser::CALLS_STAR) {
    this->CheckCalls();
  }

  if (getCurrTokenValue() == QpParser::AND) {
    CheckAnd(ClauseType::such_that);
  }
}

void SyntaxChecker::CheckUses() {
  assert(getCurrTokenValue() == QpParser::USES);

  nextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Uses clause");

  nextToken();
  if (QpParser::IsStmtRef(getCurrTokenValue())) {
    this->CheckCurrentTokenStmtRef(
        "First arg of Uses clause not valid stmt ref",
        "First arg of Uses clause has not been declared");
  } else {
    this->CheckUpcomingTokensAreEntRef(
        "First arg of Uses clause not valid ent ref",
        "First arg of Uses clause has not been declared");
  }

  nextToken();
  CheckCurrentTokenSyntax(",", "Expected \',\' for Uses clause");

  nextToken();
  this->CheckUpcomingTokensAreEntRef(
      "Second arg of Uses clause not valid ent ref",
      "Second arg of Uses clause has not been declared");

  nextToken();
  CheckCurrentTokenSyntax(")", "Expected \')\' for Uses clause");

  nextToken();
}

// ---------- HELPERS -------------

EntityType SyntaxChecker::CheckCurrentTokenPatternEntity() {
  std::string token_value = getCurrTokenValue();
  if (!QpParser::IsSynonym(token_value)) {
    throw InvalidSyntaxException(
        "Variable used for pattern does not have an identifier's syntax");
  } else if (existing_declarations.find(token_value) ==
             existing_declarations.end()) {
    throw InvalidSemanticsException(
        "Variable used for pattern has not been declared");
  } else if (existing_declarations.at(token_value).getEntityType() !=
                 EntityType::ASSIGN &&
             existing_declarations.at(token_value).getEntityType() !=
                 EntityType::WHILE &&
             existing_declarations.at(token_value).getEntityType() !=
                 EntityType::IF) {
    throw InvalidSemanticsException(
        "Variable used for pattern is not an assign, while or if synonym");
  }
  return existing_declarations.at(token_value).getEntityType();
}

void SyntaxChecker::CheckCurrentTokenPatternFirstArg(EntityType variable_type) {
  if (variable_type == EntityType::ASSIGN) {
    this->CheckUpcomingTokensAreEntRef(
        "First arg of pattern clause not valid ent ref",
        "First arg of pattern clause has not been declared");
  }
}

void SyntaxChecker::CheckCurrentTokenPatternSecondArg(
    EntityType variable_type) {
  std::string token_value = getCurrTokenValue();
  if (variable_type == EntityType::ASSIGN) {
    if (getCurrTokenValue() == "_") {
      if (AParser::getPeekTokenValue() == ")") {
        // expression-spec = _
        return;
      } else {
        // expression-spec = _"expr"_
        nextToken();
        this->CheckUpcomingTokensAreQuotedExpr(
            "Expected quoted expr for pattern second arg");

        this->CheckCurrentTokenSyntax(
            "_", "Expected '_' at the ending of pattern second arg");
      }
    } else {
      this->CheckUpcomingTokensAreQuotedExpr(
          "Expected quoted expr for pattern second arg");
    }
  }
}

void SyntaxChecker::CheckCurrentTokenStmtRef(
    std::string syntax_error_msg, std::string not_existing_error_msg) {
  std::string token_value = getCurrTokenValue();
  if (!QpParser::IsStmtRef(token_value)) {
    throw InvalidSyntaxException(syntax_error_msg);
  } else {
    if (QpParser::IsSynonym(token_value)) {
      this->CheckSynonymExists(token_value, not_existing_error_msg);
    }
  }
}

void SyntaxChecker::CheckCurrentTokenSyntax(std::string expected_value,
                                            std::string error_msg) {
  if (getCurrTokenValue() != expected_value) {
    throw InvalidSyntaxException(error_msg + ". Got " + getCurrTokenValue() +
                                 " instead.");
  }
}

void SyntaxChecker::CheckIsExpr(std::string error_msg) {
  std::vector<std::shared_ptr<Token>> infix_tokens;

  while (getCurrTokenValue() != "\"") {
    if (getCurrToken()->getTokenType() == TokenType::EOF_TOKEN) {
      throw InvalidSyntaxException(error_msg);
    }
    infix_tokens.push_back(getCurrToken());
    nextToken();
  }
  try {
    AParser::ConvertInfixToPostfix(infix_tokens);
  } catch (std::invalid_argument e) {
    throw InvalidSyntaxException(error_msg);
  }
}

void SyntaxChecker::CheckSynonymExists(std::string synonym,
                                       std::string error_msg) {
  if (existing_declarations.find(synonym) == existing_declarations.end()) {
    throw InvalidSemanticsException(error_msg);
  }
}

void SyntaxChecker::CheckUpcomingTokensAreEntRef(
    std::string syntax_error_msg, std::string not_existing_error_msg) {
  std::string value = "";
  if (getCurrTokenValue() == "\"") {
    value += "\"";
    nextToken();
  }
  if (QpParser::IsIdentifier(getCurrTokenValue()) ||
      getCurrTokenValue() == "_") {
    value += getCurrTokenValue();
  }
  if (getPeekTokenValue() == "\"") {
    value += nextToken()->getTokenVal();
  }
  if (!QpParser::IsEntRef(value)) {
    throw InvalidSyntaxException(syntax_error_msg);
  } else {
    if (QpParser::IsSynonym(value)) {
      this->CheckSynonymExists(value, not_existing_error_msg);
    }
  }
}

void SyntaxChecker::CheckUpcomingTokensAreQuotedExpr(std::string error_msg) {
  CheckCurrentTokenSyntax("\"", error_msg);

  nextToken();
  this->CheckIsExpr(error_msg);

  CheckCurrentTokenSyntax("\"", error_msg);

  nextToken();
}
