#pragma once

#include <unordered_set>

#include "../../shared/tokenizer/token/Token.h"
#include "QpParser.h"

class SyntaxChecker : public QpParser {
 public:
  SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;
  void CheckDeclaration();
  void CheckEOF();
  void CheckPattern();
  void CheckSelect();
  void CheckSuchThat();
  void CheckSuchThatOrPattern();

  ~SyntaxChecker() = default;
};
