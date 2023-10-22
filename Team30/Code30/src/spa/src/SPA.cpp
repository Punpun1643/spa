#include "SPA.h"

#include "program_knowledge_base/PKB.h"
#include "source_processor/SpController.h"

void SPA::process() {
  PKB pkb = PKB();
  SpController sp_controller = SpController();
  sp_controller.ParseAndExtract(pkb, file_path);
}
