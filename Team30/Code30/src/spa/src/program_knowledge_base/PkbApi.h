#pragma once
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../query_processing_system/common/EntityType.h"

class PkbApi {
 public:
  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(EntityType type) = 0;
};
