#include "SpController.h"

#include <fstream>
#include <memory>

#include "../shared/tokenizer/ATokenizer.h"
#include "parser/SpParser.h"
#include "design_extractor/ExtractionController.h"

SpController::SpController() {}

std::shared_ptr<ProgramNode> SpController::parseInputFile(
    std::string filePath) {
  std::ifstream inputFileStream(filePath);

  ATokenizer tokenizer = ATokenizer(inputFileStream);

  std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

  SpParser parser(tokens);
  parser.parse();
  std::shared_ptr<ProgramNode> programNode = parser.getSourceProgramNode();
  return programNode;
}
