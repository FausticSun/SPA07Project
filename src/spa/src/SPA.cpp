#include "SPA.h"
#include "DesignExtractor.h"
#include "GeneralLexer.h"
#include "PQLEvaluator.h"
#include "SIMPLEParser.h"
#include <fstream>

SPA::SPA() : pkb(new PKB()) {}

void SPA::parseSIMPLEFile(std::string filename) {
  try {
    std::ifstream file;
    file.open(filename);
    std::istream &fileStream = file;
    auto tokens = Lexer::tokenize(fileStream);
    auto newPKB = Parser::parseSIMPLE(tokens);
    DesignExtractor::populateDesigns(newPKB);
    pkb.swap(newPKB);
  } catch (std::logic_error e) {
    exit(0);
  }
}
const std::list<std::string> SPA::evaluateQuery(std::string queryString) const {
  list<string> results;
  /*
  try {
    PQLParser pqlParser;
    auto tokens = pqlParser.parse(queryString);
    auto query = pqlParser.buildQuery(tokens);
    PqlEvaluator pe(*pkb);
    auto results = pe.executeQuery(query);
    return results;
  } catch (logic_error le) {
    return results;
  } catch (invalid_argument ia) {
    return results;
  }
  */
  return results;
}
