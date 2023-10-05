#include <program_knowledge_base/PkbApi.h>

#include <unordered_map>

#include "CallsGraphProcNode.h"
class CallsProcConnector {
 public:
  explicit CallsProcConnector(PkbApi& pkb);

  void connectProcsAndUpdateRelations(
      std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
          procNodeMap);

 private:
  PkbApi& pkb;
};