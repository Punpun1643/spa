#pragma once

#include <memory>
#include <string>

#include "node/ProgramNode.h"
#include "parser/SpParserManager.h"
#include "../program_knowledge_base/PKBSPInterface.h"

class SpController {
 public:
  SpController();
  void parseAndExtract(PKBSPInterface& pkb, std::string sourceProgramFilePath);
  ~SpController() = default;
};