#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "../query_processing_system/common/EntityType.h"

class PkbApi {
 public:
  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(EntityType type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(int s1_line_num, EntityType s2_type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(EntityType s1_type, int s2_line_num) = 0;
  virtual std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(EntityType s1_type, EntityType s2_type) = 0;
  virtual ~PkbApi(); // so that the subclass destructors will be called
};
