#pragma once

#include <memory>
#include <string>

#include "node/ProgramNode.h"
#include "parser/SpParserManager.h"
#include "../program_knowledge_base/PKBSPInterface.h"
#include "../program_knowledge_base/PKBQPSInterface.h"

class SpController {
 public:
  SpController();
  void parseAndExtract(PkbApi& pkb, std::string sourceProgramFilePath);
  ~SpController() = default;
};