#include "SpController.h"

#include "design_extractor/ExtractionController.h"
#include "parser/SpParserManager.h"

SpController::SpController() {}

void SpController::ParseAndExtract(PKBSPInterface& pkb,
                                   std::string sourceProgramFilePath) {
  SpParserManager parserManager;
  std::shared_ptr<ProgramNode> programNode =
      parserManager.ParseInputFile(sourceProgramFilePath);
  ExtractionController extractionController = ExtractionController(pkb);
  extractionController.ExecuteProgramExtraction(programNode);
}
