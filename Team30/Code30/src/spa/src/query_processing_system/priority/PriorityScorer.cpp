#include "PriorityScorer.h"

#include <memory>

#include "../context/Context.h"
#include "../expression/NextExpression.h"
#include "../expression/NextTExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/SuchThatExpression.h"
#include "../expression/WithExpression.h"
#include "../helper/Helper.h"

PriorityScorer::PriorityScorer(std::shared_ptr<Context> context)
    : context(context) {}

float PriorityScorer::GetPriorityScore(
    std::shared_ptr<PatternExpression> expression) {
  int num_synonyms = 0;
  int num_new_synonyms = 0;
  int const negated = (expression->IsNot() ? 1 : 0);
  int const clause_type_priority = this->PATTERN_PRIORITY;

  std::string const synonym = expression->GetSynonym();

  this->IncrementSynonymPriorities(synonym, num_synonyms, num_new_synonyms);
  num_new_synonyms = 0;

  return this->CalculatePriorityScore(num_synonyms, num_new_synonyms, negated,
                                      clause_type_priority);
}

float PriorityScorer::GetPriorityScore(
    std::shared_ptr<SuchThatExpression> expression) {
  int num_synonyms = 0;
  int num_new_synonyms = 0;
  int const negated = (expression->IsNot() ? 1 : 0);
  int const clause_type_priority = this->SUCH_THAT_PRIORITY;

  std::string const arg1 = expression->GetArg1();
  std::string const arg2 = expression->GetArg2();

  this->IncrementSynonymPriorities(arg1, num_synonyms, num_new_synonyms);
  this->IncrementSynonymPriorities(arg2, num_synonyms, num_new_synonyms);
  num_new_synonyms = 0;

  return this->CalculatePriorityScore(num_synonyms, num_new_synonyms, negated,
                                      clause_type_priority);
}

float PriorityScorer::GetPriorityScore(
    std::shared_ptr<NextExpression> expression) {
  std::cout << "next123\n";
  float priority_score = 0;
  int num_synonyms = 0;
  int num_new_synonyms = 0;
  int const negated = (expression->IsNot() ? 1 : 0);
  int const clause_type_priority = this->SUCH_THAT_PRIORITY;
  int const such_that_type_priority = this->NEXT_PRIORITY;

  std::string const arg1 = expression->GetArg1();
  std::string const arg2 = expression->GetArg2();

  this->IncrementSynonymPriorities(arg1, num_synonyms, num_new_synonyms);
  this->IncrementSynonymPriorities(arg2, num_synonyms, num_new_synonyms);
  num_new_synonyms = 0;

  return this->CalculatePriorityScore(num_synonyms, num_new_synonyms, negated,
                                      clause_type_priority,
                                      such_that_type_priority);
}

float PriorityScorer::GetPriorityScore(
    std::shared_ptr<NextTExpression> expression) {
  float priority_score = 0;
  int num_synonyms = 0;
  int num_new_synonyms = 0;
  int const negated = (expression->IsNot() ? 1 : 0);
  int const clause_type_priority = this->SUCH_THAT_PRIORITY;
  int const such_that_type_priority = this->NEXT_T_PRIORITY;

  std::string const arg1 = expression->GetArg1();
  std::string const arg2 = expression->GetArg2();

  this->IncrementSynonymPriorities(arg1, num_synonyms, num_new_synonyms);
  this->IncrementSynonymPriorities(arg2, num_synonyms, num_new_synonyms);
  num_new_synonyms = 0;

  return this->CalculatePriorityScore(num_synonyms, num_new_synonyms, negated,
                                      clause_type_priority,
                                      such_that_type_priority);
}

float PriorityScorer::GetPriorityScore(
    std::shared_ptr<WithExpression> expression) {
  int num_synonyms = 0;
  int num_new_synonyms = 0;
  int const negated = (expression->IsNot() ? 1 : 0);
  int const clause_type_priority = this->WITH_PRIORITY;

  std::string const arg1 = expression->GetArg1();
  std::string const arg2 = expression->GetArg2();

  this->IncrementSynonymPriorities(
      arg1.substr(0, arg1.find(Helper::CLAUSE_ARG_DELIMITER)), num_synonyms,
      num_new_synonyms);
  this->IncrementSynonymPriorities(
      arg2.substr(0, arg2.find(Helper::CLAUSE_ARG_DELIMITER)), num_synonyms,
      num_new_synonyms);
  num_new_synonyms = 0;

  return this->CalculatePriorityScore(num_synonyms, num_new_synonyms, negated,
                                      clause_type_priority);
}

// private methods
float PriorityScorer::CalculatePriorityScore(
    int const num_synonyms, int const num_new_synonyms, int const negated,
    int const clause_type_priority, int const such_that_type_priority) {
  float score =
      num_synonyms * this->NUM_SYNONYMS_PRIORITY_NORMALISER +
      num_new_synonyms * this->NUM_NEW_SYNONYMS_PRIORITY_NORMALISER +
      negated * this->NEGATED_CLAUSE_NORMALISER +
      clause_type_priority * this->CLAUSE_TYPE_PRIORITY_NORMALISER +
      such_that_type_priority * this->SUCH_THAT_TYPE_PRIORITY_NORMALISER;

  return score;
}

void PriorityScorer::IncrementSynonymPriorities(std::string string,
                                                int& num_synonyms,
                                                int& num_new_synonyms) {
  if (PriorityScorer::IsSynonym(string)) {
    num_synonyms++;
    if (queried_synonyms.find(string) != queried_synonyms.end()) {
      // synonyms has been queried
      num_new_synonyms++;
    } else {
      queried_synonyms.insert(string);
    }
  }
}

bool PriorityScorer::IsIdentifier(std::string const& string) {
  if (!std::isalpha(string[0])) {
    return false;
  }
  for (size_t i = 1; i < string.length(); i++) {
    if (!std::isalnum(string[i])) {
      return false;
    }
  }
  return true;
}

bool PriorityScorer::IsQuotedIdentifier(std::string const& string) {
  if (string.size() >= 3) {
    return (string.substr(0, 1) == Helper::QUOTE &&
            IsSynonym(string.substr(1, string.size() - 2)) &&
            string.substr(string.size() - 1, 1) == Helper::QUOTE);
  }
  return false;
}

bool PriorityScorer::IsSynonym(std::string const& string) {
  return PriorityScorer::IsIdentifier(string);
}

bool PriorityScorer::IsValidQPInteger(std::string const& string) {
  try {
    stoi(string);
    if (stoi(string) < 0) {
      return false;
    }
  } catch (std::invalid_argument& e) {
    return false;
  }
  if (string.front() == '0' && string.size() > 1) {
    return false;
  }
  return true;
}
