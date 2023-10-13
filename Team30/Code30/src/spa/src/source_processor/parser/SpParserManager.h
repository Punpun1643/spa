#pragma once

#include <memory>
#include <string>

#include "../node/ProgramNode.h"

class SpParserManager {
 public:
  SpParserManager();
  std::shared_ptr<ProgramNode> ParseInputFile(std::string filePath);
  ~SpParserManager() = default;
};