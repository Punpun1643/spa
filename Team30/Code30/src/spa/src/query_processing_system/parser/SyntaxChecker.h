#pragma once

#include <unordered_map>

#include "../../shared/tokenizer/token/Token.h"
#include "../common/PqlDeclaration.h"
#include "QpParser.h"

class SyntaxChecker : public QpParser {
  std::unordered_map<std::string, PqlDeclaration> existing_declarations;

 public:
  SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;

  ~SyntaxChecker() = default;

 private:
  enum class ClauseType { such_that, pattern, with };

  // Checkers
  void CheckAnd(ClauseType clause_type);
  void CheckCalls();
  void CheckClauses();
  void CheckDeclaration();
  void CheckEOF();
  void CheckFollows();
  void CheckModifies();
  void CheckNext();
  void CheckParent();
  void CheckPattern(bool has_and);
  void CheckSelect();
  void CheckSelectBoolean();
  void CheckSelectMultiple();
  void CheckSelectSingle();
  void CheckSuchThat(bool has_and);
  void CheckUses();
  void CheckWith(bool has_and);

  // Helpers
  EntityType CheckCurrentTokenPatternEntity();
  void CheckCurrentTokenPatternFirstArg(EntityType variable_type);
  void CheckCurrentTokenPatternSecondArg(EntityType variable_type);
  void CheckCurrentTokenStmtRef(std::string syntax_error_msg,
                                std::string not_existing_error_msg);
  void CheckCurrentTokenSyntax(std::string expected_value,
                               std::string error_msg);
  void CheckIsExpr(std::string error_msg);  // TODO
  void CheckSynonymExists(std::string synonym, std::string error_msg);
  void CheckUpcomingTokensAreEntRef(std::string syntax_error_msg,
                                    std::string not_existing_error_msg);
  void CheckUpcomingTokensAreQuotedExpr(std::string error_msg);
  void CheckUpcomingTokensAreValidAttrName();
  void CheckUpcomingTokensAreWithRef();
};
