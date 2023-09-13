#include "SPA.h"
#include "source_processor/SpController.h"
#include "program_knowledge_base/PKB.h"

void SPA::process() {
  PKB pkb = PKB();
        SpController spController = SpController();
        spController.parseAndExtract(pkb, "\"/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/\"\n"
                                     "        \"Sample_source_w4_short.txt\"");
}