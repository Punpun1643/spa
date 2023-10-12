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
  std::shared_ptr<Token> current_token = GetCurrToken();
  if (!IsWordToken(current_token)) {
    throw InvalidSyntaxException(
        "Incorrect query format: Expected to start with either 'Select' or a "
        "<Declaration>");
  }
  if (GetCurrTokenValue() != QpParser::SELECT) {
    CheckDeclaration();
  }
  CheckSelect();
  CheckClauses();
}

void SyntaxChecker::CheckAnd(ClauseType clause_type) {
  assert(GetCurrTokenValue() == QpParser::AND);
  NextToken();

  if (clause_type == ClauseType::such_that) {
    if (QpParser::IsRelRef(GetCurrTokenValue())) {
      CheckSuchThat(true);
      return;
    } else {
      throw InvalidSyntaxException("Expected <relref> after 'and'");
    }
  } else if (clause_type == ClauseType::pattern) {
    if (GetCurrTokenValue() == QpParser::PATTERN) {
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
  assert(GetCurrTokenValue() == QpParser::CALLS ||
         GetCurrTokenValue() == QpParser::CALLS_STAR);

  NextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Calls clause");

  NextToken();
  this->CheckUpcomingTokensAreEntRef(
      "First arg of Calls clause not valid ent ref",
      "First arg of Calls clause has not been declared");

  NextToken();
  CheckCurrentTokenSyntax(",", "Expected \',\' for Calls clause");

  NextToken();
  this->CheckUpcomingTokensAreEntRef(
      "Second arg of Calls clause not valid ent ref",
      "Second arg of Calls clause has not been declared");

  NextToken();
  CheckCurrentTokenSyntax(")", "Expected \')\' for Calls clause");

  NextToken();
}

void SyntaxChecker::CheckClauses() {
  while (GetCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    if (GetCurrTokenValue() == QpParser::SUCH) {
      this->CheckSuchThat(false);
    } else if (GetCurrTokenValue() == QpParser::PATTERN) {
      this->CheckPattern();
    } else {
      throw InvalidSyntaxException("Did not encounter expected clause");
    }
  }
}

void SyntaxChecker::CheckDeclaration() {
  while (GetCurrTokenValue() != QpParser::SELECT) {
    if (IsEOFToken(GetCurrToken())) {
      throw InvalidSyntaxException("Query missing Select Clause");
    }
    EntityType entity_type = QpParser::StringToEntityType(
        GetCurrTokenValue());  // throws an error if not valid entity_type
    NextToken();               // synonym
    if (!IsSynonym(GetCurrTokenValue())) {
      throw InvalidSyntaxException("Invalid synonym given in declaration");
    } else {
      if (existing_declarations.find(GetCurrTokenValue()) !=
          existing_declarations.end()) {
        throw InvalidSyntaxException("Synonym cannot be declared twice");
      }
      PqlDeclaration declaration =
          PqlDeclaration(GetCurrTokenValue(), entity_type);
      existing_declarations.insert(
          std::make_pair(GetCurrTokenValue(), declaration));
    }
    NextToken();  // , OR ;
    while (GetCurrTokenValue() == ",") {
      NextToken();  // synonym
      if (!IsSynonym(GetCurrTokenValue())) {
        throw InvalidSyntaxException("Invalid synonym given in declaration");
      } else {
        if (existing_declarations.find(GetCurrTokenValue()) !=
            existing_declarations.end()) {
          throw InvalidSyntaxException("Synonym cannot be declared twice");
        }
        PqlDeclaration declaration =
            PqlDeclaration(GetCurrTokenValue(), entity_type);
        existing_declarations.insert(
            std::make_pair(GetCurrTokenValue(), declaration));
      }
      NextToken();  // , OR ;
    }
    if (GetCurrTokenValue() != ";") {
      throw InvalidSyntaxException("Invalid declaration format");
    }
    NextToken();  // entity_type or Select
  }
}

void SyntaxChecker::CheckEOF() {
  if (!IsEOFToken(GetCurrToken())) {
    throw InvalidSyntaxException("EOF Token Expected");
  }
}

void SyntaxChecker::CheckFollows() {
  assert(GetCurrTokenValue() == QpParser::FOLLOWS ||
         GetCurrTokenValue() == QpParser::FOLLOWS_STAR);

  NextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Follows/* clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for first argument of Follows/* clause",
      "Synonym in first arg of Follows/* clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Follows/* clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for second argument of Follows/* clause",
      "Synonym in second arg of Follows/* clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Follows/* clause");

  NextToken();
}

void SyntaxChecker::CheckModifies() {
  assert(GetCurrTokenValue() == QpParser::MODIFIES);

  NextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Modifies clause");

  NextToken();
  if (QpParser::IsStmtRef(GetCurrTokenValue())) {
    this->CheckCurrentTokenStmtRef(
        "First arg of Modifies clause not valid stmt ref",
        "First arg of Modifies clause has not been declared");
  } else {
    this->CheckUpcomingTokensAreEntRef(
        "First arg of Modifies clause not valid ent ref",
        "First arg of Modifies clause has not been declared");
  }

  NextToken();
  CheckCurrentTokenSyntax(",", "Expected \',\' for Modifies clause");

  NextToken();
  this->CheckUpcomingTokensAreEntRef(
      "Second arg of Modifies clause not valid ent ref",
      "Second arg of Modifies clause has not been declared");

  NextToken();
  CheckCurrentTokenSyntax(")", "Expected \')\' for Modifies clause");

  NextToken();
}

void SyntaxChecker::CheckParent() {
  assert(GetCurrTokenValue() == PARENT ||
         GetCurrTokenValue() == PARENT_STAR);

  NextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Parent/* clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for first argument of Parent/* clause",
      "Synonym in first arg of Parent/* clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Parent/* clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for second argument of Parent/* clause",
      "Synonym in second arg of Parent/* clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Parent/* clause");

  NextToken();
}

void SyntaxChecker::CheckPattern() {
  if (!(GetCurrTokenValue() == QpParser::PATTERN)) {
    return;
  }

  NextToken();
  EntityType entity_type = this->CheckCurrentTokenPatternEntity();

  NextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Pattern clause");

  NextToken();
  this->CheckCurrentTokenPatternFirstArg(entity_type);

  NextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Pattern clause");

  NextToken();
  this->CheckCurrentTokenPatternSecondArg(entity_type);

  NextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Pattern clause");


  NextToken();
  if (GetCurrTokenValue() == QpParser::AND) {
    CheckAnd(ClauseType::pattern);
  }
}

void SyntaxChecker::CheckSelect() {
  if (GetCurrTokenValue() != QpParser::SELECT) {
    throw InvalidSyntaxException("Select clause missing or invalid");
  }
  // Current token is already "Select"
  NextToken();  // synonym
  if (!IsSynonym(GetCurrTokenValue())) {
    throw InvalidSyntaxException("Invalid synonym given for Select clause");
  }
  NextToken();
}

void SyntaxChecker::CheckSuchThat(bool has_and) {
  if (!has_and) {
    NextToken();  // that
    NextToken();  // relRef
  }
  if (!QpParser::IsRelRef(GetCurrTokenValue())) {
    throw InvalidSyntaxException("Invalid relref for such that clause");
  }
  std::string rel_ref = GetCurrTokenValue();
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

  if (GetCurrTokenValue() == QpParser::AND) {
    CheckAnd(ClauseType::such_that);
  }
}

void SyntaxChecker::CheckUses() {
  assert(GetCurrTokenValue() == QpParser::USES);

  NextToken();
  CheckCurrentTokenSyntax("(", "Expected \'(\' for Uses clause");

  NextToken();
  if (QpParser::IsStmtRef(GetCurrTokenValue())) {
    this->CheckCurrentTokenStmtRef(
        "First arg of Uses clause not valid stmt ref",
        "First arg of Uses clause has not been declared");
  } else {
    this->CheckUpcomingTokensAreEntRef(
        "First arg of Uses clause not valid ent ref",
        "First arg of Uses clause has not been declared");
  }

  NextToken();
  CheckCurrentTokenSyntax(",", "Expected \',\' for Uses clause");

  NextToken();
  this->CheckUpcomingTokensAreEntRef(
      "Second arg of Uses clause not valid ent ref",
      "Second arg of Uses clause has not been declared");

  NextToken();
  CheckCurrentTokenSyntax(")", "Expected \')\' for Uses clause");

  NextToken();
}

// ---------- HELPERS -------------

EntityType SyntaxChecker::CheckCurrentTokenPatternEntity() {
  std::string token_value = GetCurrTokenValue();
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
  std::string token_value = GetCurrTokenValue();
  if (variable_type == EntityType::ASSIGN) {
    if (GetCurrTokenValue() == "_") {
      if (AParser::GetPeekTokenValue() == ")") {
        // expression-spec = _
        return;
      } else {
        // expression-spec = _"expr"_
        NextToken();
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
  std::string token_value = GetCurrTokenValue();
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
  if (GetCurrTokenValue() != expected_value) {
    throw InvalidSyntaxException(error_msg + ". Got " +
                                 GetCurrTokenValue() + " instead.");
  }
}

void SyntaxChecker::CheckIsExpr(std::string error_msg) {
  std::vector<std::shared_ptr<Token>> infix_tokens;

  while (GetCurrTokenValue() != "\"") {
    if (GetCurrToken()->getTokenType() == TokenType::EOF_TOKEN) {
      throw InvalidSyntaxException(error_msg);
    }
    infix_tokens.push_back(GetCurrToken());
    NextToken();
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
  if (GetCurrTokenValue() == "\"") {
    value += "\"";
    NextToken();
  }
  if (QpParser::IsIdentifier(GetCurrTokenValue()) ||
      GetCurrTokenValue() == "_") {
    value += GetCurrTokenValue();
  }
  if (GetPeekTokenValue() == "\"") {
    value += NextToken()->getTokenVal();
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

  NextToken();
  this->CheckIsExpr(error_msg);

  CheckCurrentTokenSyntax("\"", error_msg);

  NextToken();
}
