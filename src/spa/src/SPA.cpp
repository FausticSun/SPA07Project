#include "SPA.h"
#include "DesignExtractor.h"
#include "GeneralLexer.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "SIMPLEParser.h"
#include <fstream>
#include <sstream>

SPA::SPA() : pkb(new PKB()) {}

void SPA::parseSIMPLEFile(const std::string &filename) {
  try {
    std::ifstream file;
    file.open(filename);
    std::istream &fileStream = file;
    const auto tokens = Lexer::tokenize(fileStream);
    auto newPKB = Parser::parseSIMPLE(tokens);
    DesignExtractor::populateDesigns(newPKB);
    pkb.swap(newPKB);
  } catch (std::logic_error &e) {
    exit(0);
  }
}

void SPA::evaluateQuery(const std::string &queryString,
                        std::list<std::string> &results) const {
  auto selectBool = false;
  try {
    std::stringstream ss;
    ss << queryString;
    auto tokens = Lexer::tokenize(ss);
    for (auto it = tokens.begin(); it != std::prev(tokens.end()); ++it) {
      if (it->value == "Select" && std::next(it)->value == "BOOLEAN") {
        selectBool = true;
        break;
      }
    }
    auto query = Parser::parsePQL(tokens);
    PqlEvaluator pe(*pkb);
    pe.executeQuery(query, results);
  } catch (Parser::SemanticError &) {
    if (selectBool) {
      results.emplace_back("FALSE");
    }
  } catch (exception le) {
  }
}
