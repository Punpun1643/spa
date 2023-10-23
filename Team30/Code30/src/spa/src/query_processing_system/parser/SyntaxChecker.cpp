#include "SyntaxChecker.h"

#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../exceptions/InvalidSemanticsException.h"
#include "../exceptions/InvalidSyntaxException.h"

SyntaxChecker::SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens), has_semantic_exception(false) {}

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
  CheckEOF();
  if (has_semantic_exception) {
    throw InvalidSemanticsException(
        "Semantic exception found by SyntaxChecker");
  }
}

void SyntaxChecker::CheckAffects() {
  NextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Affects clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for first argument of Affects clause",
      "Synonym in first arg of Affects clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Affects clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for second argument of Affects clause",
      "Synonym in second arg of Affects clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Affects clause");

  NextToken();
}
void SyntaxChecker::CheckAnd(ClauseType clause_type) {
  NextToken();

  if (clause_type == ClauseType::such_that) {
    if (QpParser::IsRelRef(GetCurrTokenValue())) {
      CheckSuchThat(true);
      return;
    } else {
      throw InvalidSyntaxException("Expected <relref> after 'and'");
    }
  } else if (clause_type == ClauseType::pattern) {
    CheckPattern(true);
    return;
  } else if (clause_type == ClauseType::with) {
    CheckWith(true);
    return;
  }

  throw InvalidSyntaxException(
      "Expected either 'such' or 'pattern' after 'and'");
}

void SyntaxChecker::CheckCalls() {
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
  while (GetCurrToken()->GetTokenType() != TokenType::EOF_TOKEN) {
    std::string clause_name = GetCurrTokenValue();
    if (clause_name == QpParser::SUCH) {
      this->CheckSuchThat(false);
    } else if (clause_name == QpParser::PATTERN) {
      this->CheckPattern(false);
    } else if (clause_name == QpParser::WITH) {
      this->CheckWith(false);
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

void SyntaxChecker::CheckNext() {
  NextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Next/* clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for first argument of Next/* clause",
      "Synonym in first arg of Next/* clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Next/* clause");

  NextToken();
  this->CheckCurrentTokenStmtRef(
      "Expected stmtref for second argument of Next/* clause",
      "Synonym in second arg of Next/* clause has not been declared");

  NextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Next/* clause");

  NextToken();
}

void SyntaxChecker::CheckParent() {
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

void SyntaxChecker::CheckPattern(bool has_and) {
  if (!has_and) {
    NextToken();
  }

  NextToken();
  this->CheckCurrentTokenSyntax("(", "Expected \'(\' for Pattern clause");

  NextToken();
  this->CheckCurrentTokenPatternFirstArg();

  NextToken();
  this->CheckCurrentTokenSyntax(",", "Expected \',\' for Pattern clause");

  NextToken();
  this->CheckCurrentTokenPatternSecondArg();

  NextToken();
  this->CheckCurrentTokenSyntax(")", "Expected \')\' for Pattern clause");

  NextToken();
  if (GetCurrTokenValue() == QpParser::AND) {
    CheckAnd(ClauseType::pattern);
  }
}

void SyntaxChecker::CheckSelect() {
  if (GetCurrTokenValue() != QpParser::SELECT) {
    throw InvalidSyntaxException("Expected 'Select'");
  }

  NextToken();  // result-cl
  if (GetCurrTokenValue() == QpParser::BOOLEAN) {
    if (existing_declarations.find(QpParser::BOOLEAN) !=
        existing_declarations.end()) {
      CheckSelectSingle();
    }
  } else if (GetCurrTokenValue() == "<") {
    CheckSelectMultiple();
  } else {
    CheckSelectSingle();
  }
  NextToken();
}

void SyntaxChecker::CheckSelectMultiple() {
  NextToken();  // synonym
  while (GetCurrTokenValue() != ">") {
    if (GetCurrToken()->GetTokenType() == TokenType::EOF_TOKEN) {
      throw InvalidSyntaxException(
          "Reached EOF before reaching '>' for multiple select elements");
    }

    CheckSelectSingle();

    NextToken();  // , or >
    if (GetCurrTokenValue() == ",") {
      NextToken();  // element
      if (GetCurrTokenValue() == ">") {
        throw InvalidSyntaxException("Extra comma detected in multiple select");
      }
    }
  }
}

void SyntaxChecker::CheckSelectSingle() {
  std::string synonym = GetCurrTokenValue();
  if (synonym == QpParser::BOOLEAN) {
    if (existing_declarations.find(QpParser::BOOLEAN) ==
        existing_declarations.end()) {
      throw InvalidSyntaxException(
          "Expected 'BOOLEAN' to be a declared synonym, but has not been "
          "declared");
    }
  }
  if (!IsSynonym(synonym)) {
    throw InvalidSyntaxException("Expected synonym for select clause");
  } else {
    CheckSynonymExists(synonym, "Undeclared synonym used for select clause");
  }
  if (GetPeekTokenValue() == ".") {
    // check attrRef
    NextToken();  // .
    NextToken();  // attrName
    CheckUpcomingTokensAreValidAttrName();
  }
}

void SyntaxChecker::CheckSuchThat(bool has_and) {
  if (!has_and) {
    if (NextToken()->GetTokenVal() != "that") {
      throw InvalidSyntaxException("Expected 'that' in such that clause");
    }
    NextToken();  // relRef
  }
  if (!QpParser::IsRelRef(GetCurrTokenValue())) {
    throw InvalidSyntaxException("Invalid relref for such that clause");
  }
  std::string rel_ref = GetCurrTokenValue();
  if (rel_ref == QpParser::AFFECTS) {
    this->CheckAffects();
  } else if (rel_ref == QpParser::CALLS || rel_ref == QpParser::CALLS_STAR) {
    this->CheckCalls();
  } else if (rel_ref == QpParser::FOLLOWS ||
             rel_ref == QpParser::FOLLOWS_STAR) {
    this->CheckFollows();
  } else if (rel_ref == QpParser::MODIFIES) {
    this->CheckModifies();
  } else if (rel_ref == QpParser::NEXT || rel_ref == QpParser::NEXT_STAR) {
    this->CheckNext();
  } else if (rel_ref == QpParser::PARENT || rel_ref == QpParser::PARENT_STAR) {
    this->CheckParent();
  } else if (rel_ref == QpParser::USES) {
    this->CheckUses();
  }

  if (GetCurrTokenValue() == QpParser::AND) {
    CheckAnd(ClauseType::such_that);
  }
}

void SyntaxChecker::CheckUses() {
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

void SyntaxChecker::CheckWith(bool has_and) {
  if (!has_and) {
    // current token: with
    NextToken();
  }

  CheckUpcomingTokensAreWithRef();

  if (NextToken()->GetTokenVal() != "=") {
    throw InvalidSyntaxException("Expected '=' within 'with' clause");
  }

  NextToken();
  CheckUpcomingTokensAreWithRef();

  if (NextToken()->GetTokenVal() == QpParser::AND) {
    CheckAnd(ClauseType::with);
  }
}

// ---------- HELPERS -------------

EntityType SyntaxChecker::CheckCurrentTokenPatternEntity() {
  std::string token_value = GetCurrTokenValue();
  if (!IsSynonym(token_value)) {
    throw InvalidSyntaxException("Invalid synonym used for pattern");
  } else {
    CheckSynonymExists(token_value,
                       "Variable used for pattern has not been declared");
  }
  if (existing_declarations.at(token_value).GetEntityType() !=
          EntityType::ASSIGN) {
    has_semantic_exception = true;
  }
  return existing_declarations.at(token_value).GetEntityType();
}

void SyntaxChecker::CheckUpcomingTokensAreWithRef() {
  std::string ref = GetCurrTokenValue();
  if (ref == "\"") {
    NextToken();  // ident
    if (IsIdentifier(GetCurrTokenValue())) {
      ref += GetCurrTokenValue();
    } else {
      throw InvalidSyntaxException("Expected valid identifier for with ref");
    }

    NextToken();  // "
    if (GetCurrTokenValue() != "\"") {
      throw InvalidSyntaxException(
          "Expected \" after indentifier for with ref");
    }
    return;
  } else if (QpParser::IsSynonym(ref)) {
    if (GetPeekTokenValue() != ".") {
      throw InvalidSyntaxException("Expected . after synonym for with ref");
    }
    CheckSynonymExists(ref,
                       "Variable used for 'with' clause has not been declared");

    NextToken();  // .

    NextToken();
    CheckUpcomingTokensAreValidAttrName();
    return;
  } else {
    if (!QpParser::IsValidInteger(ref)) {
      throw InvalidSyntaxException("Expected valid integer for with ref");
    }
    return;
  }
  throw InvalidSyntaxException("No valid input for with ref");
}

void SyntaxChecker::CheckCurrentTokenPatternFirstArg() {
  this->CheckUpcomingTokensAreEntRef(
      "First arg of pattern clause not valid ent ref",
      "First arg of pattern clause has not been declared");
}

void SyntaxChecker::CheckCurrentTokenPatternSecondArg() {
  std::string token_value = GetCurrTokenValue();
  if (GetCurrTokenValue() == "_") {
    if (AParser::GetPeekTokenValue() == ")") {
      // expression-spec = _
      return;
    } else {
      // expression-spec = _"expr"_
      NextToken();
      this->CheckUpcomingTokensAreQuotedExpr(
          "Expected quoted expr for pattern second arg");

      NextToken();
      this->CheckCurrentTokenSyntax(
          "_", "Expected '_' at the ending of pattern second arg");
    }
  } else {
    this->CheckUpcomingTokensAreQuotedExpr(
        "Expected quoted expr for pattern second arg");
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
    throw InvalidSyntaxException(error_msg + ". Got " + GetCurrTokenValue() +
                                 " instead.");
  }
}

void SyntaxChecker::CheckIsExpr(std::string error_msg) {
  std::vector<std::shared_ptr<Token>> infix_tokens;

  while (GetCurrTokenValue() != "\"") {
    if (GetCurrToken()->GetTokenType() == TokenType::EOF_TOKEN) {
      throw InvalidSyntaxException(error_msg);
    }
    infix_tokens.push_back(GetCurrToken());
    NextToken();
  }
  try {
    std::queue<std::shared_ptr<std::string>> post_fix =
        AParser::ConvertInfixToPostfix(infix_tokens);
    AParser::BuildExprTreeAndValidate(post_fix);
  } catch (...) {
    throw InvalidSyntaxException(error_msg);
  }
}

void SyntaxChecker::CheckSynonymExists(std::string synonym,
                                       std::string error_msg) {
  if (existing_declarations.find(synonym) == existing_declarations.end()) {
    this->has_semantic_exception = true;
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
    value += NextToken()->GetTokenVal();
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
}

void SyntaxChecker::CheckUpcomingTokensAreValidAttrName() {
  std::string attr_name = GetCurrTokenValue();

  if (attr_name != QpParser::STMT_NUM && attr_name != QpParser::PROC_NAME &&
      attr_name != QpParser::VAR_NAME && attr_name != QpParser::VALUE) {
    throw InvalidSyntaxException("Invalid attr name");
  }
}
