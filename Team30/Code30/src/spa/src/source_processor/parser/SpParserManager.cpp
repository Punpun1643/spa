#include "SpParserManager.h"

#include <fstream>
#include <memory>

#include "../../shared/tokenizer/ATokenizer.h"
#include "SpParser.h"

SpParserManager::SpParserManager() {}

std::shared_ptr<ProgramNode> SpParserManager::parseInputFile(
    std::string filePath) {
  std::ifstream inputFileStream(filePath);

  ATokenizer tokenizer = ATokenizer(inputFileStream);

  std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

  SpParser parser(tokens);
  parser.parse();
  std::shared_ptr<ProgramNode> programNode = parser.getSourceProgramNode();

  return programNode;
}
