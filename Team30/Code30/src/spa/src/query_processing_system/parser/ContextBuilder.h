#pragma once

#include <set>

#include "../context/Context.h"
#include "QpParser.h"

/**
 * Builds the context for the interpretation of a query expression tree
 *
 * At the end of building (parsing), the pqlDeclarations will have been formed
 * and stored in Context. Exceptions handled by this parser:
 * - Creation of multiple declarations with the same synonym
 *
 */
class ContextBuilder : public QpParser {
 public:
  ContextBuilder(std::vector<std::shared_ptr<Token>> tokens);

  void parse() override;
  std::shared_ptr<Context> GetContext();

  ~ContextBuilder() = default;

 private:
  std::shared_ptr<Context> context;
  std::set<std::string> existing_synonyms;

  void parseDeclaration();
  void addSynonym(std::shared_ptr<std::vector<std::string>> synonym_list,
                  std::string synonym);
};
