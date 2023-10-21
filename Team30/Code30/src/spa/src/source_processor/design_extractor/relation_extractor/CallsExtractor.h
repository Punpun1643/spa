#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../../../program_knowledge_base/PKBSPInterface.h"
#include "../../node/util_node/CallsGraphProcNode.h"
class CallsExtractor {
 public:
  explicit CallsExtractor(PKBSPInterface& pkb);

  void ExtractCallAbstractions(
      std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
          proc_node_map);

 private:
  PKBSPInterface& pkb;
};
