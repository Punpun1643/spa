#include <program_knowledge_base/PkbApi.h>

#include <memory>
#include <unordered_map>

#include "CallsGraphProcNode.h"
class CallsExtractor {
 public:
  explicit CallsExtractor(PkbApi& pkb);

  void extractCallAbstractions(
      std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
          procNodeMap);

 private:
  PkbApi& pkb;
};