#include "ContextBuilder.h"

#include "../common/EntityType.h"
#include "../exceptions/InvalidSyntaxException.h"

ContextBuilder::ContextBuilder(std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {
  this->context = std::make_shared<Context>();
}

std::shared_ptr<Context> ContextBuilder::GetContext() {
  return std::move(this->context);
}

void ContextBuilder::parse() {
  while (GetCurrToken()->getTokenVal() != "Select") {
    this->parseDeclaration();
  }
}

void ContextBuilder::parseDeclaration() {
  EntityType entity_type =
      QpParser::StringToEntityType(GetCurrToken()->getTokenVal());
  std::shared_ptr<std::vector<std::string>> synonym_list(
      std::make_shared<std::vector<std::string>>());
  std::string synonym = NextToken()->getTokenVal();
  this->addSynonym(synonym_list, synonym);
  NextToken();  // ; OR ,
  while (GetCurrToken()->getTokenVal() == ",") {
    synonym = NextToken()->getTokenVal();
    this->addSynonym(synonym_list, synonym);
    NextToken();  // ; OR ,
  }
  NextToken();  // entity type OR select

  this->context->addDeclarations(entity_type, *synonym_list);
}

void ContextBuilder::addSynonym(
    std::shared_ptr<std::vector<std::string>> synonym_list,
    std::string synonym) {
  if (this->existing_synonyms.find(synonym) != this->existing_synonyms.end()) {
    throw InvalidSyntaxException("Synonym declared twice");
  } else {
    this->existing_synonyms.insert(synonym);
    synonym_list->push_back(synonym);
  }
}
