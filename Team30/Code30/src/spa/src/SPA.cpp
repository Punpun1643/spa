#include "SPA.h"

#include "program_knowledge_base/PKB.h"
#include "source_processor/SpController.h"

void SPA::process() {
  PKB pkb = PKB();
  SpController spController = SpController();
  spController.ParseAndExtract(pkb, filePath);
}
