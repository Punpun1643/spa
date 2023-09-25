#include "TestWrapper.h"

#include <fstream>
#include <iostream>
#include "../../spa/src/shared/tokenizer/Tokenizer.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/common/SuchThatClause.h"
#include "../../spa/src/query_processing_system/exceptions/InvalidSemanticsException.h"
#include "../../spa/src/query_processing_system/exceptions/InvalidSyntaxException.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
bool volatile AbstractWrapper::GlobalStop = false;

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
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  try {
    qps_controller->HandleQuery(query, results, query_evaluator);
  } catch (InvalidSemanticsException e) {
    results.clear();
//    results.push_back(e.what());
    results.push_back("SemanticError");
  } catch (InvalidSyntaxException e) {
    results.clear();
//    results.push_back(e.what());
    results.push_back("SyntaxError");
  }
}
