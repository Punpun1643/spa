#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "../../program_knowledge_base/PKBSPInterface.h"
#include "../node/util_node/CallsGraphProcNode.h"
class CallsProcConnector {
 public:
  explicit CallsProcConnector(PKBSPInterface& pkb);

  void ConnectProcsAndUpdateRelations(
      std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
          proc_node_map);

 private:
  PKBSPInterface& pkb;
};
