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
      if (dec.find(getCurrToken()->getTokenVal()) != dec.end()) {
        throw InvalidSyntaxException("Synonym cannot be declared twice");
      }
      dec.insert(getCurrToken()->getTokenVal());
    }
    nextToken();  // , OR ;
    while (getCurrToken()->getTokenVal() == ",") {
      nextToken();  // synonym
      if (!IsSynonym(getCurrToken()->getTokenVal())) {
        throw InvalidSyntaxException("Invalid synonym given in declaration");
      } else {
        if (dec.find(getCurrToken()->getTokenVal()) != dec.end()) {
          throw InvalidSyntaxException("Synonym cannot be declared twice");
        }
        dec.insert(getCurrToken()->getTokenVal());
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
    throw InvalidSyntaxException("Invalid synonym gyven for Select clause");
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
    /* std::cout << "\nsc1\n"; */
    throw InvalidSyntaxException("Invalid relref for such that clause");
  }
  if (peekToken()->getTokenVal() == "*") {
    if (!QpParser::IsTransitiveRelRef(getCurrToken()->getTokenVal())) {
      /* std::cout << "\nsc2\n"; */
      throw InvalidSyntaxException(
          "Invalid transitive relref for such that clause");
    } else {
      nextToken();  // *
    }
  }
  nextToken();  // (
  if (getCurrToken()->getTokenVal() != "(") {
    /* std::cout << "\nsc3\n"; */
    throw InvalidSyntaxException("Invalid such that clause syntax");
  }
  nextToken();  // stmtRef
  if (!IsStmtRef(getCurrToken()->getTokenVal()) &&
      !IsEntRef(getCurrToken()->getTokenVal())) {
    /* std::cout << "\nsc4\n"; */
    if (getCurrToken()->getTokenVal() == "\"") {
      if (!(IsSynonym(nextToken()->getTokenVal()))) {
        /* std::cout << "\nsc6: " << getCurrToken()->getTokenVal() << "\n"; */
        throw InvalidSyntaxException(
            "Invalid first arg: Expected synonym within identifier quotes");
      }
      if (nextToken()->getTokenVal() != "\"") {
        /* std::cout << "\nsc7: " << getCurrToken()->getTokenVal() << "\n"; */
        throw InvalidSyntaxException("Invalid first arg: Expected closing \"");
      }
    } else {
      /* std::cout << "\nsc8: " << getCurrToken()->getTokenVal() << "\n"; */
      throw InvalidSyntaxException(
          "First arg in such that clause not valid stmtref/entref");
    }
  }

  nextToken();  // ,
  if (getCurrToken()->getTokenVal() != ",") {
    /* std::cout << "\nsc5\n"; */
    throw InvalidSyntaxException("Invalid such that clause syntax");
  }
  /* std::cout << "\nsccheck1: " << getCurrToken()->getTokenVal() << "\n"; */
  nextToken();  // stmtRef
  if (!(IsStmtRef(getCurrToken()->getTokenVal())) &&
      !(QpParser::IsEntRef(getCurrToken()->getTokenVal()))) {
    if (getCurrToken()->getTokenVal() == "\"") {
      if (!(QpParser::IsSynonym(nextToken()->getTokenVal()))) {
        /* std::cout << "\nsc6: " << getCurrToken()->getTokenVal() << "\n"; */
        throw InvalidSyntaxException(
            "Invalid second arg: Expected synonym within identifier quotes");
      }
      if (nextToken()->getTokenVal() != "\"") {
        /* std::cout << "\nsc7: " << getCurrToken()->getTokenVal() << "\n"; */
        throw InvalidSyntaxException("Invalid second arg: Expected closing \"");
      }
    } else {
      /* std::cout << "\nsc8: " << getCurrToken()->getTokenVal() << "\n"; */
      throw InvalidSyntaxException(
          "Second arg in such that clause not valid stmtref/entref");
    }
  }
  nextToken();  // )
  if (getCurrToken()->getTokenVal() != ")") {
    /* std::cout << "\nsc9\n"; */
    throw InvalidSyntaxException("Invalid such that clause structure");
  }
  nextToken();
}

void SyntaxChecker::CheckSuchThatOrPattern() {
  for (int i = 0; i < 2; i++) {
    CheckSuchThat();
    CheckPattern();
  }
}
