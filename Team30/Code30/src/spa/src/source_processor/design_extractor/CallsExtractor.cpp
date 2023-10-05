#include "CallsExtractor.h"

CallsExtractor::CallsExtractor(PkbApi& pkb) : pkb(pkb) {}

void CallsExtractor::extractCallAbstractions(
    std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
    procNodeMap) {
  for (auto mapNameNode : procNodeMap) {
    std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procsCalled =
        mapNameNode.second->getProcsCalled();
    std::string procCalling = mapNameNode.first;
    for (std::shared_ptr<CallsGraphProcNode> procCalled : procsCalled) {
      //pkb.insertRelation(RelationType::CALLS, procCalling, procCalled);
    }
  }
}