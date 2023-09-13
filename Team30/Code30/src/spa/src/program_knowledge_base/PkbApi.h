#pragma once
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include <source_processor/node/stmt_node/StmtNode.h>

class PkbApi {
 public:
  virtual bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                                  std::shared_ptr<StmtNode> stmt2) = 0;
  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                        EntityType s2_type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                        int s2_line_num) = 0;
  virtual std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type) = 0;
  virtual ~PkbApi();  // so that the subclass destructors will be called
};
