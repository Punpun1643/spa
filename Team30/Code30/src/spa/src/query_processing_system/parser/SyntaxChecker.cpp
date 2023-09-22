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

void SyntaxChecker::CheckEOF() {
  if (!IsEOFToken(getCurrToken())) {
    throw InvalidSyntaxException("Invalid query syntax: EOF Token expected");
  }
}

void SyntaxChecker::CheckPattern() {
  if (!(QpParser::IsSynonym(nextToken()->getTokenVal()))) {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: Expected 'syn-assign' after 'pattern'");
  }
  if (nextToken()->getTokenVal() != "(") {
    throw InvalidSyntaxException("Invalid pattern syntax: Expected '('");
  }
  nextToken();  // " OR EntRef
  if (getCurrToken()->getTokenVal() == "\"") {
    if (!(QpParser::IsIdentifier(nextToken()->getTokenVal()))) {
      throw InvalidSyntaxException(
          "Invalid pattern syntax: Expected identifier for first argument");
    }
    if (nextToken()->getTokenVal() != "\"") {
      throw InvalidSyntaxException(
          "Invalid pattern syntax: Invalid identifier syntax for first "
          "argument");
    }
  } else if (QpParser::IsEntRef(getCurrToken()->getTokenVal())) {
    if (!(QpParser::IsIdentifier(getCurrToken()->getTokenVal()))) {
      throw InvalidSyntaxException(
          "Invalid pattern syntax: Expected identifier for first argument");
    }
  } else {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: Expected EntRef for first argument");
  }
  if (nextToken()->getTokenVal() != ",") {
    throw InvalidSyntaxException("Invalid pattern syntax: Expected ','");
  }
  if (nextToken()->getTokenVal() != "_") {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: Expected '_' for second arg");
  }
  nextToken();  // " OR )
  if (getCurrToken()->getTokenVal() == "\"") {
    nextToken();  // factor
    if (!(QpParser::IsSynonym(getCurrToken()->getTokenVal())) &&
        !(AParser::IsIntegerToken(getCurrToken()))) {
      throw InvalidSyntaxException(
          "Invalid pattern syntax: Invalid syntax for <factor> of second arg");
    }
    if (nextToken()->getTokenVal() != "\"") {
      throw InvalidSyntaxException(
          "Invalid pattern syntax for second arg: Expected '\"'");
    }
    if (nextToken()->getTokenVal() != "_") {
      throw InvalidSyntaxException(
          "Invalid patten syntax for second arg: Expected '_'");
    }
    if (nextToken()->getTokenVal() != ")") {
      throw InvalidSyntaxException(
          "Invalid pattern syntax after second arg: Expected ')'");
    }
  } else if (getCurrToken()->getTokenVal() == ")") {
  } else {
    throw InvalidSyntaxException(
        "Invalid pattern syntax: Invalid syntax for second arg");
  }
  nextToken();  // pattern end
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

void SyntaxChecker::CheckSuchThat() {
  while ((getCurrToken()->getTokenVal() == "such") &&
         (peekToken()->getTokenVal() == "that")) {
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
        !QpParser::IsEntRef(getCurrToken()->getTokenVal())) {
      /* std::cout << "\nsc4\n"; */
      throw InvalidSyntaxException(
          "First arg in such that clause not valid stmtref/entref");
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
          throw InvalidSyntaxException(
              "Invalid second arg: Expected closing \"");
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
  }
  nextToken();
}

void SyntaxChecker::CheckSuchThatOrPattern() {
  if ((getCurrToken()->getTokenVal() == "such") &&
      (peekToken()->getTokenVal() == "that")) {
    CheckSuchThat();
  }
  if (getCurrToken()->getTokenVal() == "pattern") {
    this->CheckPattern();
  }
  CheckEOF();
}
