#include "SPA.h"

#include "program_knowledge_base/PKB.h"
#include "source_processor/SpController.h"

void SPA::process() {
  PKB pkb = PKB();
  SpController spController = SpController();
  std::string filePath =
      "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
      "Sample_source_w4_short.txt";
  spController.parseAndExtract(pkb,
                               filePath);
}