#pragma once

#include <math.h>

#include <string>
#include <memory>
#include <unordered_set>

class Context;
class PatternExpression;
class SuchThatExpression;
class WithExpression;

class PriorityScorer {
 public:
  explicit PriorityScorer(std::shared_ptr<Context> context);

  float GetPriorityScore(std::shared_ptr<PatternExpression> expression);
  float GetPriorityScore(std::shared_ptr<SuchThatExpression> expression);
  float GetPriorityScore(std::shared_ptr<WithExpression> expression);

 private:
  float const WITH_PRIORITY = 1;
  float const PATTERN_PRIORITY = 2;
  float const SUCH_THAT_PRIORITY = 3;
  float const NUM_NEW_SYNONYMS_PRIORITY_NORMALISER = 1.0 * pow(10, 0);
  float const NUM_SYNONYMS_PRIORITY_NORMALISER = 1.0 * pow(10, -1);
  float const NEGATED_CLAUSE_NORMALISER = 1.0 * pow(10, -2);
  float const CLAUSE_TYPE_PRIORITY_NORMALISER = 1.0 * pow(10, -3);

  std::shared_ptr<Context> context;
  std::unordered_set<std::string> queried_synonyms;

  float CalculatePriorityScore(int const num_synonyms,
                               int const num_new_synonyms, int const negated,
                               int const clause_type_priority);
  void IncrementSynonymPriorities(std::string string, int& num_synonyms,
                                  int& num_new_synonyms);
};
