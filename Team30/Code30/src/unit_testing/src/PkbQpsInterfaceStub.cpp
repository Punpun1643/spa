#include "PkbQpsInterfaceStub.h"

PkbQpsInterfaceStub::PkbQpsInterfaceStub(){
};

std::unique_ptr<std::vector<std::string>> PkbQpsInterfaceStub::getEntitiesWithType(
    EntityType type) {
  std::unique_ptr<std::vector<std::string>> output =
      std::make_unique<std::vector<std::string>>();
  return {};
}

bool PkbQpsInterfaceStub::isRelationTrueValueValue(std::string value_1, std::string value_2,
                                       RelationType rel_type) {
  valueValueCalls++;
  last_rel_passed = rel_type;
  return false;
}
bool PkbQpsInterfaceStub::isRelationTrueValueWild(std::string value,
                                      RelationType rel_type) {
  valueWildCalls++;
  last_rel_passed = rel_type;
  return false;
}
bool PkbQpsInterfaceStub::isRelationTrueWildValue(std::string value,
                                      RelationType rel_type) {
  wildValueCalls++;
  last_rel_passed = rel_type;
  return true;
}
bool PkbQpsInterfaceStub::isRelationTrueWildWild(RelationType rel_type) {
  wildWildCalls++;
  last_rel_passed = rel_type;
  return true;
}

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getRelationSynonymWild(EntityType entity_type,
                                RelationType rel_type) {
  synonymWildCalls++;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>();  // empty
}
std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getRelationWildSynonym(EntityType entity_type,
                                RelationType rel_type) {
  wildSynonymCalls++;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>();  // empty
}

std::unique_ptr<std::vector<std::string>> PkbQpsInterfaceStub::getRelationSynonymValue(
    EntityType entity_type, std::string value, RelationType rel_type) {
  synonymValueCalls++;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>();  // empty
}
std::unique_ptr<std::vector<std::string>> PkbQpsInterfaceStub::getRelationValueSynonym(
    std::string value, EntityType entity_type, RelationType rel_type) {
  valueSynonymCalls++;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>();  // empty
}

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbQpsInterfaceStub::getRelationSynonymSynonym(EntityType entity_type_1,
                                   EntityType entity_type_2,
                                   RelationType rel_type) {
  synonymSynonymCalls++;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::pair<std::string, std::string>>>();
}

std::unique_ptr<std::vector<std::string>> PkbQpsInterfaceStub::getPatternMatchesWithWildLhs(
    std::string rhs_expr, MatchType expr_match_type) {
  if (expr_match_type == MatchType::WILD_MATCH) {
    auto return_vec = std::vector<std::string>({"1"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else if (expr_match_type == MatchType::PARTIAL_MATCH) {
    auto return_vec = std::vector<std::string>({"2"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else {  // exact
    return std::make_unique<std::vector<std::string>>();
  }
}

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getPatternMatchesWithLhsValue(std::string lhs_value,
                                       std::string rhs_expr,
                                       MatchType expr_match_type) {
  if (expr_match_type == MatchType::WILD_MATCH) {
    auto return_vec = std::vector<std::string>({"3"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else if (expr_match_type == MatchType::PARTIAL_MATCH) {
    auto return_vec = std::vector<std::string>({"4"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else {
    return std::make_unique<std::vector<std::string>>();
  }
};

// 2 paired values - for the implicit assign declaration, and the values for the
// given lhs_entity_type
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbQpsInterfaceStub::getPatternMatchesWithDeclarationLhs(std::string rhs_expr,
                                             MatchType expr_match_type) {
  auto output =
      std::make_unique<std::vector<std::pair<std::string, std::string>>>();

  if (expr_match_type == MatchType::WILD_MATCH) {
    output->push_back(std::make_pair("1", "varX"));

  } else if (expr_match_type == MatchType::PARTIAL_MATCH) {
    output->push_back(std::make_pair("2", "varY"));
  }
  return output;
};
