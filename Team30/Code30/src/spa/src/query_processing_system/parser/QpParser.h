#pragma once

#include <vector>

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"

class QpParser : public AParser {
 public:
  explicit QpParser(std::vector<std::shared_ptr<Token>> tokens);

  virtual ~QpParser() = default;
};
