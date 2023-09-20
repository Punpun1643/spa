#include "SyntaxChecker.h"

#include <iostream>
#include <stdexcept>

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
    }
    nextToken();  // , OR ;
    if (getCurrToken()->getTokenVal() == ",") {
      while (getCurrToken()->getTokenVal() == ",") {
        nextToken();  // synonym
        if (!IsSynonym(getCurrToken()->getTokenVal())) {
          throw InvalidSyntaxException("Invalid synonym given in declaration");
        }
        nextToken();  // , OR ;
      }
    }
    if (getCurrToken()->getTokenVal() != ";") {
      throw InvalidSyntaxException("Invalid declaration format");
    }
    nextToken();  // entity_type or Select
  }
}

void SyntaxChecker::CheckSelect() {
  if (getCurrToken()->getTokenVal() != "Select") {
    throw InvalidSyntaxException("Select clause missing or invalid");
  }
  // Current token is already "Select"
  nextToken();
  if (!IsSynonym(getCurrToken()->getTokenVal())) {
    throw InvalidSyntaxException("Invalid synonym gyven for Select clause");
  }
  nextToken();
}

void SyntaxChecker::CheckSuchThatOrPattern() {
  if ((getCurrToken()->getTokenVal() == "such") &&
      (peekToken()->getTokenVal() == "that")) {
    CheckSuchThat();
    /* CheckPattern(); */
  } else {
    ;
    /* CheckPattern(); */
  }
  CheckEOF();
}

void SyntaxChecker::CheckSuchThat() {
  while ((getCurrToken()->getTokenVal() == "such") &&
         (peekToken()->getTokenVal() == "that")) {
    nextToken();  // that
    nextToken();  // relRef
    if (!QpParser::IsRelRef(getCurrToken()->getTokenVal())) {
      throw InvalidSyntaxException("Invalid relref for such that clause");
    }
    if (peekToken()->getTokenVal() == "*") {
      if (!QpParser::IsTransitiveRelRef(getCurrToken()->getTokenVal())) {
        throw InvalidSyntaxException(
            "Invalid transitive relref for such that clause");
      } else {
        nextToken();  // *
      }
    }
    nextToken();  // (
    if (getCurrToken()->getTokenVal() != "(") {
      throw InvalidSyntaxException("Invalid such that clause syntax");
    }
    nextToken();  // stmtRef
    // next time need to extend to using relref too
    if (!IsStmtRef(getCurrToken()->getTokenVal())) {
      throw InvalidSyntaxException(
          "First arg in such that clause not valid stmtref");
    }
    nextToken();  // ,
    if (getCurrToken()->getTokenVal() != ",") {
      throw InvalidSyntaxException("Invalid such that clause syntax");
    }
    nextToken();  // stmtRef
    // next time need to extend to using relref too
    if (!IsStmtRef(getCurrToken()->getTokenVal())) {
      throw InvalidSyntaxException(
          "First arg in such that clause not valid stmtref");
    }
    nextToken();  // )
    if (getCurrToken()->getTokenVal() != ")") {
      throw InvalidSyntaxException("Invalid such that clause structure");
    }
  }
  nextToken();
}

void SyntaxChecker::CheckEOF() {
  if (!IsEOFToken(getCurrToken())) {
    throw InvalidSyntaxException("Invalid query syntax: EOF Token expected");
  }
}
