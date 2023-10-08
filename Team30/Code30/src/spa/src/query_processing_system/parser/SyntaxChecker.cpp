#include "SyntaxChecker.h"

#include <iostream>
#include <stdexcept>

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
  if (current_token->getTokenVal() == "Select") {
    // no declaration, only select
    CheckSelect();
    CheckSuchThatOrPattern();
  } else {
    CheckDeclaration();
    CheckSelect();
    CheckSuchThatOrPattern();
  }
}

void SyntaxChecker::CheckDeclaration() {
  while (getCurrToken()->getTokenVal() != "Select") {
    if (IsEOFToken(getCurrToken())) {
      throw InvalidSyntaxException("Query missing Select Clause");
    }
    EntityType entity_type = QpParser::StringToEntityType(
        getCurrToken()
            ->getTokenVal());  // throws an error if not valid entity_type
    nextToken();               // synonym
    if (!IsSynonym(getCurrToken()->getTokenVal())) {
      throw InvalidSyntaxException("Invalid synonym given in declaration");
    } else {
      if (existing_declarations.find(getCurrToken()->getTokenVal()) !=
          existing_declarations.end()) {
        throw InvalidSyntaxException("Synonym cannot be declared twice");
      }
      existing_declarations.insert(getCurrToken()->getTokenVal());
    }
    nextToken();  // , OR ;
    while (getCurrToken()->getTokenVal() == ",") {
      nextToken();  // synonym
      if (!IsSynonym(getCurrToken()->getTokenVal())) {
        throw InvalidSyntaxException("Invalid synonym given in declaration");
      } else {
        if (existing_declarations.find(getCurrToken()->getTokenVal()) !=
            existing_declarations.end()) {
          throw InvalidSyntaxException("Synonym cannot be declared twice");
        }
        existing_declarations.insert(getCurrToken()->getTokenVal());
      }
      nextToken();  // , OR ;
    }
    if (getCurrToken()->getTokenVal() != ";") {
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
  assert(getCurrToken()->getTokenVal() == "Follows" ||
         getCurrToken()->getTokenVal() == "Follows*");

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
  assert(getCurrToken()->getTokenVal() == "Modifies");

  nextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Modifies clause");

  nextToken();
  if (QpParser::IsStmtRef(getCurrToken()->getTokenVal())) {
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
  assert(getCurrToken()->getTokenVal() == "Parent" ||
         getCurrToken()->getTokenVal() == "Parent*");

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
  if (getCurrToken()->getTokenVal() != "pattern") {
    return;
  }
  if (!(QpParser::IsSynonym(nextToken()->getTokenVal()))) {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: Expected 'syn-assign' after 'pattern'");
  }
  if (nextToken()->getTokenVal() != "(") {
    throw InvalidSyntaxException("Invalid pattern syntax: Expected '('");
  }

  nextToken();  // synonym or "

  if (getCurrToken()->getTokenVal() != "\"" &&
      getCurrToken()->getTokenVal() != "_" &&
      getCurrToken()->getTokenType() != TokenType::WORD_TOKEN) {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: First arg should be synonym, variable or "
        "wildcard");
  }

  // check valid variable
  if (getCurrToken()->getTokenVal() == "\"") {
    if ((nextToken()->getTokenType() != TokenType::WORD_TOKEN) ||
        nextToken()->getTokenVal() != "\"") {
      throw InvalidSyntaxException(
          "Invalid pattern syntax: Variable name is not valid");
    }
  }

  if (nextToken()->getTokenVal() != ",") {
    throw InvalidSyntaxException("Invalid pattern syntax: expected ,");
  }

  if (nextToken()->getTokenVal() != "_" &&
      getCurrToken()->getTokenType() != TokenType::WORD_TOKEN) {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: Second arg should be synonym, wild card or "
        "partial "
        "match");
  }

  // check for partial match
  if (getCurrToken()->getTokenVal() == "_") {
    if (peekToken()->getTokenVal() == "\"") {
      nextToken();
      if (!(nextToken()->getTokenType() == TokenType::INTEGER_TOKEN ||
            getCurrToken()->getTokenType() == TokenType::WORD_TOKEN) ||
          nextToken()->getTokenVal() != "\"" ||
          nextToken()->getTokenVal() != "_") {
        throw InvalidSyntaxException(getCurrToken()->getTokenVal());
      }
    }
  }

  nextToken();  // )
  if (getCurrToken()->getTokenVal() != ")") {
    throw InvalidSyntaxException(
        "Invalid pattern syntax after second arg: Expected ')'");
  }
  nextToken();  // pattern end
}

void SyntaxChecker::CheckSelect() {
  if (getCurrToken()->getTokenVal() != "Select") {
    throw InvalidSyntaxException("Select clause missing or invalid");
  }
  // Current token is already "Select"
  nextToken();  // synonym
  if (!IsSynonym(getCurrToken()->getTokenVal())) {
    throw InvalidSyntaxException("Invalid synonym given for Select clause");
  }
  nextToken();
}

void SyntaxChecker::CheckSuchThat() {
  if (getCurrToken()->getTokenVal() != "such") {
    return;
  }
  nextToken();  // that
  nextToken();  // relRef
  if (!QpParser::IsRelRef(getCurrToken()->getTokenVal())) {
    throw InvalidSyntaxException("Invalid relref for such that clause");
  }
  std::string rel_ref = getCurrToken()->getTokenVal();
  if (rel_ref == "Follows" || rel_ref == "Follows*") {
    this->CheckFollows();
  } else if (rel_ref == "Parent" || rel_ref == "Parent*") {
    this->CheckParent();
  } else if (rel_ref == "Uses") {
    this->CheckUses();
  } else if (rel_ref == "Modifies") {
    this->CheckModifies();
  }
}

void SyntaxChecker::CheckSuchThatOrPattern() {
  for (int i = 0; i < 2; i++) {
    CheckSuchThat();
    CheckPattern();
  }
}

void SyntaxChecker::CheckUses() {
  assert(getCurrToken()->getTokenVal() == "Uses");

  nextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Uses clause");

  nextToken();
  if (QpParser::IsStmtRef(getCurrToken()->getTokenVal())) {
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

// ---------- HELPER CHECKERS -------------

void SyntaxChecker::CheckCurrentTokenStmtRef(
    std::string syntax_error_msg, std::string not_existing_error_msg) {
  std::string token_value = getCurrToken()->getTokenVal();
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
  if (getCurrToken()->getTokenVal() != expected_value) {
    throw InvalidSyntaxException(error_msg + ". Got " +
                                 getCurrToken()->getTokenVal() + " instead.");
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
  if (getCurrToken()->getTokenVal() == "\"") {
    value += "\"";
    nextToken();
  }
  if (QpParser::IsIdentifier(getCurrToken()->getTokenVal()) ||
      getCurrToken()->getTokenVal() == "_") {
    value += getCurrToken()->getTokenVal();
  }
  if (peekToken()->getTokenVal() == "\"") {
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
