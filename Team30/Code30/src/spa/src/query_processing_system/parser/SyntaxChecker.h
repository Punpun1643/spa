#pragma once

#include <unordered_set>

#include "../../shared/tokenizer/token/Token.h"
#include "QpParser.h"

class SyntaxChecker : public QpParser {
  std::unordered_set<std::string> existing_declarations;

 public:
  SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;

  ~SyntaxChecker() = default;

 private:
  void CheckDeclaration();
  void CheckEOF();
  void CheckFollows();
  void CheckModifies();
  void CheckParent();
  void CheckPattern();
  void CheckSelect();
  void CheckSuchThat();
  void CheckSuchThatOrPattern();
  void CheckUses();
  // Helper checkers
  void CheckCurrentTokenStmtRef(std::string syntax_error_msg,
                                std::string not_existing_error_msg);
  void CheckCurrentTokenSyntax(std::string expected_value,
                               std::string error_msg);
  void CheckSynonymExists(std::string synonym, std::string error_msg);
  void CheckUpcomingTokensAreEntRef(std::string syntax_error_msg,
                                    std::string not_existing_error_msg);
};
