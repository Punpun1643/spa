#include "ContextBuilder.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../exceptions/InvalidSyntaxException.h"
#include "shared/types/EntityType.h"

ContextBuilder::ContextBuilder(std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {
  this->context = std::make_shared<Context>();
}

std::shared_ptr<Context> ContextBuilder::GetContext() {
  return std::move(this->context);
}

void ContextBuilder::parse() {
  while (GetCurrToken()->GetTokenVal() != "Select") {
    this->parseDeclaration();
  }
}

void ContextBuilder::parseDeclaration() {
  EntityType entity_type =
      QpParser::StringToEntityType(GetCurrToken()->GetTokenVal());
  std::shared_ptr<std::vector<std::string>> synonym_list(
      std::make_shared<std::vector<std::string>>());
  std::string synonym = NextToken()->GetTokenVal();
  this->addSynonym(synonym_list, synonym);
  NextToken();  // ; OR ,
  while (GetCurrToken()->GetTokenVal() == ",") {
    synonym = NextToken()->GetTokenVal();
    this->addSynonym(synonym_list, synonym);
    NextToken();  // ; OR ,
  }
  NextToken();  // entity type OR select

  this->context->AddDeclarations(entity_type, *synonym_list);
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
