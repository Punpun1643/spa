#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../shared/tokenizer/token/Token.h"
#include "node/ProgramNode.h"

class SpController {
 public:
  SpController();
  std::shared_ptr<ProgramNode> parseInputFile(std::string filePath);
  ~SpController() = default;
};