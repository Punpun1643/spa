#include "TestWrapper.h"

#include <iostream>
#include <fstream>
#include "../../spa/src/shared/tokenizer/Tokenizer.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/common/SuchThatClause.h"


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  this->sp_controller = std::make_shared<SpController>();
  this->pkb = std::make_shared<PKB>();
  this->qps_controller = std::make_shared<QPSController>();
  this->query_evaluator = std::make_shared<QueryEvaluator>(*(this->pkb));
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  this->sp_controller->parseAndExtract(*(this->pkb), filename);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
  // TODO: abstract away into one API call
  std::vector<std::unique_ptr<Clause>> clauses = qps_controller->ParseAndGetClauses(query);
  std::unique_ptr<std::vector<std::string>> query_results;

  std::unique_ptr<Clause> clause = std::move(clauses[0]); // Move ownership
  std::unique_ptr<SelectClause> selectClause(dynamic_cast<SelectClause*>(clause.release()));

  // TODO: Come up with a better design for this
  if (clauses.size() == 1) {
    query_results = this->query_evaluator->evaluateQuery(std::move(selectClause));
  }
  std::copy(query_results->begin(), query_results->end(), std::back_inserter(results));
}
