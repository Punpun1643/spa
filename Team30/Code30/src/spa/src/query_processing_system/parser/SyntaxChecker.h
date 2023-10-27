#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../shared/tokenizer/token/Token.h"
#include "QpParser.h"
#include "query_processing_system/references/PqlDeclaration.h"

class SyntaxChecker : public QpParser {
  std::unordered_map<std::string, PqlDeclaration> existing_declarations;

 public:
  explicit SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;

  ~SyntaxChecker() = default;

 private:
  enum class ClauseType { such_that, pattern, with };
  bool has_semantic_exception;

  // Checkers
  void CheckAffects();
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
  void CheckPatternAssign();
  void CheckPatternIf();
  void CheckPatternWhile();
  void CheckSelect();
  void CheckSelectMultiple();
  void CheckSelectSingle();
  void CheckSuchThat(bool has_and);
  void CheckUses();
  void CheckWith(bool has_and);

  // Helpers
  EntityType CheckCurrentTokenPatternEntity();
  void CheckCurrentTokenPatternFirstArg();
  void CheckCurrentTokenStmtRef(std::string syntax_error_msg,
                                std::string not_existing_error_msg);
  void CheckCurrentTokenSyntax(std::string expected_value,
                               std::string error_msg);
  void CheckExpressionSpec();
  void CheckIsExpr(std::string error_msg);
  void CheckSynonymExists(std::string synonym);
  void CheckUpcomingTokensAreEntRef(std::string syntax_error_msg,
                                    std::string not_existing_error_msg);
  void CheckUpcomingTokensAreQuotedExpr(std::string error_msg);
  void CheckUpcomingTokensAreValidAttrName();
  void CheckUpcomingTokensAreWithRef();
};
