#include "SyntaxChecker.h"

#include <iostream>
#include <stdexcept>

#include "../exceptions/InvalidSyntaxException.h"

SyntaxChecker::SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens){};

void SyntaxChecker::parse() {
  std::shared_ptr<Token> current_token = getCurrToken();
  if (!IsWordToken(current_token)) {
    throw std::runtime_error(
        "Incorrect query format: Does not start with either 'Select' or a "
        "<declaration>");
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
      throw std::runtime_error("Query is missing a select clause!");
    }
    EntityType entity_type = StringToEntityType(
        getCurrToken()
            ->getTokenVal());  // throws an error if not valid entity_type
    nextToken();               // synonym
    if (!IsSynonym(getCurrToken()->getTokenVal())) {
      throw std::runtime_error("Invalid synonym given in declaration");
    }
    nextToken();  // , OR ;
    if (getCurrToken()->getTokenVal() == ",") {
      while (getCurrToken()->getTokenVal() == ",") {
        nextToken();  // synonym
        if (!IsSynonym(getCurrToken()->getTokenVal())) {
          throw std::runtime_error("Invalid synonym given in declaration");
        }
        nextToken();  // , OR ;
      }
    }
    if (getCurrToken()->getTokenVal() != ";") {
      throw std::runtime_error("Invalid declaration format");
    }
    nextToken();  // entity_type or Select
  }
}

void SyntaxChecker::CheckSelect() {
  if (getCurrToken()->getTokenVal() != "Select") {
    throw std::runtime_error("Select clause missing or invalid");
  }
  // Current token is already "Select"
  nextToken();
  if (!IsSynonym(getCurrToken()->getTokenVal())) {
    throw std::runtime_error("Invalid synonym given for Select clause");
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
    if (!IsRelRef(getCurrToken()->getTokenVal())) {
      throw std::runtime_error("Invalid such that clause");
    }
    if (peekToken()->getTokenVal() == "*") {
      if (!IsTransitiveRelRef(getCurrToken()->getTokenVal())) {
        throw std::runtime_error(getCurrToken()->getTokenVal() +
                                 " clause does not have a transitive version!");
      } else {
        nextToken();  // {
      }
    }
    nextToken();  // (
    if (getCurrToken()->getTokenVal() != "(") {
      throw std::runtime_error("Invalid such that clause syntax!");
    }
    nextToken();  // stmtRef
    // next time need to extend to using relref too
    if (!IsStmtRef(getCurrToken()->getTokenVal())) {
      throw std::runtime_error(
          "First item in such that clause is not a valid stmtRef!");
    }
    nextToken();  // ,
    if (getCurrToken()->getTokenVal() != ",") {
      throw std::runtime_error("Invalid such that clause syntax!");
    }
    nextToken();  // stmtRef
    // next time need to extend to using relref too
    if (!IsStmtRef(getCurrToken()->getTokenVal())) {
      throw std::runtime_error(
          "First item in such that clause is not a valid stmtRef!");
    }
    nextToken();  // )
    if (getCurrToken()->getTokenVal() != ")") {
      throw std::runtime_error("Invalid such that clause structure");
    }
  }
  nextToken();
}

void SyntaxChecker::CheckEOF() {
  if (!IsEOFToken(getCurrToken())) {
    throw InvalidSyntaxException("Invalid query syntax: EOF Token expected");
  }
}
