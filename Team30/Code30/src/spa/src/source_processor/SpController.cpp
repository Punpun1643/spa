#include "SpController.h"

#include "design_extractor/ExtractionController.h"
#include "parser/SpParserManager.h"
#include <iostream>

SpController::SpController() {}

void SpController::parseAndExtract(PkbApi& pkb,
                                   std::string sourceProgramFilePath) {
  std::cout << "Address of PKB parseAndExtract: " << &pkb << std::endl;
  SpParserManager parserManager;
  std::shared_ptr<ProgramNode> programNode =
      parserManager.parseInputFile(sourceProgramFilePath);
  ExtractionController extractionController = ExtractionController(pkb);
  extractionController.executeProgramExtraction(programNode);
}