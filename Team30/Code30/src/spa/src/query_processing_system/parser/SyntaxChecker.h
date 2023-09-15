#pragma once

#include "../../shared/tokenizer/token/Token.h"
#include "QpParser.h"

class SyntaxChecker : public QpParser {
 public:
  SyntaxChecker(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;
  void CheckDeclaration();
  void CheckSelect();
  void CheckSuchThatOrPattern();
  void CheckSuchThat();
  void CheckEOF();
};
