#pragma once

#include <memory>
#include <string>

#include "../program_knowledge_base/PKBSPInterface.h"
#include "node/ProgramNode.h"
#include "parser/SpParserManager.h"

class SpController {
 public:
  SpController();
  void ParseAndExtract(PKBSPInterface& pkb,
                       std::string source_program_file_path);
  ~SpController() = default;
};
