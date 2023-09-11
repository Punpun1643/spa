#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <memory>


#include "../source_processor/node/stmt_node/StmtNode.h"
#include "../query_processing_system/common/EntityType.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"
#include "PkbApi.h"

class PKB : public PkbApi{
    EntityDatabase* entData;
    RelDatabase* relData;

public:
    PKB();
    bool insertFollows(std::shared_ptr<StmtNode> stmt1, std::shared_ptr<StmtNode> stmt2);
    std::unique_ptr<std::vector<std::string>> getEntitiesWithType(EntityType type);
    std::optional<std::pair<int, int>> getFollows(int s1_line_num, EntityType s2_type);
    std::optional<std::pair<int, int>> getFollows(EntityType s1_type, int s2_line_num);
    std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(EntityType s1_type, EntityType s2_type);
};