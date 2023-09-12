#include "RelDatabase.h"

RelDatabase::RelDatabase() {
  relationships[RelType::PARENT] = ParentsTable();
  //relationships[RelType::FOLLOWS] = FollowsTable();
}
