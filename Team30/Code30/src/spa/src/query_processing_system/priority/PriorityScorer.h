#pragma once

#include <math.h>

#include <string>
#include <memory>
#include <unordered_set>

class Context;
class PatternExpression;
class SuchThatExpression;
class NextExpression;
class NextTExpression;
class WithExpression;

class PriorityScorer {
 public:
  explicit PriorityScorer(std::shared_ptr<Context> context);

  float GetPriorityScore(std::shared_ptr<PatternExpression> expression);
  float GetPriorityScore(std::shared_ptr<SuchThatExpression> expression);
  float GetPriorityScore(std::shared_ptr<NextExpression> expression);
  float GetPriorityScore(std::shared_ptr<NextTExpression> expression);
  float GetPriorityScore(std::shared_ptr<WithExpression> expression);

 private:
  float const WITH_PRIORITY = 1;
  float const PATTERN_PRIORITY = 2;
  float const SUCH_THAT_PRIORITY = 3;
  float const NEXT_PRIORITY = 98;
  float const NEXT_T_PRIORITY = 99;
  float const NUM_NEW_SYNONYMS_PRIORITY_NORMALISER = 1.0 * pow(10, 0);
  float const NUM_SYNONYMS_PRIORITY_NORMALISER = 1.0 * pow(10, -1);
  float const NEGATED_CLAUSE_NORMALISER = 1.0 * pow(10, -2);
  float const CLAUSE_TYPE_PRIORITY_NORMALISER = 1.0 * pow(10, -3);
  float const SUCH_THAT_TYPE_PRIORITY_NORMALISER = 1.0 * pow(10, -5);

  std::shared_ptr<Context> context;
  std::unordered_set<std::string> queried_synonyms;

  float CalculatePriorityScore(int const num_synonyms,
                               int const num_new_synonyms, int const negated,
                               int const clause_type_priority,
                               int const such_that_type_priority=0);
  void IncrementSynonymPriorities(std::string string, int& num_synonyms,
                                  int& num_new_synonyms);

  bool IsIdentifier(std::string const& string);
  bool IsValidQPInteger(std::string const& string);
  bool IsQuotedIdentifier(std::string const& string);
  bool IsSynonym(std::string const& string);
};

