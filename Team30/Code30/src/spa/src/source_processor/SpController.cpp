#include "SpController.h"

#include "design_extractor/ExtractionController.h"
#include "parser/SpParserManager.h"

SpController::SpController() {}

void SpController::ParseAndExtract(PKBSPInterface& pkb,
                                   std::string source_program_file_path) {
  SpParserManager parser_manager;
  std::shared_ptr<ProgramNode> program_node =
      parser_manager.ParseInputFile(source_program_file_path);
  ExtractionController extraction_controller = ExtractionController(pkb);
  extraction_controller.ExecuteProgramExtraction(program_node);
}
