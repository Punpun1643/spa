#include "SpParserManager.h"

#include <fstream>
#include <memory>
#include <vector>

#include "../../shared/tokenizer/Tokenizer.h"
#include "../../shared/tokenizer/token/TokenType.h"
#include "../exceptions/EmptySourceFileException.h"
#include "SpParser.h"

SpParserManager::SpParserManager() {}

std::shared_ptr<ProgramNode> SpParserManager::ParseInputFile(
    std::string file_path) {
  std::ifstream input_file_stream(file_path);

  Tokenizer tokenizer = Tokenizer(input_file_stream);

  std::vector<std::shared_ptr<Token>> tokens = tokenizer.Tokenize();

  if (tokens[0]->GetTokenType() == TokenType::EOF_TOKEN) {
    throw EmptySourceFileException();
  }

  SpParser parser(tokens);
  parser.parse();
  std::shared_ptr<ProgramNode> program_node = parser.GetSourceProgramNode();

  return program_node;
}
