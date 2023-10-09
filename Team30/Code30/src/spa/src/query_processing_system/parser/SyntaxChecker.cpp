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

void SyntaxChecker::CheckCalls() {
  assert(getCurrToken()->getTokenVal() == "Calls" ||
         getCurrToken()->getTokenVal() == "Calls*");

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
      PqlDeclaration declaration =
          PqlDeclaration(getCurrToken()->getTokenVal(), entity_type);
      existing_declarations.insert(
          std::make_pair(getCurrToken()->getTokenVal(), declaration));
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
        PqlDeclaration declaration =
            PqlDeclaration(getCurrToken()->getTokenVal(), entity_type);
        existing_declarations.insert(
            std::make_pair(getCurrToken()->getTokenVal(), declaration));
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
  if (!(getCurrToken()->getTokenVal() == "pattern")) {
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
  } else if (rel_ref == "Calls" || rel_ref == "Calls*") {
    this->CheckCalls();
  }
}

void SyntaxChecker::CheckSuchThatOrPattern() {
  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    if (getCurrToken()->getTokenVal() == "such") {
      this->CheckSuchThat();
    } else if (getCurrToken()->getTokenVal() == "pattern") {
      this->CheckPattern();
    }
    else {
      throw InvalidSyntaxException("Did not encounter expected clause");
    }
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

EntityType SyntaxChecker::CheckCurrentTokenPatternEntity() {
  std::string token_value = getCurrToken()->getTokenVal();
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
  std::string token_value = getCurrToken()->getTokenVal();
  if (variable_type == EntityType::ASSIGN) {
    if (getCurrToken()->getTokenVal() == "_") {
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

void SyntaxChecker::CheckIsExpr(std::string error_msg) { 
  std::vector<std::shared_ptr<Token>> infix_tokens;

  while (getCurrToken()->getTokenVal() != "\"") {
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

void SyntaxChecker::CheckUpcomingTokensAreQuotedExpr(std::string error_msg) {
  CheckCurrentTokenSyntax("\"", error_msg);

  nextToken();
  this->CheckIsExpr(error_msg);

  CheckCurrentTokenSyntax("\"", error_msg);

  nextToken();
}
